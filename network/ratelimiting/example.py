import rate_limited_http_server
import http.server

rate_limited_http_server.rate_limited_http_server(
        server_address = ('', 8000), 
        limit=5, 
        window=60, 
        RequestHandlerClass = http.server.BaseHTTPRequestHandler).serve_forever()
