// This file is part of mongoose web server project,
// https://github.com/cesanta/mongoose

using System;

public class Program {
  static private int EventHandler(IntPtr conn_ptr, int ev) {
		MongooseConnection conn = (MongooseConnection)
			System.Runtime.InteropServices.Marshal.PtrToStructure(
				conn_ptr , typeof(MongooseConnection));

		if (ev == 102) {
			// MG_AUTH
			return 1;
		} else if (ev == 103) {
			// MG_REQUEST
	    Mongoose.send_data(conn_ptr, "Hello from C#!\n");
			Mongoose.send_data(conn_ptr, "URI: " + conn.uri + "\n");
			Mongoose.send_data(conn_ptr, "HTTP Headers:\n");
			
			for (int i = 0; i < conn.num_headers; i++) {
				IntPtr name = conn.http_headers[i].name;
				IntPtr val = conn.http_headers[i].value;
				System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name);
				Mongoose.send_data(conn_ptr, "  " +
					System.Runtime.InteropServices.Marshal.PtrToStringAnsi(name) + ": " +
						System.Runtime.InteropServices.Marshal.PtrToStringAnsi(val) + "\n");
			}			
      return 1;
		}
		return 0;
  }

  static void Main() {
    Mongoose web_server = new Mongoose(".", "9001",
        new MongooseEventHandler(EventHandler));

    Console.WriteLine("Mongoose started, press Ctrl-C to exit.");
		for (;;) {
			web_server.poll(1000);
		}
  }
}
