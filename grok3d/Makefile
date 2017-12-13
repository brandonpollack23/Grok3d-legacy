#program types and flags
CC = g++
#inc is grok3d includes
#../include is external includes like glad
#submodules: glm, notstd
INCLUDEDIRS = ./inc/ ../glm/ ../notstd/ ../include
#libdl is required by glad
CFLAGS = -std=c++17 -g -O2 -Wall -fno-exceptions $(GLFAGS) $(addprefix -I, $(INCLUDEDIRS))

AR = ar
ARFLAGS = rcs

#MAKE specific stuff
#VPATH is the search path for all prerequisites
#searches for all cpp files, uses sed to get just the path, and uses sort to trim to unique
VPATHCPP = $(shell find . -name "*.cpp" | sed 's/\(.*\)\/.*\..*$$/\1/g' | sort -u)
VPATHOBJ = $(patsubst ./src%, ./$(OBJDIR)%, $(VPATHCPP))
VPATH = $(VPATHCPP) $(VPATHOBJ)

#other defines
TARGET = libgrok3d.a
BINDIR = bin
OBJDIR = o
SOURCES = $(shell find . -name "*.cpp" -o -name "*.c")
OBJECTS = $(patsubst ./src/%, ./$(OBJDIR)/%, $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SOURCES))))

#dependency management
DEPDIR := .d
DEPDIRS = $(patsubst ./src%, ./.d%, $(VPATHCPP))
$(shell mkdir -p $(DEPDIRS) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

.PHONY: all clean

all: $(TARGET)

rebuild: clean all

$(TARGET): $(OBJECTS)
	$(AR) $(ARFLAGS) $(OBJDIR)/$@ $^

$(OBJDIR)/%.o: %.cpp $(DEPDIR)/%.d | $(VPATHOBJ)
	$(CC) $(DEPFLAGS) -c $(CFLAGS) $< -o $@
	$(POSTCOMPILE)

$(VPATHOBJ):
	mkdir -p $@

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

clean:
	rm -rf $(OBJDIR)

print-%: 
	@echo $($*)

INCLUDES = $(patsubst ./src%, $(DEPDIR)%.d, $(basename $(SOURCES)))
include $(INCLUDES)
