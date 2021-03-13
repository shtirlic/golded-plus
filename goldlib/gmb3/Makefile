# -*- makefile -*-

TOP=../..
TARGET=gmb3
INCS=-I$(TOP)/goldlib -I$(TOP)/goldlib/gall -I$(TOP)/goldlib/gcui -I$(TOP)/goldlib/gcfg -I$(TOP)/goldlib/gmb3 -I$(TOP)/goldlib/smblib
ifdef MINGW
INCS+=-I$(TOP)/goldlib/glibc
else
ifeq ($(PLATFORM),emx)
INCS+=-I$(TOP)/goldlib/glibc
endif
endif

include $(TOP)/GNUmakef.inc
include $(TOP)/GNUmakef.lib
