ROOT = $(shell pwd)

include subtask.mk

CXX = g++
CC = gcc
PROTOBUF_HOME = /opt/sogou/protobuf3/root/usr/
PROTOC = $(PROTOBUF_HOME)/bin/protoc

CXXFLAGS = -Wall -O -std=c++1z 
CFLAGS = -Wall -O 
CXXFLAGS += -I . -I include -I _include -I _include/workflow -I src -I $(PROTOBUF_HOME)/include 
CFLAGS += -I . -I include -I _include -I _include/workflow -I src -I $(PROTOBUF_HOME)/include 
LDFLAGS = -L _lib -lworkflow -L $(PROTOBUF_HOME)/lib64 -lprotobuf -lpthread -lssl -lcrypto -lrt

ifeq ($(DEBUG), 1)
	CXXFLAGS += -O0 -g
else 
	CXXFLAGS += -O2 -g -fPIC
endif

TARGET = main
ST_LIB = _lib/libworkflow.a

SRC = $(wildcard src/*.cc \
		src/kernel/*.cc \
		src/config/*.cc \
		src/common/*.cc \
		src/util/*.cc \
		src/proto/server_config.pb.cc)

#SRC2 = $(notdir $(SRC))
#OBJ = $(patsubst %.cc,build/%.o,$(SRC2))

OBJ = $(patsubst %.cc,build/%.o,$(SRC))

.PHONY:clean build proto pb st_all

all: st_all pb $(TARGET)
	@echo "** make all **"

$(TARGET):$(OBJ)
	@mkdir -p $(@D)
	$(CXX) $^ -o $@ $(LDFLAGS)

build/src/%.o:./src/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $^ -o $@


# =============================
# 	subtask lib
# =============================

#ST_OBJS = $(ST_OBJ) $(ST_OBJ_C)

st_all: st_header $(ST_LIB)

$(ST_LIB): $(ST_OBJ)
	@mkdir -p $(@D)
	$(AR) qc $@ $^
	#$(AR) crv $@ $^

build/SubTask/src/%.o:./SubTask/src/%.cc
	@mkdir -p $(@D)
	$(CXX) $(ST_CXXFLAGS) -c $< -o $@

build/SubTask/src/%.o:./SubTask/src/%.c
	@mkdir -p $(@D)
	$(CC) $(ST_CFLAGS) -c $< -o $@

st_header: 
	@mkdir -p _include
	@mkdir -p _include/workflow
	@for h in $(SUBTASK_HDR); \
	do \
		ln -fs $(ROOT)/$$h $(ROOT)/_include/workflow/$$(basename $$h); \
	done


# =============================
# 	protobuf gen
# =============================

pb:
	@mkdir -p $(@D)
	$(PROTOC) -I=./src/proto --cpp_out=./src/proto src/proto/*.proto

clean:
	@echo "** make clean **"
	rm -rf $(TARGET) $(OBJ) $(ST_OBJ) $(ST_LIB) build output _include _lib

install:
	@echo "** make install **"

	mkdir -p $(DESTDIR)
	mkdir -p $(DESTDIR)/bin
	
	$(if $(DESTDIR),mv $(TARGET) $(DESTDIR)/bin/,)

check:
	@echo "** make check **"
