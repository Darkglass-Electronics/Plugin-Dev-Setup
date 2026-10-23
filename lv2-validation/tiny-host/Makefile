#!/usr/bin/make -f
# Makefile for mod-cv-randoms.lv2 #
# --------------------------------- #

CXX ?= g++
PKG = lilv-0

BASE_FLAGS = -Wall -Wextra -pipe -Wno-unused-parameter
BASE_OPTS  = -O3 -ffast-math $(shell pkg-config --cflags $(PKG))
LIBS = $(shell pkg-config --libs $(PKG))

LINK_OPTS  = -Wl,-O1 -Wl,--as-needed -Wl,--strip-all
BASE_FLAGS += -fPIC -DPIC

ifeq ($(DEBUG),true)
BASE_FLAGS += -DDEBUG -O0 -g
LINK_OPTS   =
else
BASE_FLAGS += -DNDEBUG $(BASE_OPTS) -fvisibility=hidden
CXXFLAGS   += -fvisibility-inlines-hidden
endif

BUILD_CXX_FLAGS = $(BASE_FLAGS) -std=c++17 $(CXXFLAGS) $(CPPFLAGS)

LINK_FLAGS      = $(LINK_OPTS) -Wl,--no-undefined $(LDFLAGS)

NAME = tiny-host

all: $(NAME)

$(NAME): $(NAME).cpp
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) -lm $(SHARED) -o $@ $(LIBS)

clean:
	rm -f $(NAME)
