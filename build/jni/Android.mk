LOCAL_PATH := $(call my-dir)/../..
include $(CLEAR_VARS)

# To build with lua support, uncomment two lines below:
#LUA_SOURCES := build/lua_5.2.1.c build/sqlite3.c build/lsqlite3.c
#LUA_FLAGS := -I$(LOCAL_PATH)/build -DTHREADSAFE=1 -DSQLITE_ENABLE_FTS3 -DSQLITE_ENABLE_FTS3_PARENTHESIS -DUSE_LUA -DUSE_LUA_SQLITE3 -DLUA_COMPAT_ALL -D"getlocaledecpoint() ='.'"

LOCAL_CFLAGS    := -std=c99 -O2 -W -Wall -pthread -pipe $(LUA_FLAGS) $(COPT)
LOCAL_MODULE    := mongoose
LOCAL_SRC_FILES := build/main.c mongoose.c $(LUA_SOURCES)

include $(BUILD_EXECUTABLE)
