# Makefile for TBX Library

include FixDeps:Rules/make

CXX=g++
CXXFLAGS=-O2 -Wall -mthrowback
AR=ar

TARGET=libtbx.a

CCSRC = $(wildcard tbx/*.cc)

all: $(TARGET)

$(TARGET): $(CCSRC:.cc=.o)
	$(AR) -r $(TARGET) $(CCSRC:.cc=.o)

clean:
	rm -f $(CCSRC:.cc=.o)

doc:
	doxygen tbx.doxyfile

include $(CCSRC:.cc=.d)
