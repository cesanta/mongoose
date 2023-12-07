# Device Dashboard via RNDIS on an RP2040

Your headless Raspberry Pi Pico-based hardware can also have a device dashboard on a web browser when you connect it to your computer via USB

- See detailed tutorial at https://mongoose.ws/tutorials/rp2040/pico-rndis-dashboard/

Note for Mac users: For this example to run on Mac, please set `DUAL_CONFIG=1` in `CMakeLists.txt` before building; this enables CDC-ECM. Please notice that Windows may not recognize the mass-storage device in that case.
