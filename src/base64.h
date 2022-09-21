#pragma once
int mg_base64_update(unsigned char p, char *to, int len);
int mg_base64_final(char *to, int len);
int mg_base64_encode(const unsigned char *p, int n, char *to, int tolen);
int mg_base64_decode(const char *src, int n, char *dst, int dstlen);
