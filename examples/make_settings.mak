# Copyright (c) 2014 Cesanta Software
# All rights reserved

CFLAGS = -W -Wall -I.. -pthread -g -pipe $(CFLAGS_EXTRA)
RM = rm -rf
OUT = -o $@
ALL_PROGS = hello websocket server post multi_threaded upload auth pubsub ws_ssl
NS = ../../../net_skeleton
SW = ../../../ssl_wrapper

ifeq ($(OS),Windows_NT)
  MSVC = ../../vc6
  RM = del /q /f
  OUT =
  CC = $(MSVC)/bin/cl
  CFLAGS = /MD /TC /nologo /W3 /I$(MSVC)/include /I..
  CFLAGS += /link /incremental:no /libpath:$(MSVC)/lib /machine:IX86
  CFLAGS += $(CFLAGS_EXTRA)
else
  UNAME_S := $(shell uname -s)
  CC = g++
  CFLAGS += -g -O0

  ifeq ($(UNAME_S),Linux)
    CFLAGS += -ldl
  endif

  ifeq ($(UNAME_S),Darwin)
  endif
endif
