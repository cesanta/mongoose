# Mongoose Network Library tutorials and example code

The fastest way to quickly get Mongoose running on your hardware is to follow our [Tutorials](https://mongoose.ws/documentation/#tutorials):

- Browse for your desired protocol and function, read the tutorial
- For embedded hardware specifics, browse to your desired board, or the closest one. You'll find links to different IDE, RTOS, and TCP/IP stack options, and a link to our [Wizard](https://mongoose.ws/documentation/#mongoose-wizard), a tool that generates UI code for you.

## Navigating this tree

Steps to get the most out of this code base:

- Most code here has a README.md file that points to a verbose tutorial describing how to get the intended functionality out of Mongoose Network Library. Code there runs as is in Windows, Ubuntu Linux, and Mac (very few cases don't run in Windows).
  - Navigate your way to the desired protocol or function, read the README.md file
  - Follow the linked tutorial
  - In cases were there is no tutorial:
    - Follow the [Build Tools](https://mongoose.ws/documentation/tutorials/tools/) tutorial to setup your development environment.
    - run `make all`
- Embedded hardware tutorials are conveniently linked from OS tutorials, or grouped by vendor and board.
  - Navigate your way to the desired board or the closest one, read the README.md file. In most cases you'll find a link to our [Wizard](https://mongoose.ws/wizard/); for others, there is a link to a [Tutorial](https://mongoose.ws/documentation/#tutorials), as described at the top of this text. The Wizard will also link you to the tutorials you'll need.
  - In cases were there you don't find a link to a tutorial:
    - Follow the [Build Tools](https://mongoose.ws/documentation/tutorials/tools/) tutorial to setup your development environment.
    - run `make build`
- Once you have a running example close to your needs, you can start developing your application
  - Please read our [User Guide](https://mongoose.ws/documentation/#user-guide)

## Contributing

Rules for creating a new example:

- Makefile golden reference for desktop/server examples: [http-server](http/http-server/)
- Makefile golden reference for embedded examples: [stm32/nucleo-f746zg-baremetal-builtin-rndis](stm32/nucleo-f746zg-baremetal-builtin-rndis/)
- Examples must build on Windows, Mac and Ubuntu Linux systems
- Assume that users installed their tools according to the [Build Tools](https://mongoose.ws/documentation/tutorials/tools/) tutorial
- Makefiles must not include other Makefiles
- Use `CFLAGS` for system-specific compilation options
- Use `CFLAGS_MONGOOSE` for Mongoose-specific compilation options
- Use `$(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA)` to compile
- If an external repository is required, download it on demand using git shallow clone. See the embedded examples golden reference
- Keep Makefiles as short as possible, but verbose to understand them easily
- Symlink files when required, like `mongoose.c`, `ca.pem`, etc. Make no copies
- Examples README.md files should contain only the title and the link to the corresponding mongoose.ws tutorial page
