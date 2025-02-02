import datetime
import http.server
DEFAULT_VALS = {
        'limit' : 2,
        'window' : 4,
        }

# logger stuff, just ignore it
import logging
import logging.config
import colorlog
import sys

# Define the format and log colors
log_format = '%(asctime)s [%(levelname)s] %(name)s [%(funcName)s]: %(message)s'
log_colors = {
        'DEBUG': 'cyan',
        'INFO': 'green',
        'WARNING': 'yellow',
        'ERROR': 'red',
        'CRITICAL': 'bold_red',
        }

# Create the ColoredFormatter object
console_formatter = colorlog.ColoredFormatter(
        '%(log_color)s' + log_format,
        log_colors = log_colors
        )
stdout_handler = logging.StreamHandler(stream=sys.stdout)
stdout_handler.setFormatter(console_formatter)

logger = logging.getLogger(__name__)
logger.addHandler(stdout_handler)
logger.setLevel("DEBUG")
# end of logger stuff

LIMIT_ERR_MSG = b"\
HTTP/1.1 429 Too Many Requests\r\n\
Content-Type: text/plain\r\n\
Content-Length: 32\r\n\
\r\n\
Too Many Requests - Slow down.\
"

def get_current_timestamp():
    return int(datetime.datetime.now().timestamp())
class rate_limited_http_server(http.server.HTTPServer):
    def __init__(self, 
                 limit = DEFAULT_VALS['limit'], 
                 window = DEFAULT_VALS['window'],
                 *args, **kwargs):

        super().__init__(*args, **kwargs)
        self.limit = limit
        self.window = window
        #                         connection ip address     request timestamps
        self.limit_cache : dict[  int,        list[int]]            = {}

    def verify_request(self, request, client_address):
        client_ip_address = client_address[0]
        try:
            self._remove_out_of_window_records(client_ip_address)
            logger.debug(f"new limit cache:{self.limit_cache=}")
        except KeyError:
            self.limit_cache.update({client_ip_address : [get_current_timestamp()]})
            logger.debug(f"new limit cache:{self.limit_cache=}")
            logger.debug("accepting reason: cache miss")
            return True

        if len(self.limit_cache[client_ip_address]) < self.limit:
            self.limit_cache[client_ip_address].append(get_current_timestamp())
            return True 

        logger.debug("rejecting")
        request.sendall(LIMIT_ERR_MSG)
        request.close()
        return False

    def _remove_out_of_window_records(self, key):
        now = get_current_timestamp()
        list_len = len(self.limit_cache[key])

        logger.debug(f"{now=}")

        #if oldest request is not out of window we simply giveup
        if not self._is_out_of_window(self.limit_cache[key][0], now):
            return

        i = self._nearest_to_out_of_window(key, now)

        assert i >= 0, "invalid state of program"

        logger.debug(f"removing {self.limit_cache[key][i:list_len]} from {key=}, with {i=} and {list_len=}")
        del self.limit_cache[key][0:i]
 
    def _is_out_of_window(self, timestamp : int, now : int):
        return (timestamp < now - self.window)

    def _nearest_to_out_of_window(self, key, now):
        """
        this function tries to find an element outside of window as fast as possible using binary search
        as a result, it might not return the most optimal answer
        it should not be problematic, since the remaining out of window elements can be deleted later on
        """
        low = 0
        high = len(self.limit_cache[key])

        window_edge = now - self.window
        mid = (high  - low) // 2
        while (low < high):
            if self.limit_cache[key][mid] == window_edge:
                return mid - 1
            elif self.limit_cache[key][mid] < window_edge:
                return mid

            high = mid - 1
        return mid - 1
