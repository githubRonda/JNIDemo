LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := jnidemo
#LOCAL_SRC_FILES := jnidemo.c
LOCAL_SRC_FILES += jnidemo.c \ cfork.c

LOCAL_LDLIBS += -llog #LD: load的意思。 -l 也是load加载的意思。log表示加载的库名，位于ndk/platform/android-xx/arch-arm(其他架构目录也可以)/usr/lib

include $(BUILD_SHARED_LIBRARY)
