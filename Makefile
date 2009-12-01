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

copytoapp: all doc
	mkdir -p !TBX/tbx/h
	copy tbx.h.* !TBX.tbx.h.* ~CF
	copy libtbx/a !TBX.libtbx/a ~CF
	mkdir -p !TBX/html
	copy docs.html.* !TBX.html.* ~CF

cleanapp:
	rm -rf !TBX/tbx
	rm -rf !TBX/html
	IfThere !TBX.libtbx/a Then Delete !TBX.libtbx/a

include $(CCSRC:.cc=.d)
