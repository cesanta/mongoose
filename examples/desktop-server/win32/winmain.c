#include <shellapi.h>
#include <shlobj.h>

static const char *s_server_name = "Mongoose v" MG_VERSION;
static HANDLE hThread;  // Serving thread
static NOTIFYICONDATA TrayIcon;

enum {
  ID_QUIT = 100,  // Menu item IDs
  ID_SEPARATOR,
  ID_SET_DIR,
  ID_ICON_MONGOOSE = 150,  // Icons for mongoose menu
};

static const char *get_my_url(void) {
  static char buf[100];
  char local_ip[50] = "", hostname[200];
  struct hostent *he;
  if (gethostname(hostname, sizeof(hostname)) == 0 &&
      (he = gethostbyname(hostname)) != NULL) {
    snprintf(local_ip, sizeof(local_ip), "%s",
             inet_ntoa(*(struct in_addr *) he->h_addr_list[0]));
  }
  if (local_ip[0] == '\0') snprintf(local_ip, sizeof(local_ip), "127.0.0.1");
  snprintf(buf, sizeof(buf), "http://%s:%d", local_ip,
           mg_url_port(s_listening_address));
  return buf;
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam,
                                   LPARAM lParam) {
  POINT pt;
  HMENU hMenu;
  switch (msg) {
    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        case ID_QUIT:
          Shell_NotifyIcon(NIM_DELETE, &TrayIcon);
          PostQuitMessage(0);
          return 0;
        case ID_SET_DIR: {
          char path[PATH_MAX] = "";
          BROWSEINFO bi;
          memset(&bi, 0, sizeof(bi));
          bi.hwndOwner = hWnd;
          bi.lpszTitle = "Choose root directory:";
          bi.ulFlags = BIF_RETURNONLYFSDIRS;
          SHGetPathFromIDList(SHBrowseForFolder(&bi), path);
          if (path[0] != '\0') {
            FILE *fp = opencfg(__argv[0], "w");
            s_root_dir = strdup(path);
            if (fp != NULL) {
              fprintf(fp, "-d %s\n", path);
              fclose(fp);
            }
          }
          break;
        }
      }
      break;
    case WM_USER:
      switch (lParam) {
        case WM_RBUTTONUP:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK: {
          char buf[1024];
          hMenu = CreatePopupMenu();
          AppendMenu(hMenu, MF_STRING | MF_GRAYED, ID_SEPARATOR, s_server_name);
          AppendMenu(hMenu, MF_SEPARATOR, ID_SEPARATOR, "");
          snprintf(buf, sizeof(buf), "Shared directory: %s", s_root_dir);
          AppendMenu(hMenu, MF_STRING | MF_GRAYED, ID_SEPARATOR, buf);
          snprintf(buf, sizeof(buf), "URL: %s", get_my_url());
          AppendMenu(hMenu, MF_STRING | MF_GRAYED, ID_SEPARATOR, buf);
          AppendMenu(hMenu, MF_SEPARATOR, ID_SEPARATOR, "");
          AppendMenu(hMenu, MF_STRING, ID_SET_DIR,
                     "Change shared directory ...");
          AppendMenu(hMenu, MF_STRING, ID_QUIT, "Exit");
          GetCursorPos(&pt);
          SetForegroundWindow(hWnd);
          TrackPopupMenu(hMenu, 0, pt.x, pt.y, 0, hWnd, NULL);
          PostMessage(hWnd, WM_NULL, 0, 0);
          DestroyMenu(hMenu);
          break;
        }
      }
      break;
    case WM_CLOSE:
      Shell_NotifyIcon(NIM_DELETE, &TrayIcon);
      PostQuitMessage(0);
      return 0;  // We've just sent our own quit message, with proper hwnd.
    default:
      break;
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmdline, int show) {
  WNDCLASS cls;
  HWND hWnd;
  MSG msg = {0};

  init_mongoose(__argc, __argv);
  memset(&cls, 0, sizeof(cls));
  cls.lpfnWndProc = (WNDPROC) WindowProc;
  cls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  cls.lpszClassName = s_server_name;
  RegisterClass(&cls);
  hWnd = CreateWindow(cls.lpszClassName, s_server_name, WS_OVERLAPPEDWINDOW, 0,
                      0, 0, 0, NULL, NULL, NULL, NULL);
  ShowWindow(hWnd, SW_HIDE);

  TrayIcon.cbSize = sizeof(TrayIcon);
  TrayIcon.uID = ID_ICON_MONGOOSE;
  TrayIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  TrayIcon.hIcon =
      LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_ICON_MONGOOSE));
  TrayIcon.hWnd = hWnd;
  snprintf(TrayIcon.szTip, sizeof(TrayIcon.szTip), "%s", s_server_name);
  TrayIcon.uCallbackMessage = WM_USER;
  Shell_NotifyIcon(NIM_ADD, &TrayIcon);

  while (msg.message != WM_QUIT) {
    mg_mgr_poll(&s_mgr, 1);
    if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE) > 0) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  mg_mgr_free(&s_mgr);
  return msg.wParam;
}
