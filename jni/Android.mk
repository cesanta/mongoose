LOCAL_PATH := $(call my-dir)/..
include $(CLEAR_VARS)

LOCAL_CFLAGS    := -std=c99 -O2 -W -Wall -pthread -pipe $(COPT)
LOCAL_MODULE    := mongoose
LOCAL_SRC_FILES := examples/hello_world/hello_world.c mongoose.c

include $(BUILD_EXECUTABLE)
