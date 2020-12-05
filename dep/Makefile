RACK_DIR ?= ..
.NOTPARALLEL:

DEP_LOCAL := .
include $(RACK_DIR)/dep.mk

# add target for precompiled files
dep:
	$(WGET) "https://github.com/lindenbergresearch/Rack.arm64/releases/download/v1.1.6_arm64/Rack.dep.v1.1.6_arm.tgz"
	$(SHA256) Rack.dep.v1.1.6_arm.tgz 799579b71748753b14245d67e7b2295f5b626a30f78dc1790771359acb424f2c
	$(UNTAR) Rack.dep.v1.1.6_arm.tgz
	rm Rack.dep.v1.1.6_arm.tgz
