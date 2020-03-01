ROOT = $(shell pwd)

CC = g++
CFLAGS = -O2 -g -Wall -O -std=c++1z 
CFLAGS += -I . -I include
LDFLAGS = -lworkflow

TARGET = main
#OBJS = build/main.o build/log.o build/server.o 

SRC = $(wildcard ./picsrv/*.cc ./picsrv/common/*.cc)
SRC2 = $(notdir $(SRC))

OBJ = $(patsubst %.cc,build/%.o,$(SRC2))


# other command
#OBJS = $(addprefix build/objs/, $(OBJ))
#ifeq ($(DEBUG), 1)
#        CFLAGS += -g -O0 -D_GLIBCXX_ASSERTIONS
#else
#        CFLAGS += -O2 -g -Wall -O
#endif


.PHONY:clean build

all: $(TARGET)
	@echo "** make all **"

$(TARGET):$(OBJ)
	@mkdir -p $(@D)
	$(CC) $^ -o $@ $(LDFLAGS)

build/%.o:picsrv/%.cc
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

build/%.o:picsrv/common/%.cc
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	@echo "** make clean **"
	rm -rf $(TARGET) $(OBJ) build output

install:
	@echo "** make install **"

	mkdir -p $(DESTDIR)
	mkdir -p $(DESTDIR)/bin
	
	$(if $(DESTDIR),mv $(TARGET) $(DESTDIR)/bin/,)

check:
	@echo "** make check **"
