src=$(wildcard *.cc,./picsrv/*.cc, ./picsrv/common/*.cc)
dir=$(notdir $(src))
obj=$(patsubst %.cc,%.o,$(dir) )

all:
	@echo $(src)
	@echo $(dir)
	@echo $(obj)
	@echo "end"
