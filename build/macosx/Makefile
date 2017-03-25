TARGET := rapidjson
XCODEBUILD ?= xcodebuild
PROJROOT := $(CURDIR)/../..

RUNTIME_SUPPORT := unity

ifdef RUNTIME_SUPPORT
	TARGET := $(TARGET)-$(RUNTIME_SUPPORT)
endif

CMD_ARGS := -project $(CURDIR)/rapidjson.xcodeproj -target "$(TARGET)" -configuration "Release"


.PHONY: all
all: clean build dist

.PHONY: build
build:
	$(XCODEBUILD) $(CMD_ARGS)

.PHONY: dist
dist:
	mkdir -p $(PROJROOT)/dist/unity/OSX.Any
	rm -rf $(PROJROOT)/dist/unity/OSX.Any/rapidjson.bundle
	cp -r $(CURDIR)/build/Release/rapidjson-unity.bundle $(PROJROOT)/dist/unity/OSX.Any/rapidjson.bundle

.PHONY: clean
clean:
	$(XCODEBUILD) $(CMD_ARGS) clean
