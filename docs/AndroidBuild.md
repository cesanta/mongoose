# Mongoose Build on Android

This is a small guide to help you run mongoose on Android. Currently it is
tested on the HTC Wildfire. If you have managed to run it on other devices
as well, please comment or drop an email in the mailing list.
Note : You dont need root access to run mongoose on Android.

- Clone Mongoose Git repo
- Download the Android NDK from [http://developer.android.com/tools/sdk/ndk/index.html](http://developer.android.com/tools/sdk/ndk/index.html)
- Run `/path-to-ndk/ndk-build -C /path-to-mongoose/build`
  That should generate mongoose/lib/armeabi/mongoose
- Using the adb tool (you need to have Android SDK installed for that),
  push the generated mongoose binary to `/data/local` folder on device.
- From adb shell, navigate to `/data/local` and execute `./mongoose`.
- To test if the server is running fine, visit your web-browser and
  navigate to `http://127.0.0.1:8080` You should see the `Index of /` page.

![screenshot](https://a248.e.akamai.net/camo.github.com/b88428bf009a2b6141000937ab684e04cc8586af/687474703a2f2f692e696d6775722e636f6d2f62676f6b702e706e67)


Notes:

- `jni` stands for Java Native Interface. Read up on Android NDK if you want
  to know how to interact with the native C functions of mongoose in Android
  Java applications.
- TODO: A Java application that interacts with the native binary or a
  shared library.
