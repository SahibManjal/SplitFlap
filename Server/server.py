import json
from chooseTimetable import getTimetable
from http.server import HTTPServer, BaseHTTPRequestHandler


class Serv(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.end_headers()
        self.wfile.write(json.dumps(getTimetable()).encode("utf-8"))


if __name__ == "__main__":
    httpd = HTTPServer(("0.0.0.0", 8080), Serv)
    httpd.serve_forever()
