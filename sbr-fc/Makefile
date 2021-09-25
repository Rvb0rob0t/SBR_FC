# A reasonably-general Makefile for a cpp project
# Useful links:
#    https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
#    https://www.gnu.org/software/make/manual/html_node/Implicit-Rules.html#Implicit-Rules
#    http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
#
# Emilio Domínguez Sánchez - emilio.dominguezs@um.es
# April 2020


# Do not use make's built-in rules and variables
# (this increases performance and avoids hard-to-debug behaviour);
MAKEFLAGS += -rR
### Define according to folder structure ###
INCD=include#	headers folder
SRCD=src#		source folder/s and file extension
BUILD=build#	build files folder (objects, dependencies, etc)
BIND=bin#		binaries folder
# define the main files
TARGET=$(BIND)/sbr

#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
# define the C source files
SRCS=$(shell find $(SRCD) -type f -name "*.cpp")
# define the C object files 
OBJS=$(patsubst $(SRCD)/%,$(BUILD)/%,$(SRCS:.cpp=.o))
# define the auto-generated dependency files
DEPS=$(patsubst $(SRCD)/%,$(BUILD)/%,$(SRCS:.cpp=.d))

### Define compiling process ###
CXX=g++# C compiler
INC=-I $(INCD)
CXXFLAGS=-g -Wall -Werror -Wno-unused -std=c++17# Compile-time flags
LDLIBS=
	

$(TARGET): $(OBJS) | $(BIND)
	$(CXX) $(CXXFLAGS) $(INC) $(LDLIBS) $^ -o $(TARGET)

# rule to generate a dep file by using the C preprocessor
# (see man cpp for details on the -MM and -MT options)
$(BUILD)/%.d: $(SRCD)/%.cpp | $(BUILD)
	($(CXX) $(CXXFLAGS) $(INC) $(LDLIBS) $< -MM -MT $(@:.d=.o) && printf '\t$(CXX) $(CXXFLAGS) $(INC) $(LDLIBS) -c $< -o $(@:.d=.o)';) >$@

# Files without auto detection of dependencies.
$(BUILD):
	mkdir $@

$(BIND):
	mkdir $@


# Include dependencies files
-include $(DEPS)

.PHONY: clean cleandep run

clean:
	rm -f $(TARGET) $(OBJS)

cleandep:
	rm -f $(DEPS)

run: $(TARGET)
	./$(TARGET)