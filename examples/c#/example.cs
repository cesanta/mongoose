// This file is part of mongoose web server project,
// https://github.com/cesanta/mongoose

using System;
using System.Runtime.InteropServices;

public class Program {
  static private int EventHandler(MongooseEvent ev) {
    if (ev.type != 1) {
      return 0; // Mark as unhandled
    }

    MongooseRequestInfo request_info = (MongooseRequestInfo)
      Marshal.PtrToStructure(ev.request_info, typeof(MongooseRequestInfo));

    if (request_info.uri != "/test") {
      return 0; // Mark as unhandled
    }

    Mongoose.write(ev.conn, "HTTP/1.1 200 OK\r\n\r\n");
    Mongoose.write(ev.conn, "Hello from C#!\n");

    return 1; // Mark as handled
  }

  static void Main() {
    Mongoose web_server = new Mongoose(".", "9000",
        new MongooseEventHandler(EventHandler));

    Console.WriteLine("Mongoose v." + web_server.version_ + " started.");
    Console.WriteLine("Press enter to exit program.");

    // Serve requests until user presses "enter" on a keyboard
    Console.ReadLine();
    web_server.stop();
  }
}
