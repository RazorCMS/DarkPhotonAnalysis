CXX = $(shell root-config --cxx)
LD = $(shell root-config --ld)

PWD = $(shell pwd)
TOOLS = $(PWD)/tools

CPPFLAGS := $(shell root-config --cflags) -I$(PWD)/include -I$(TOOLS)/include
LDFLAGS := $(shell root-config --glibs) $(STDLIBDIR) -lRooFit -lRooFitCore -L$(PWD)/include

CPPFLAGS += -g -std=c++1y

TARGET = getLimits
TARGET2 = lowMass_prompt
TARGET4 = create_reduced_tree
TARGET5 = create_subTree
TARGET6 = checkFits
TARGET7 = binnedFits

SRC = app/getLimits.cc src/fitDarkphoton.cc src/pdfs.cc $(TOOLS)/src/CommandLineInput.cc
SRC2 = app/lowMass_prompt.cc src/fitDarkphoton.cc src/pdfs.cc $(TOOLS)/src/CommandLineInput.cc src/dataTree.cc
SRC4 = app/create_reduced_tree.cc src/tree.cc $(TOOLS)/src/CommandLineInput.cc
SRC5 = app/create_subTree.cc src/subTree.cc 
SRC6 = app/checkFits.cc src/pdfs.cc
SRC7 = app/binnedFits.cc src/pdfs.cc

OBJ = $(SRC:.cc=.o)
OBJ2 = $(SRC2:.cc=.o)
OBJ4 = $(SRC4:.cc=.o)
OBJ5 = $(SRC5:.cc=.o)
OBJ6 = $(SRC6:.cc=.o)
OBJ7 = $(SRC7:.cc=.o)

.PHONY: all clean

all : $(TARGET) $(TARGET2) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7)

$(TARGET) : $(OBJ)
	$(LD) $(CPPFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)
	@echo $@
	@echo $<
	@echo $^

$(TARGET2) : $(OBJ2)
	$(LD) $(CPPFLAGS) -o $(TARGET2) $(OBJ2) $(LDFLAGS)
	@echo $@
	@echo $<
	@echo $^

$(TARGET4) : $(OBJ4)
	$(LD) $(CPPFLAGS) -o $(TARGET4) $(OBJ4) $(LDFLAGS)
	@echo $@
	@echo $<
	@echo $^

$(TARGET5) : $(OBJ5) 
	$(LD) $(CPPFLAGS) -o $(TARGET5) $(OBJ5) $(LDFLAGS)
	@echo $@
	@echo $<
	@echo $^

$(TARGET6) : $(OBJ6) 
	$(LD) $(CPPFLAGS) -o $(TARGET6) $(OBJ6) $(LDFLAGS)
	@echo $@
	@echo $<
	@echo $^

$(TARGET7) : $(OBJ7) 
	$(LD) $(CPPFLAGS) -o $(TARGET7) $(OBJ7) $(LDFLAGS)
	@echo $@
	@echo $<
	@echo $^

%.o : %.cc
	$(CXX) $(CPPFLAGS) -o $@ -c $<
	@echo $@
	@echo $<

clean :
	rm -f *.o app/*.o src/*.o $(TOOLS)/src/*.o $(TARGET) $(TARGET2) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) *~
