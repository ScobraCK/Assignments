var gl;
var points = [];
var NumTimesToSubdivide;

window.onload = function init() {
    var canvas = document.getElementById("gl-canvas");
    gl = WebGLUtils.setupWebGL( canvas );
    if ( !gl ) { alert( "WebGL isn't available" ); }

    // init square as two diagonals AC
    var vertices = [
        vec2(-1, 1),
        vec2(1, -1)
    ]

    NumTimesToSubdivide = parseInt(document.getElementById("depth").value);
    divideSquare(vertices[0], vertices[1], NumTimesToSubdivide);

    // Configure WebGL
    gl.viewport(0, 0, canvas.width, canvas.height);
    gl.clearColor( 1.0, 1.0, 1.0, 1.0 );

    //  Load shaders and initialize attribute buffers
    var program = initShaders( gl, "vertex-shader", "fragment-shader" );
    gl.useProgram( program );

    // Load the data into the GPU
    var bufferId = gl.createBuffer();
    gl.bindBuffer( gl.ARRAY_BUFFER, bufferId );
    gl.bufferData( gl.ARRAY_BUFFER, flatten(points), gl.STATIC_DRAW );

    // Associate out shader variables with our data buffer
    var vPosition = gl.getAttribLocation( program, "vPosition" );
    gl.vertexAttribPointer( vPosition, 2, gl.FLOAT, false, 0, 0 );
    gl.enableVertexAttribArray( vPosition ); 

    // depth slider
    // onchange to reduce CPU load
    document.getElementById("depth").onchange = function() {
        points = [];  // reset points
        document.getElementById("depth_value").textContent = this.value;
        NumTimesToSubdivide = parseInt(this.value);  // event.srcElement is deprecated
        divideSquare(vertices[0], vertices[1], NumTimesToSubdivide);
        gl.bufferData( gl.ARRAY_BUFFER, flatten(points), gl.STATIC_DRAW );  // update buffer
        render();
    }

    // RGB sliders
    var rLoc = gl.getUniformLocation(program, "r");
    var gLoc = gl.getUniformLocation(program, "g");
    var bLoc = gl.getUniformLocation(program, "b");

    // use oninput for real-time updates
    document.getElementById("r").oninput = function() {
        document.getElementById("r_value").textContent = this.value;
        gl.uniform1f(rLoc, this.value);
        render();
    }

    document.getElementById("g").oninput = function() {
        document.getElementById("g_value").textContent = this.value;
        gl.uniform1f(gLoc, this.value);
        render();
    }

    document.getElementById("b").oninput = function() {
        document.getElementById("b_value").textContent = this.value;
        gl.uniform1f(bLoc, this.value);
        render();
    }

    render();
}

function square(A, C) {
    var B = vec2(C[0], A[1]);
    var D = vec2(A[0], C[1]);
    points.push(A, B, C);
    points.push(A, C, D);
}

function divideSquare(A, C, count) {
    if ( count === 0) {
        square(A, C);
    } else {
        var AC1 = mix(A, C, 1/3);
        var AC2 = mix(A, C, 2/3);
        --count;

        //create new squares
        var x_arr = [A[0], AC1[0], AC2[0], C[0]];
        var y_arr = [A[1], AC1[1], AC2[1], C[1]];
        for (let j = 0; j < y_arr.length - 1; j++) { // y
            for (let i = 0; i < x_arr.length - 1; i++) {  // x
                var topLeft = vec2(x_arr[i], y_arr[j])
                var bottomRight = vec2(x_arr[i+1], y_arr[j+1])
                if (equal(topLeft, AC1)) {  // middle square
                    continue;
                } 
                divideSquare(topLeft, bottomRight, count);
            }
        }

    }
}

function render()
{
    gl.clear( gl.COLOR_BUFFER_BIT );
    gl.drawArrays( gl.TRIANGLES, 0, points.length );
}
