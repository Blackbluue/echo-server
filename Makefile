#
# **************************************************************
# *                Simple C++ Makefile Template                *
# *                                                            *
# * Author: Arash Partow (2003)                                *
# * URL: http://www.partow.net/programming/makefile/index.html *
# *                                                            *
# * Copyright notice:                                          *
# * Free use of this C++ Makefile template is permitted under  *
# * the guidelines and in accordance with the the MIT License  *
# * http://www.opensource.org/licenses/MIT                     *
# *                                                            *
# **************************************************************
#
SHELL := bash
.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

ifeq ($(origin .RECIPEPREFIX), undefined)
  $(error This Make does not support .RECIPEPREFIX. Please use GNU Make 4.0 or later)
endif
.RECIPEPREFIX = >

CC           := cc
CFLAGS       := -std=c11 -Wall -Wextra -Wpedantic
LDFLAGS      := -L/usr/lib -lm
LDLIBS       += -lscrypt
BUILD        := build
OBJ_DIR      := $(BUILD)/objects
APP_DIR      := $(BUILD)/apps
TARGET1      := server
TARGETS      := $(TARGET1)
INCLUDE      := -Iinclude/
SRC_DIR      := src
SRC          := $(wildcard $(SRC_DIR)/common/*.c)
OBJECTS      := $(SRC:%.c=$(OBJ_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

all: build $(TARGETS)


$(OBJ_DIR)/%.o: %.c
> @mkdir -p $(@D)
> $(CC) $(CFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(TARGET1): $(OBJ_DIR)/$(SRC_DIR)/$(TARGET1).o $(OBJECTS)
> @mkdir -p $(@D)
> $(CC) $(CFLAGS) -o $(APP_DIR)/$(TARGET1) $^ $(LDFLAGS) $(LDLIBS)

-include $(DEPENDENCIES)

.PHONY: all build clean debug release info

build:
> @mkdir -p $(APP_DIR)

debug: CFLAGS += -DDEBUG -g
debug: all

release: CFLAGS += -O2
release: all

clean:
> -@rm -rvf $(OBJ_DIR)/*
> -@rm -rvf $(APP_DIR)/*

info:
> @echo "[*] Application dir: ${APP_DIR}     "
> @echo "[*] Object dir:      ${OBJ_DIR}     "
> @echo "[*] Sources:         ${SRC}         "
> @echo "[*] Objects:         ${OBJECTS}     "
> @echo "[*] Dependencies:    ${DEPENDENCIES}"
