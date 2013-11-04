LOCAL_PATH:= $(call my-dir)
LIB_ROOT_REL:= ..
LIB_ROOT_ABS:= $(LOCAL_PATH)/..

#mongoose
include $(CLEAR_VARS)
 
LOCAL_CFLAGS := -std=c99 -O2 -W -Wall -pthread -pipe $(COPT)
 
LOCAL_SRC_FILES := \
 $(LIB_ROOT_REL)/main.c \
 $(LIB_ROOT_REL)/../mongoose.c \
 
LOCAL_C_INCLUDES += \
 $(LIB_ROOT_ABS) \
 $(LIB_ROOT_ABS)/.. \
 
LOCAL_MODULE := mongoose
 
include $(BUILD_EXECUTABLE)
