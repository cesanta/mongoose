This directory contains Code Composer Studio projects for Mongoose.

 - Mongoose: the library project, produces a static library (`Mongoose.a`).

 - MG_sensor_demo: a demo application that serves files from SLFS and outputs
   sensor data to the WebSocket connection. It uses the Mongoose library project,
   so you'll need to import both.

CC3200SDK_1.2.0 needs to be installed in ${TI_PRODUCTS_DIR} (`C:\TI` on Windows, `~/ti` on Linux).

*Note:* When importing projects, make sure the `Copy projects into workspace` checkbox is *not* checked.
