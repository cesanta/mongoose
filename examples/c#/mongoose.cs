// This file is part of mongoose web server project,
// https://github.com/cesanta/mongoose

using System;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)] public struct MongooseHeader {
  [MarshalAs(UnmanagedType.LPTStr)] public IntPtr name;
  [MarshalAs(UnmanagedType.LPTStr)] public IntPtr value;
};

// mongoose.h :: struct mg_request_info
[StructLayout(LayoutKind.Sequential)] public struct MongooseRequestInfo {
  [MarshalAs(UnmanagedType.LPTStr)] public string request_method;
  [MarshalAs(UnmanagedType.LPTStr)] public string uri;
  [MarshalAs(UnmanagedType.LPTStr)] public string http_version;
  [MarshalAs(UnmanagedType.LPTStr)] public string query_string;
  [MarshalAs(UnmanagedType.LPTStr)] public string remote_user;
  public int remote_ip;
  public int remote_port;
  public int is_ssl;
  [MarshalAs(UnmanagedType.ByValArray,SizeConst=64)]
    public MongooseHeader[] http_headers;
};

[StructLayout(LayoutKind.Sequential)] public struct MongooseEvent {
  public int type;
  public IntPtr user_data;
  public IntPtr conn_data;
  public IntPtr event_param;
  public IntPtr conn;
  public IntPtr request_info;
};

public delegate int MongooseEventHandlerN(ref MongooseEvent ev);
public delegate int MongooseEventHandler(MongooseEvent ev);

public class Mongoose {
  public const string dll_name_ = "mongoose";
  public string version_ = "??";

  // These are here to store a ref to the callbacks
  // while they are over in unmanaged code, to prevent garbage collection.
  private event MongooseEventHandlerN delegates;

  private IntPtr ctx_;

  [DllImport(dll_name_)] private static extern
    IntPtr mg_start([MarshalAs(UnmanagedType.LPArray,
                     ArraySubType=UnmanagedType.LPTStr)] string[] options,
                    MongooseEventHandlerN callback,
                    IntPtr user_data);
  [DllImport(dll_name_)] private static extern void mg_stop(IntPtr ctx);
  [DllImport(dll_name_)] private static extern IntPtr mg_version();
  [DllImport(dll_name_)] public static extern int mg_write(IntPtr conn,
      string data, int length);

  public Mongoose(string document_root,
                  string listening_ports,
                  MongooseEventHandler event_handler) {
    version_ = Marshal.PtrToStringAnsi(mg_version());

    string[] options = {
      "document_root", document_root,
      "listening_ports", listening_ports,
      null
    };

    MongooseEventHandlerN cb = delegate(ref MongooseEvent ev) {
      return event_handler(ev);
    };

    // Prevent garbage collection
    delegates += cb;

    ctx_ = mg_start(options, cb, IntPtr.Zero);
  }

  public static int write(IntPtr conn, string data) {
    return mg_write(conn, data, data.Length);
  }

  public void stop() {
    if (this.ctx_ != IntPtr.Zero) {
      mg_stop(this.ctx_);
    }
    this.ctx_ = IntPtr.Zero;
  }

  ~Mongoose() {
    stop();
  }
}
