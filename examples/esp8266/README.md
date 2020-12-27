# An example ESP8266 application

To build this application, follow these steps:

1. Make sure you have Docker installed
2. Clone whole mongoose repository on your machine:
  ```sh
  git clone https://github.com/cesanta/mongoose.git
  ```
3. Start command line or terminal, and go into the example's directory:
  ```sh
  cd mongoose/examples/esp8266
  ```
4. Run `make`. This invokes a Docker-based build. A firmware will be built
  in the `src/build/` directory:
  ```sh
  make example
  ```
