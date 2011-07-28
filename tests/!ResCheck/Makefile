# Makefile for ResCheck test program

CXX=g++
CXXFLAGS=-O2 -ITBX: -mthrowback

LDFLAGS=-LTBX: -ltbx -static

TARGET=rescheck
TARGETELF=reschecke1f

OBJS=rescheck.o

all: $(TARGET)

$(TARGET):	$(TARGETELF)
	elf2aif $(TARGETELF) $(TARGET)

$(TARGETELF):	$(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(TARGETELF)

clean:
	rm -f $(OBJS) $(TARGETELF) $(TARGET)
