import json
from chooseTimetable import getTimetable
from http.server import HTTPServer, BaseHTTPRequestHandler


class Serv(BaseHTTPRequestHandler):
    def do_GET(self):
        body = json.dumps(getTimetable()).encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)


if __name__ == "__main__":
    httpd = HTTPServer(("0.0.0.0", 8080), Serv)
    httpd.serve_forever()
