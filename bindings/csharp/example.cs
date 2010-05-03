// This is C# example on how to use Mongoose embeddable web server,
// http://code.google.com/p/mongoose
//
// Before using the mongoose module, make sure that Mongoose shared library is
// built and present in the current (or system library) directory

using System;
using System.Runtime.InteropServices;

public class Program {

	// This function is called when user types in his browser http://127.0.0.1:8080/foo
	static private void UriHandler(MongooseConnection conn, MongooseRequestInfo ri) {
		conn.write("HTTP/1.1 200 OK\r\n\r\n");
		conn.write("Hello from C#!\n");
		conn.write("Your user-agent is: " + conn.get_header("User-Agent") + "\n");
	}

    static private void UriDumpInfo(MongooseConnection conn, MongooseRequestInfo ri)
    {
        conn.write("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
        conn.write("<html><body><head>Calling Info</head>");
        conn.write("<p>Request: " + ri.request_method + "</p>");
        conn.write("<p>URI: " + ri.uri + "</p>");
        conn.write("<p>Query: " + ri.query_string + "</p>");
        if (ri.post_data_len > 0) conn.write("<p>Post(" + ri.post_data_len + ")[@" + ri.post_data + "]: '" + Marshal.PtrToStringAnsi(ri.post_data) + "'</p>");
        conn.write("<p>User:" + ri.remote_user + "</p>");
        conn.write("<p>IP: " + ri.remote_ip + "</p>");
        conn.write("<p>HTTP: " + ri.http_version + "</p>");
        conn.write("<p>Port: " + ri.remote_port + "</p>");
        conn.write("<p>NUM Headers: " + ri.num_headers + "</p>");
        for (int i = 0; i < Math.Min(64, ri.num_headers); i++)
        {
            conn.write("<p>" + i + ":" + Marshal.PtrToStringAnsi(ri.http_headers[i].name)
                + ":" + Marshal.PtrToStringAnsi(ri.http_headers[i].value) + "</p>");
        }
        conn.write("</body></html>");
    }

	static void Main() {
		Mongoose web_server = new Mongoose();

		// Set options and /foo URI handler
		web_server.set_option("ports", "8080");
		web_server.set_option("root", "c:\\");
		web_server.set_uri_callback("/foo", new MongooseCallback(UriHandler));
        web_server.set_uri_callback("/dumpinfo", new MongooseCallback(UriDumpInfo));

		// Serve requests until user presses "enter" on a keyboard
		Console.ReadLine();
	}
}
