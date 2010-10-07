# Makefile for ResView test program

CXX=g++
CXXFLAGS=-O2 -ITBX:

LDFLAGS=-LTBX: -ltbx -static

TARGET=!RunImage
TARGETELF=resview

OBJS=resview.o

all: $(TARGET)

$(TARGET):	$(TARGETELF)
	elf2aif $(TARGETELF) $(TARGET)

$(TARGETELF):	$(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(TARGETELF)

clean:
	rm -f $(OBJS) $(TARGETELF) $(TARGET)
