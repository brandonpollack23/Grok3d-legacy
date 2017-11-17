#program types and flags
CC = g++
INCLUDEDIRS = ./inc/ ../glm/ ../notstd/
CFLAGS = -std=c++17 -g -O2 -Wall -fno-exceptions $(addprefix -I, $(INCLUDEDIRS))

LD = $(CC)
LDFLAGS = 

AR = ar
ARFLAGS = 

#MAKE specific stuff
#VPATH is the search path for all prerequisites
#searches for all cpp files, uses sed to get just the path, and uses sort to trim to unique
VPATHCPP = $(shell find . -name "*.cpp" | sed 's/\(.*\)\/.*\..*$$/\1/g' | sort -u)
VPATHOBJ = $(patsubst ./src%, ./$(OBJDIR)%, $(VPATHCPP))
VPATH = $(VPATHCPP) $(VPATHOBJ)

#other defines
TARGET = maintest
BINDIR = bin
OBJDIR = o
SOURCES = $(shell find . -name "*.cpp")
OBJECTS = $(patsubst ./src/%, ./$(OBJDIR)/%, $(patsubst %.cpp, %.o, $(SOURCES)))

#dependency management
DEPDIR := .d
DEPDIRS = $(patsubst ./src%, ./.d%, $(VPATHCPP))
$(shell mkdir -p $(DEPDIRS) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

.PHONY: all clean

all: $(TARGET)

rebuild: clean all

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(LD) -o $(BINDIR)/$@ $^ $(LDFLAGS)

$(BINDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.cpp $(DEPDIR)/%.d | $(VPATHOBJ)
	$(CC) $(DEPFLAGS) -c $(CFLAGS) $< -o $@
	$(POSTCOMPILE)

$(VPATHOBJ):
	mkdir -p $@

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

clean:
	rm -rf $(OBJDIR) $(BINDIR)

print-%: 
	@echo $($*)

INCLUDES = $(patsubst ./src%, $(DEPDIR)%.d, $(basename $(SOURCES)))
include $(INCLUDES)
