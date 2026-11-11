# Zephyr Project examples

- See detailed tutorial at https://mongoose.ws/tutorials/zephyr/device-dashboard/

The tutorial is specific for the Device Dashboard example but [build instructions](https://mongoose.ws/tutorials/zephyr/device-dashboard/#build-and-run) are generic


## Optional: Using a simulated board

For testing purposes, we build for an x86 target that can be simulated with QEMU, and runs inside a _Docker_ container. Its Ethernet interface is also simulated, and uses the TUN/TAP interface. You can use this to try the examples or for your own development with Zephyr, without the need to have an actual board connected; you only need to have _Docker_ installed as the Zephyr Docker image handles the emulation for you.

- You'll need a TUN/TAP interface (default name is _tap0_) on your host machine; follow the instructions [here](https://github.com/cesanta/mongoose/tree/master/examples/mip-tap#linux-setup) to configure your network to provide a suitable networking environment.
- Run `make build BUILD_ARGS="-b qemu_x86" OVERLAY="-DOVERLAY_CONFIG=overlay-e1000.conf"`.
  - The interface configuration on the simulated machine side is in the file _overlay-e1000.conf_, and defaults to _tap0_. You can change this file or specify a different one by changing the `OVERLAY_CONFIG` argument above.
- Once the build succeeds, run `make run` to run the target. The default _Docker_ configuration is to share _/dev/net/tun_; if this does not work for you, pass the necessary _Docker_ parameters to _make_ with a `DOCKER_ARGS=` argument.
