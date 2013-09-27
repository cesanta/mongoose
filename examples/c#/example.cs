// This file is part of mongoose web server project,
// https://github.com/cesanta/mongoose

using System;
using System.Runtime.InteropServices;

public class Program {
  static private int RequestHandler(MongooseConnection conn) {
    return 0;
    /*
    conn.write("HTTP/1.1 200 OK\r\n\r\n");
    conn.write("Hello from C#!\n");
    conn.write("Your user-agent is: " + conn.get_header("User-Agent") + "\n");
    */
  }

  static void Main() {
    Mongoose web_server = new Mongoose("c:\\", "8080",
        new MongooseBeginRequest(RequestHandler));

    Console.WriteLine("Mongoose v." + web_server.version_ + " started.");
    Console.WriteLine("Press enter to exit program.");

    // Serve requests until user presses "enter" on a keyboard
    Console.ReadLine();
  }
}
