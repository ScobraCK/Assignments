from http.server import HTTPServer, SimpleHTTPRequestHandler
from pathlib import Path
import os

HOST = "localhost"
PORT = 8000

# Serve files relative to this server.py file
os.chdir(Path(__file__).parent)

server = HTTPServer((HOST, PORT), SimpleHTTPRequestHandler)

print(f"Server running at http://{HOST}:{PORT}")
print("Available pages:")
print(f"  http://{HOST}:{PORT}/CarpetV1/carpet.html")
print(f"  http://{HOST}:{PORT}/CarpetV2/carpet.html")
print(f"  http://{HOST}:{PORT}/CarpetV3/carpet.html")
print("Press Ctrl+C to stop the server.")

try:
    server.serve_forever()
except KeyboardInterrupt:
    print("\nServer stopped.")
    server.server_close()