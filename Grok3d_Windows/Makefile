#program types and flags
CC = clang
INCLUDEDIRS = ./inc/ ../glm/ ../notstd/
CFLAGS = -std=c++17 -g -Wall -O2 $(addprefix -I, $(INCLUDEDIRS))

LD = $(CC)
LDFLAGS = 

AR = ar
ARFLAGS = 

#MAKE specific stuff
#VPATH is the search path for all prerequisites
#searches for all cpp files, uses sed to get just the path, and uses sort to trim to unique
VPATH = $(shell find . -name "*.cpp" | sed 's/\(.*\)\/.*\..*$$/\1/g' | sort -u)

#other defines
TARGET = maintest
BINDIR = bin
OBJDIR = o
SOURCES = $(shell find . -name "*.cpp")
OBJECTS = $(patsubst ./src/%, ./$(OBJDIR)/%, $(patsubst %.cpp, %.o, $(SOURCES)))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(LD) -o $(BINDIR)/$@ $(OBJDIR)/$^ $(LDFLAGS)

$(BINDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

print-%: 
	@echo $*=$($*)
