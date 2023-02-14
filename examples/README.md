# Mongoose Network Library Examples

In order to build and run any of the existing examples, please follow:
https://mongoose.ws/documentation/#how-to-build-and-run-examples

## Contributing

Rules for creating a new example:

- Makefile golden reference for desktop/server example: [http-server](http-server)
- Makefile golden reference for the embedded example:
   [stm32/nucleo-f746zg-baremetal](stm32/nucleo-f746zg-baremetal)
- An example must build on Windows, Mac and Ubuntu Linux systems
- Assume that user installed tools according to https://mongoose.ws/tutorials/tools/
- Makefile must not include any other make files
- Use `CFLAGS` for system-specific compilation options
- Use `CFLAGS_MONGOOSE` for mongoose-specific compilation options
- Use `$(CFLAGS) $(CFLAGS_MONGOOSE) $(CFLAGS_EXTRA)` to compile
- If external repository is required, download it on demand using git
  shallow clone. See embedded example golden reference
- Keep Makefile as short as possible, but verbose to understand it easily
- Symlink files when required, like `mongoose.c`, `ca.pem`, etc. Make no copies
- Example's README.md should contain only the title and the link to mongoose.ws
  tutorial page
