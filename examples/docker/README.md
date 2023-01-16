# Example Docker image

The Dockerfile shows the minimum steps needed to compile and run an example Mongoose-based HTTP server

Just run

```sh
$ make docker
```

and it will install _gcc_ and _make_, clone the Mongoose repo, and build the http-server example.

That will be turned into a Docker image you can use.

This image is then run in a container mapping your current directory, where a directory named `web_root` is expected (the Makefile has created one for you). Put your HTML tree inside that directory and it will be served by Mongoose. The example Makefile also creates a very simple page showing "OK", connect to Mongoose pointing your browser to `http://localhost:8000`; port 8000 in the container is mapped to your host too.

