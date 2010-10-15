# Makefile for TBX Library

include FixDeps:Rules/make

CXX=g++
CXXFLAGS=-O2 -Wall -mthrowback
AR=ar

TARGET=libtbx.a

CCSRC = $(wildcard tbx/*.cc) $(wildcard tbx/view/*.cc) $(wildcard tbx/res/*.cc) $(wildcard tbx/doc/*.cc)


bin:	$(TARGET)

doc:
	doxygen tbx.doxyfile

all:	bin doc

$(TARGET): $(CCSRC:.cc=.o)
	$(AR) -r $(TARGET) $(CCSRC:.cc=.o)

clean:
	rm -f $(CCSRC:.cc=.o)


copytoapp: all
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
