# APP_STL := c++_static
APP_STL := gnustl_static

ifeq ($(RUNTIME_SUPPORT), unity)
	APP_ABI := armeabi-v7a x86
else
	APP_ABI := all
endif
