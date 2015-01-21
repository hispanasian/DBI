CC = g++ -O2 -Wno-deprecated 
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
LIBDIR := lib
INCLUDEDIR := include
TARGET := bin/main

SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g # -Wall
INC := -I include

tag = -i

ifdef linux
tag = -n
endif

PARSING := $(BUILDDIR)/y.tab.o $(BUILDDIR)/lex.yy.o
MAINS := $(BUILDDIR)/main.o $(BUILDDIR)/test.o

main: $(filter-out $(MAINS),$(OBJECTS)) $(PARSING) $(BUILDDIR)/main.o
	@echo " Linking..."
	$(CC) -o $(TARGET) $(LIB) $^ -lfl 

#$(TARGET): $(filter-out $(BUILDDIR)/test.o,$(OBJECTS))
#	@echo " Linking..."
#	@echo " $(CC) $^ -o $(TARGET)"; $(CC) $^ -o $(BUILDDIR)/$(TARGET)

#$(TARGET): $(OBJECTS)
#	@echo " Linking..."
#	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

# compile cc files
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

# Compile Bison and Flex files
$(BUILDDIR)/y.tab.o: $(SRCDIR)/Parser.y
	(cd $(SRCDIR);yacc -d Parser.y)
	mv $(SRCDIR)/y.tab.h $(INCLUDEDIR)
	sed $(tag) $(SRCDIR)/y.tab.c -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/"
	$(CC) $(INC) -c -o $@ $(SRCDIR)/y.tab.c
	
$(BUILDDIR)/lex.yy.o: $(SRCDIR)/Lexer.l
	(cd $(SRCDIR);lex Lexer.l)
	$(CC) $(INC) -c -o $@ $(SRCDIR)/lex.yy.c

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)
	rm -f $(SRCDIR)/y.tab.c
	rm -f $(SRCDIR)/lex.yy.c
	rm -f $(INCLUDEDIR)/y.tab.h

# Tests
tester:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

# Spikes
ticket:
	$(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean
