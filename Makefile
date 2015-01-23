CC = g++ -O2 -Wno-deprecated

# Vars
SRCDIR := src
BUILDDIR := build
LIBDIR := lib
INCLUDEDIR := include
TARGETDIR := bin

SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
ALL_OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g # -Wall
LIB := -L lib
INC := -I include
PARSING := $(BUILDDIR)/y.tab.o $(BUILDDIR)/lex.yy.o
MAINS := $(BUILDDIR)/main.o $(BUILDDIR)/test.o

# Objects excluding main
OBJECTS := $(filter-out $(MAINS),$(ALL_OBJECTS)) $(PARSING)

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CPPFLAGS += -isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include

# Bison/Flex stuff
tag = -i

ifdef linux
tag = -n
flex_lib = -lfl
sed = sed -n src/y.tab.c -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/“
endif

# Check if on OS X
ifeq "$(OSTYPE)" "darwin"
tag = -n
endif

###### Main Build ######
# Build main
main: $(OBJECTS) $(BUILDDIR)/main.o
	$(CC) -o $(TARGETDIR)/main $^ $(lfl)

# build/test.o: src/test.cc
	# 	$(CC) -g -c -I include -o build/test.o src/test.cc

# Compile cc files
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

# Compile Flex/Bison files
$(BUILDDIR)/y.tab.o: $(SRCDIR)/Parser.y
	(cd $(SRCDIR);yacc -d Parser.y)
	$(sed)
	(mv $(SRCDIR)/y.tab.h $(INCLUDEDIR))
	g++ $(CFLAGS) $(INC) -c -o $@ $(SRCDIR)/y.tab.c

$(BUILDDIR)/lex.yy.o: $(SRCDIR)/Lexer.l
	(cd $(SRCDIR);lex  Lexer.l)
	gcc $(CFLAGS) $(INC) -c -o $@ $(SRCDIR)/lex.yy.c


########## Testing ##########
# Test Vars
TESTDIR := test

# GoogleTest Vars
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
GTEST_DIR = lib/gtest-1.7.0
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
$(GTEST_DIR)/include/gtest/internal/*.h
GMOCK_DIR = lib/gmock-1.7.0
GMOCK_HEADERS = $(GMOCK_DIR)/include/gmock/*.h \
$(GMOCK_DIR)/include/gmock/internal/*.h \
$(GTEST_HEADERS)
GMOCK_SRCS_ = $(GMOCK_DIR)/src/*.cc $(GMOCK_HEADERS)
GTESTLIBS := $(LIBDIR)/gtest_main.a $(LIBDIR)/gmock_main.a


###### Test Builds ######
utest: $(OBJECTS) $(TARGETDIR)/utest.o $(GTESTLIBS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o bin/$@

# Compile tests cc files
$(BUILDDIR)/%.o: $(TESTDIR)/%.$(SRCEXT) $(GTEST_HEADERS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) $(INC) -c -o $@ $<

# Create GTest and GMock libs
$(BUILDDIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
		-o $@ $(GTEST_DIR)/src/gtest-all.cc

$(BUILDDIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
		-o $@ $(GTEST_DIR)/src/gtest_main.cc

$(LIBDIR)/gtest.a : $(BUILDDIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(LIBDIR)/gtest_main.a : $(BUILDDIR)/gtest-all.o $(BUILDDIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

$(BUILDDIR)/gmock-all.o : $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
	-c -o $@ $(GMOCK_DIR)/src/gmock-all.cc

$(BUILDDIR)/gmock_main.o : $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
	-c -o $@ $(GMOCK_DIR)/src/gmock_main.cc

$(LIBDIR)/gmock.a : $(BUILDDIR)/gmock-all.o $(BUILDDIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(LIBDIR)/gmock_main.a : $(BUILDDIR)/gmock-all.o $(BUILDDIR)/gtest-all.o $(BUILDDIR)/gmock_main.o
	$(AR) $(ARFLAGS) $@ $^


###### Clean ######
clean:
	rm -f *.o
	rm -f build/*
	rm -f bin/*
	rm -f *.out
	rm -f src/y.tab.c
	rm -f src/lex.yy.c
	rm -f src/y.tab.h
	rm -f include/y.tab.h
