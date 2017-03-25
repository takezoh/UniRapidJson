PROJROOT := $(CURDIR)/../..


.PHONY: all
all: clean build dist

.PHONY: build
build:
	mkdir build

	cp -r $(PROJROOT)/src/runtime-support/unity/native build/src
	cp -r $(PROJROOT)/rapidjson/include/rapidjson build/src
	mkdir build/src/platform
	cp $(PROJROOT)/src/platform/*.* build/src/platform

.PHONY: dist
dist:
	mkdir -p $(PROJROOT)/dist/unity/Internal.Any
	rm -rf $(PROJROOT)/dist/unity/Internal.Any/rapidjson.plugin
	cp -r build/src $(PROJROOT)/dist/unity/Internal.Any/rapidjson.plugin

.PHONY: clean
clean:
	rm -rf build
