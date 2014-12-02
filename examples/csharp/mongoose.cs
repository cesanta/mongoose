// This file is part of mongoose web server project,
// https://github.com/cesanta/mongoose

using System;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)] public struct MongooseHeader {
  [MarshalAs(UnmanagedType.LPTStr)] public IntPtr name;
  [MarshalAs(UnmanagedType.LPTStr)] public IntPtr value;
};

// mongoose.h :: struct mg_connection
[StructLayout(LayoutKind.Sequential)] public struct MongooseConnection {
  [MarshalAs(UnmanagedType.LPTStr)] public string request_method;
  [MarshalAs(UnmanagedType.LPTStr)] public string uri;
  [MarshalAs(UnmanagedType.LPTStr)] public string http_version;
  [MarshalAs(UnmanagedType.LPTStr)] public string query_string;

	[MarshalAs(UnmanagedType.ByValArray,SizeConst=48)] public char[] remote_ip;
	[MarshalAs(UnmanagedType.LPTStr)] public string local_ip;
	[MarshalAs(UnmanagedType.U2)] public short remote_port;
	[MarshalAs(UnmanagedType.U2)] public short local_port;
	
	[MarshalAs(UnmanagedType.SysInt)] public int num_headers;
  [MarshalAs(UnmanagedType.ByValArray,SizeConst=30)]
    public MongooseHeader[] http_headers;
	
	[MarshalAs(UnmanagedType.LPTStr)] public IntPtr content;
	[MarshalAs(UnmanagedType.SysInt)] public int content_len;
	
	[MarshalAs(UnmanagedType.SysInt)] public int is_websocket;
	[MarshalAs(UnmanagedType.SysInt)] public int status_code;
	[MarshalAs(UnmanagedType.SysInt)] public int wsbits;
};

public delegate int MongooseEventHandler(IntPtr c, int ev);

public class Mongoose {
  public const string dll_ = "mongoose";
  private IntPtr server_;

	[DllImport(dll_)] private static extern IntPtr
		mg_create_server(IntPtr user_data, MongooseEventHandler eh);
	[DllImport(dll_)] private static extern int
		mg_poll_server(IntPtr server, int milli);
  [DllImport(dll_)] private static extern IntPtr
		mg_set_option(IntPtr server, string name, string value);
	[DllImport(dll_)] public static extern int
		mg_send_data(IntPtr conn, string data, int length);	

  public Mongoose(string document_root,
                  string listening_port,
                  MongooseEventHandler event_handler) {
    server_ = mg_create_server(IntPtr.Zero, event_handler);
		mg_set_option(server_, "document_root", document_root);
		mg_set_option(server_, "listening_port", listening_port);
  }
	
  public static int send_data(IntPtr conn, string data) {
    return mg_send_data(conn, data, data.Length);
  }
	
	public void poll(int milli) {
    mg_poll_server(server_, milli);
  }
	
	// TODO: add destructor and call mg_destroy_server()
}
