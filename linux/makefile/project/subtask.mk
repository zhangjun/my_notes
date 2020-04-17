
ST_CXXFLAGS = -Wall -O -std=c++1z
ST_FLAGS = -Wall -O -std=c++1z

ST_CXXFLAGS += -I _include/workflow
ST_CFLAGS += -I _include/workflow

LIB_TARGET = _lib/libworkflow.a

SUBTASK_DIR = SubTask/src

SUBTASK_SRC = $(wildcard $(SUBTASK_DIR)/kernel/*.cc \
			$(SUBTASK_DIR)/server/*.cc \
			$(SUBTASK_DIR)/factory/*.cc \
			$(SUBTASK_DIR)/protocol/*.cc \
			$(SUBTASK_DIR)/manager/*.cc \
			$(SUBTASK_DIR)/facility/*.cc \
			$(SUBTASK_DIR)/algorithm/*.cc \
			$(SUBTASK_DIR)/util/*.cc)

SUBTASK_SRC_C = $(wildcard $(SUBTASK_DIR)/kernel/*.c \
			$(SUBTASK_DIR)/protocol/*.c)

SUBTASK_HDR = $(wildcard $(SUBTASK_DIR)/kernel/*.h \
			$(SUBTASK_DIR)/server/*.h \
			$(SUBTASK_DIR)/factory/*.h \
			$(SUBTASK_DIR)/factory/*.inl \
			$(SUBTASK_DIR)/protocol/*.h \
			$(SUBTASK_DIR)/protocol/*.inl \
			$(SUBTASK_DIR)/manager/*.h \
			$(SUBTASK_DIR)/facility/*.h \
			$(SUBTASK_DIR)/facility/*.inl \
			$(SUBTASK_DIR)/algorithm/*.h \
			$(SUBTASK_DIR)/util/*.h)

#ST_HDR = $(addprefix _inc/, $(notdir $(SUBTASK_HDR)))
ST_HDR = $(notdir $(SUBTASK_HDR))

#SRC2 = $(notdir $(SRC))
# SRC_CXX = $(filter-out %.c,$(SOURCES))

IO_FILTER = $(SUBTASK_DIR)/kernel/IOService_posix.cc $(SUBTASK_DIR)/kernel/IOService_thread.cc

ST_OBJ =   $(patsubst %.cc,build/%.o,$(filter-out $(IO_FILTER), $(SUBTASK_SRC))) $(patsubst %.c,build/%.o,$(SUBTASK_SRC_C))
ST_OBJ_C = $(patsubst %.c,build/%.o,$(SUBTASK_SRC_C))
