#$(warning "release libs: $(RELEASE_LIBS) ")
#$(warning "release kos: $(RELEASE_KOS) ")

.PHONY : clean all install uninstall

all: install

clean: uninstall

install:
### .a & .so ###
ifneq ($(RELEASE_LIBS),)
	@mkdir -p $(REL_LIB);cp $(RELEASE_LIBS) $(REL_LIB)
endif

### .h ###
ifneq ($(RELEASE_HEADERS),)
	@mkdir -p $(REL_INC);cp $(RELEASE_HEADERS) $(REL_INC)
endif

### .ko ###
ifneq ($(RELEASE_KOS),)
	@mkdir -p $(REL_KO);cp $(RELEASE_KOS) $(REL_KO)
endif

### datas ###
ifneq ($(RELEASE_DATAS),)
	@mkdir -p $(REL_DATA);cp $(RELEASE_DATAS) $(REL_DATA)
endif

uninstall:
### .a & .so ###
ifneq ($(RELEASE_LIBS),)
	@if [ -d $(REL_LIB) ]; then cd $(REL_LIB); rm  $(notdir $(RELEASE_LIBS)) -f; fi
endif

### .h ###
ifneq ($(RELEASE_HEADERS),)
	@if [ -d $(REL_INC) ]; then cd $(REL_INC); rm  $(notdir $(RELEASE_HEADERS)) -f; fi
endif

### .ko ###
ifneq ($(RELEASE_KOS),)
	@if [ -d $(REL_KO) ]; then cd $(REL_KO); rm  $(notdir $(RELEASE_KOS)) -f; fi
endif

### datas ###
ifneq ($(RELEASE_DATAS),)
	@if [ -d $(REL_DATA) ]; then cd $(REL_DATA); rm  $(notdir $(RELEASE_DATAS)) -f; fi
endif
