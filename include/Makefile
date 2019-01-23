CXX = $(shell root-config --cxx)
LD = $(shell root-config --ld)

INC = $(shell pwd)
Repo = $(shell git rev-parse --show-toplevel)

CPPFLAGS := $(shell root-config --cflags) -I$(INC)/include 
LDFLAGS := $(shell root-config --glibs) $(STDLIBDIR) -O -lRooFit -lRooFitCore -lMinuit -lHtml -lPyROOT -lFoam -lRooStats -lTreePlayer

CPPFLAGS += -g -std=c++1y -fPIC

SRC = ../src/CustomPdfs.cc
SHARED = libCustomPdfs.so
INCLUDELIST = CustomPdfs.hh Linkdef.h

all: rootdict.cc rootdict.o CustomPdfs.o $(SHARED)


rootdict.cc: 
	$(ROOTSYS)/bin/rootcint -f $@ -c $(CINTINCLUDES) -I$(INC) $(INCLUDELIST) 

rootdict.o:
	$(CXX) rootdict.cc $(CPPFLAGS) $(LDFLAGS) -o $@ -I$(INC) -c

CustomPdfs.o:
	$(CXX) $(SRC) $(CPPFLAGS) $(LDFLAGS) -o $@ -I$(INC) -c

$(SHARED): rootdict.o CustomPdfs.o
	$(CXX) -shared  -I$(ROOTSYS)/include/ $(LDFLAGS) -o $@ $^

clean :
	rm -f *.o rootdict.cc rootdict_rdict.pcm
