#####################################
# Copyright (c) 2015
# # All rights reserved.
# ######################################

# Make file for next-muduo examples.

# #Target Name, Source File Directories, Library(Such as stdcx,iostr,mysql), Head File Directories.
# The name of example
DESTINATION := echo-server
SRCDIRS := examples/echo_server next_muduo next_muduo/threads
LIBS := pthread
INCLUDES := .

RM := rm -f
# Suffix Of Source Files, such as c, cc and cpp, etc.
PS=cc
# GNU Make Variables
CC=g++
CPPFLAGS = -g -Wall -O3 -march=x86-64
CPPFLAGS += $(addprefix -I,$(INCLUDES))
CPPFLAGS += -MMD
#CXXFLAGS = $(CPPFLAGS)
CXXFLAGS += -std=c++11 -Wall

#SOURCE Files
ifeq ($(SRCDIRS),)
	SRCDIRS = .
endif
SOURCE := $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*.,$(PS))))
OBJS := $(patsubst %.$(PS),%.o,$(SOURCE))
DEPS := $(patsubst %.o,%.d,$(OBJS))
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))
MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.$(PS),$(MISSING_DEPS)))

.PHONY : all deps objs clean rebuild

all : $(DESTINATION)

deps : $(DEPS)
		$(CC) -MM -MMD $(SOURCE)

objs : $(OBJS)

clean :
		@$(RM) $(OBJS) $(DEPS) $(DESTINATION)

rebuild: clean all 

ifneq ($(MISSING_DEPS),)
$(MISSING_DEPS) :
		@$(RM) $(patsubst %.d,%.o,$@)
endif

-include $(DEPS)

$(DESTINATION) : $(OBJS)
		$(CC) -o $(DESTINATION) $(OBJS) $(addprefix -l,$(LIBS))
#End
