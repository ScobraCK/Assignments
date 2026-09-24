var gl;
var points = [];
var NumTimesToSubdivide = 5;

window.onload = function init() {
    var canvas = document.getElementById("gl-canvas");
    gl = WebGLUtils.setupWebGL( canvas );
    if ( !gl ) { alert( "WebGL isn't available" ); }

    // init square as two diagonals AC
    var vertices = [
        vec2(-1, 1),
        vec2(1, -1)
    ]

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
    render();
}

/*
 * square()
 * 
 * A -- B
 * | \\ |
 * D -- C
 * diagram to show abc, adc would form a square made from triangles
 *
 * It does not matter if the two given points are AC specifically. Any diagonal points will work.
 * As long as two arbitrary diagonal points(X, Y) are given var b and d will always be the other two points.
 * And the resulting two triagnles will have XY as a side. Example below.
 * X  *  | *  X  | Y  *
 * *  Y  | Y  *  | *  X
*/

function square(A, C) {
    var B = vec2(C[0], A[1]);
    var D = vec2(A[0], C[1]);
    points.push(A, B, C);
    points.push(A, C, D);
}


/*
 * divideSquare()
 * 
 * A             B
 *
 *    AC1   BD1
 *
 *    BD2   AC2
 *
 * D             C

 * AC1 divides AC by 1:2, and AC2 by 2:1. Same for BD.
 * Initially ABCD was used to form the square. This meant that I would need to define 4 vec2 points for
 * 8 squares. And we already can get the x, y coordinates from A, AC1, AC2, and C. Thus by using only diagonals(AC) * * we can reduce the variables by half.
 * NOTE: this only works if the square is not tilted
 */

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
