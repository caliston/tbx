# Makefile for ResTest test program

CXX=g++
CXXFLAGS=-O2 -ITBX:

LDFLAGS=-LTBX: -ltbx -static

TARGET=!RunImage
TARGETELF=restest

OBJS=restest.o

all: $(TARGET)

$(TARGET):	$(TARGETELF)
	elf2aif $(TARGETELF) $(TARGET)

$(TARGETELF):	$(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(TARGETELF)

clean:
	rm -f $(OBJS) $(TARGETELF) $(TARGET)
