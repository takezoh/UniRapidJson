LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

NDK_APP_DST_DIR := ../jniLibs/$(TARGET_ARCH_ABI)

LOCAL_MODULE := rapidjson
LOCAL_MODULE_FILENAME := librapidjson

LOCAL_CPPFLAGS += -std=c++11

SRC_ROOT := $(LOCAL_PATH)/../../../src

define cpp_src_files
	$(shell find $(1) -name "*.cpp")
endef

LOCAL_SRC_FILES := \
	$(call cpp_src_files, $(SRC_ROOT)/platform/android)

LOCAL_C_INCLUDES := \
	$(SRC_ROOT) \
	$(SRC_ROOT)/include \
	$(SRC_ROOT)/../rapidjson/include

ifeq ($(RUNTIME_SUPPORT), unity)
	LOCAL_SRC_FILES += \
		$(SRC_ROOT)/runtime-support/unity/native/unity-support.cpp

	include $(BUILD_SHARED_LIBRARY)
else
	include $(BUILD_STATIC_LIBRARY)
endif
