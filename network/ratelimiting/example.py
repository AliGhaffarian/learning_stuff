import rate_limited_http_server
import http
import http.server

rate_limited_http_server.rate_limited_http_server(
        server_address = ('', 8000), 
        limit=2, 
        window=5, 
        RequestHandlerClass = http.server.BaseHTTPRequestHandler).serve_forever()
