CXX = $(shell root-config --cxx)
LD = $(shell root-config --ld)

PWD = $(shell pwd)
TOOLS = $(PWD)/tools

CPPFLAGS := $(shell root-config --cflags) -I$(PWD)/include -I$(TOOLS)/include
LDFLAGS := $(shell root-config --glibs) $(STDLIBDIR) -lRooFit -lRooFitCore -L$(PWD)/include

CPPFLAGS += -g -std=c++1y

TARGET = getLimits
TARGET2 = lowMass_prompt
TARGET3 = create_reduced_tree

SRC = app/getLimits.cc src/fitDarkphoton.cc src/pdfs.cc $(TOOLS)/src/CommandLineInput.cc
SRC2 = app/lowMass_prompt.cc src/fitDarkphoton.cc src/pdfs.cc $(TOOLS)/src/CommandLineInput.cc
SRC3 = app/create_reduced_tree.cc src/tree.cc $(TOOLS)/src/CommandLineInput.cc $(TOOLS)/src/Filename.cc

OBJ = $(SRC:.cc=.o)
OBJ2 = $(SRC2:.cc=.o)
OBJ3 = $(SRC3:.cc=.o)

.PHONY: all clean

all : $(TARGET) $(TARGET2) $(TARGET3)

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

$(TARGET3) : $(OBJ3)
	$(LD) $(CPPFLAGS) -o $(TARGET3) $(OBJ3) $(LDFLAGS)
	@echo $@
	@echo $<
	@echo $^

%.o : %.cc
	$(CXX) $(CPPFLAGS) -o $@ -c $<
	@echo $@
	@echo $<

clean :
	rm -f *.o app/*.o src/*.o $(TOOLS)/src/*.o $(TARGET) $(TARGET2) $(TARGET3) *~
