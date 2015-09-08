# Mongoose-based HTTP load balancer

## Configuration

Load balancer is configured with command-line flags.

### Global flags

* `-p port` – TCP port to listen on. Default: 8000.
* `-l log_file` – path to the log file. Default: none.
* `-s ssl_cert` – path to SSL certificate. Default: none.

### Backend configuration

Main flag is `-b uri_prefix host_port` – it adds a new backend for a given
URI prefix. Example: `-b /stuff/ 127.0.0.1:8080` will route all requests that
start with '/stuff/' to a backend at port 8080 on localhost. There is a special
syntax for `uri_prefix` that allows you to change the URIs that get passed to
backends:

* `-b /stuff/=/ 127.0.0.1:8080` – for '/stuff/thing' backend will see '/thing'.
* `-b /stuff/=/other/ 127.0.0.1:8080` – '/stuff/thing' => '/other/thing'.

Also there are few per-backend flags that can be placed before `-b` and apply
only to the next backend:

* `-r` – instead of proxying requests load balancer will reply with 302 
redirect.
* `-v vhost` – match not only URI prefix but 'Host:' header as well.

### Example

```
load_balancer -s path/to/cert.pem \
  -v example.com -b /site/=/ 127.0.0.1:8080 \
  -b /static/ 127.0.0.1:8081 \
  -b /static/ 127.0.0.1:8082
``` 

In this example requests to 'example.com/site/' will be forwarded to the
backend on port 8080 with '/site' prefix stripped off and requests to
'/static/' on any virtual host will be balanced in round-robin fashion between
backends on ports 8081 and 8082.

