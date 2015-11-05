#####################################
# Copyright (c) 2015
# # All rights reserved.
# ######################################
DESTINATION := next-muduo-client # The name of example
SRCDIRS :=
LIBS :=
INCLUDES := .

RM := rm -f
#C,CC or CPP
PS=cc
# GNU Make
CC=g++
CPPFLAGS = -g -Wall -O3 -march=x86-64
CPPFLAGS += $(addprefix -I,$(INCLUDES))
CPPFLAGS += -MMD
#CXXFLAGS = $(CPPFLAGS)
CXXFLAGS += -std=c++11 -Wall

#SOURCE
ifeq ($(SRCDIRS),)
	SRCDIRS = .
endif
SOURCE := $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(PS))))
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
