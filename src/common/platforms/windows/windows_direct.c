/*
 * Copyright (c) 2017 Cesanta Software Limited
 * All rights reserved
 */

#ifdef _WIN32

int rmdir(const char *dirname) {
  return _rmdir(dirname);
}

unsigned int sleep(unsigned int seconds) {
  Sleep(seconds * 1000);
  return 0;
}

#endif /* _WIN32 */
