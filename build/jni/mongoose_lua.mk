LOCAL_PATH:= $(call my-dir)
LIB_ROOT_REL:= ..
LIB_ROOT_ABS:= $(LOCAL_PATH)/..

#lua
include $(CLEAR_VARS)
 
SQLITE_FLAGS := -DTHREADSAFE=1 -DSQLITE_ENABLE_FTS3 \
               -DSQLITE_ENABLE_FTS3_PARENTHESIS
LUA_FLAGS := -DUSE_LUA -DUSE_LUA_SQLITE3 -DLUA_COMPAT_ALL -D"getlocaledecpoint() ='.'"

LOCAL_CFLAGS := $(SQLITE_FLAGS) $(LUA_FLAGS)

LOCAL_SRC_FILES := \
 $(LIB_ROOT_REL)/sqlite3.c \
 $(LIB_ROOT_REL)/lsqlite3.c \
 $(LIB_ROOT_REL)/lua_5.2.1.c \
 
LOCAL_C_INCLUDES += \
 $(LIB_ROOT_ABS) \
 $(LIB_ROOT_ABS)/.. \
 
LOCAL_SHARED_LIBRARIES := 
 
LOCAL_MODULE := liblua
 
include $(BUILD_SHARED_LIBRARY)


#mongoose
include $(CLEAR_VARS)
 
LOCAL_CFLAGS := -std=c99 -O2 -W -Wall -pthread -pipe $(COPT) -DUSE_LUA
 
LOCAL_SRC_FILES := \
 $(LIB_ROOT_REL)/main.c \
 $(LIB_ROOT_REL)/../mongoose.c \
 
LOCAL_C_INCLUDES += \
 $(LIB_ROOT_ABS) \
 $(LIB_ROOT_ABS)/.. \
 
LOCAL_SHARED_LIBRARIES := liblua
 
LOCAL_MODULE := mongoose
 
include $(BUILD_EXECUTABLE)