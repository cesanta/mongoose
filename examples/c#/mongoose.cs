// This file is part of mongoose web server project,
// https://github.com/cesanta/mongoose

using System;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)] public struct MongooseHeader {
  public IntPtr name;    // Using IntPtr here because if we use strings here,
  public IntPtr value;   // it won't be properly marshalled.
};

// mongoose.h :: struct mg_request_info
[StructLayout(LayoutKind.Sequential)] public struct MongooseRequestInfo {
  public string request_method;
  public string uri;
  public string http_version;
  public string query_string;
  public string remote_user;
  public int remote_ip;
  public int remote_port;
  public int is_ssl;
  public IntPtr user_data;
  public IntPtr conn_data;
  [MarshalAs(UnmanagedType.ByValArray,SizeConst=64)]
    public MongooseHeader[] http_headers;
};

[UnmanagedFunctionPointer(CallingConvention.StdCall)] public delegate
  int MongooseBeginRequest_native(IntPtr conn);
public delegate int MongooseBeginRequest(MongooseConnection conn);

// mongoose.h :: struct mg_callbacks
[StructLayout(LayoutKind.Sequential)] public struct MongooseCallbacks {
  public MongooseBeginRequest_native begin_request;
  [MarshalAs(UnmanagedType.ByValArray,SizeConst=20)]
    public IntPtr[] other_callbacks;
};

public class Mongoose {
  public const string dll_name_ = "mongoose";
  public string version_ = "??";

  // These are here to store a ref to the callbacks
  // while they are over in unmanaged code, to prevent garbage collection.
  private event MongooseBeginRequest_native delegates;

  private IntPtr ctx_;

  [DllImport(dll_name_)] private static extern
    IntPtr mg_start([MarshalAs(UnmanagedType.LPArray,
                     ArraySubType=UnmanagedType.LPTStr)] string[] options,
                    ref MongooseCallbacks callbacks,
                    IntPtr user_data);
  [DllImport(dll_name_)] public static extern
    IntPtr mg_get_request_info(IntPtr conn);
  [DllImport(dll_name_)] private static extern void mg_stop(IntPtr ctx);
  [DllImport(dll_name_)] private static extern string mg_version();

  public Mongoose(string document_root, string listening_ports,
                  MongooseBeginRequest request_handler) {
    version_ = mg_version();

    string[] options = {
      "document_root", document_root,
      "listening_ports", listening_ports,
      null
    };

    MongooseBeginRequest_native begin_request_cb = delegate(IntPtr conn) {
      return request_handler(new MongooseConnection(conn, this));
    };
    MongooseCallbacks callbacks = new MongooseCallbacks();
    callbacks.begin_request = begin_request_cb;

    // Prevent garbage collection
    delegates += begin_request_cb;

    ctx_ = mg_start(options, ref callbacks, IntPtr.Zero);
  }

  ~Mongoose() {
    if (this.ctx_ != IntPtr.Zero) {
      mg_stop(this.ctx_);
    }
    this.ctx_ = IntPtr.Zero;
  }
}

public class MongooseConnection {
  public Mongoose mongoose;
  private IntPtr conn;

  [DllImport(Mongoose.dll_name_)] private static extern
    string mg_get_header(IntPtr conn, string name);
  [DllImport(Mongoose.dll_name_)] private static extern
    string mg_get_var(IntPtr conn, string name);
  [DllImport(Mongoose.dll_name_)] public static extern
    int mg_write(IntPtr conn, string data, int length);

  public MongooseConnection(IntPtr conn_, Mongoose mongoose_) {
    mongoose = mongoose_;
    conn = conn_;
  }

  public string get_header(string header_name) {
    return mg_get_header(this.conn, header_name);
  }

  public string get_var(string header_name) {
    string s = mg_get_var(this.conn, header_name);
    string copy = "" + s;
    return copy;
  }

  public int write(string data) {
    return mg_write(this.conn, data, data.Length);
  }
}
