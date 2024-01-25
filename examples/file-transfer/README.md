# File Transfer

This example contains minimal HTTP client and server.

The client uploads a file to the server in a single POST, shaping traffic to send small data chunks.

The server manually processes requests in order to be able to write as soon as data arrives, to avoid buffering a whole (possibly huge) file not fitting in RAM.

Uploads are authenticated using Basic Auth. Both client and server have a default user/pass and can be configured using the command line. Only authenticated users can upload a file.

The server can also accept regular uploads from any HTTP client, for example curl:

```sh
	curl -su user:pass  http://localhost:8090/upload/foo.txt --data-binary @Makefile
```

- Follow the [Build Tools](../tools/) tutorial to setup your development environment.
- Start a terminal in this project directory; and build the example:

  ```sh
  cd mongoose/examples/file-transfer
  make clean all
  ```

- Manually start the server, either in background (to reuse the same terminal window) or in foreground; in which case you'll need another terminal to run the client. The server will listen at all interfaces in port 8090

  ```sh
  ./server
  6332b7 2 server.c:157:main              Mongoose version : v7.12
  6332b7 2 server.c:158:main              Listening on     : http://0.0.0.0:8090
  6332b7 2 server.c:159:main              Web root         : [/home/mongoose/examples/file-transfer/web_root]
  6332b7 2 server.c:160:main              Uploading to     : [/home/mongoose/examples/file-transfer/upload]
  ```

- Manually run the client to send a file, default is to send it as "foo.txt" to the server in localhost at port 8090

  ```sh
  ./client -f Makefile
  ok
  ```

Default operation is to assume hardcoded username and password. Call both server and client with no arguments to see usage instructions

See detailed tutorials at
	https://mongoose.ws/tutorials/file-uploads/
	https://mongoose.ws/tutorials/http-server/
	https://mongoose.ws/tutorials/http-client/
