RACK_DIR ?= ..
.NOTPARALLEL:


# Target paths
DEP_LOCAL := .
include $(RACK_DIR)/dep.mk


# Targets
download:
	$(WGET) "https://github.com/lindenbergresearch/Rack.arm64/releases/download/untagged-15bcb6516d1c04923dc7/Rack.dep.v1.1.6_arm.tar.gz"
	$(SHA256) Rack.dep.v1.1.6_arm.tar.gz ad78cd487771558edcfa6d3405d959bbb2050a8d2e8f9c434594f83b3c1ccb7e
	$(UNTAR) Rack.dep.v1.1.6_arm.tar.gz
	rm Rack.dep.v1.1.6_arm.tar.gz

unpack: download
	tar -xzvf Rack.dep.v1.1.6_arm.tar.gz



