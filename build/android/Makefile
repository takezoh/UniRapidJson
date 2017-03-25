NAME ?= rapidjson
NDK_BUILD ?= ndk-build
PROJROOT := $(CURDIR)/../..

RUNTIME_SUPPORT := unity


.PHONY: all
all: clean build dist


.PHONY: build
build:
	rm -rf jniLibs
	$(NDK_BUILD) -j4 -C jni RUNTIME_SUPPORT=$(RUNTIME_SUPPORT)
	./gradlew cleanPackageReleaseJniLibs assembleRelease

.PHONY: dist
dist:
	mkdir -p $(PROJROOT)/dist/unity/Android.Any
	rm -f $(PROJROOT)/dist/unity/Android.Any/$(NAME)-android-release.aar
	cp build/outputs/aar/android-release.aar $(PROJROOT)/dist/unity/Android.Any/$(NAME)-android-release.aar

.PHONY: clean
clean:
	$(NDK_BUILD) -C jni clean
	./gradlew clean
