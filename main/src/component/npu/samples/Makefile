TARGET = "ortm_run_model"
TARGET += "sample_shared_lib"
TARGET += "mobilenetv2_rgbplanar"
TARGET += "resnet50_binary"
TARGET += "custom_abs"
TARGET += "sample_api_user_guide"

$(warning echo "TARGET: $(TARGET)")
TARGET_CLEAN = $(addsuffix _clean,$(TARGET))

.PHONY:clean all $(TARGET) $(TARGET_CLEAN)

all: $(TARGET)
	@echo "~~~~~~~~~~~~~~Build NPU Sample SUCCESS~~~~~~~~~~~~~~"

clean:$(TARGET_CLEAN)
	@echo "~~~~~~~~~~~~~~Clean NPU Sample SUCCESS~~~~~~~~~~~~~~"


$(TARGET):
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

$(TARGET_CLEAN):
	@echo "~~~~~~~~~~Start clean $(subst _clean,,$@)~~~~~~~~~~"
	@cd $(subst _clean,,$@) && $(MAKE) clean
