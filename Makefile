CC = g++ -O2 -Wno-deprecated
GTEST_DIR = lib/gtest-1.7.0
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
$(GTEST_DIR)/include/gtest/internal/*.h
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread



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

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
		$(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
		$(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

main: build/Record.o build/Comparison.o build/ComparisonEngine.o build/Schema.o build/File.o build/y.tab.o build/lex.yy.o build/main.o
	$(CC) -o bin/main build/Record.o build/Comparison.o build/ComparisonEngine.o build/Schema.o build/File.o build/y.tab.o build/lex.yy.o build/main.o $(lfl)

bin/test.out: build/Record.o build/Comparison.o build/ComparisonEngine.o build/Schema.o build/File.o build/DBFile.o build/y.tab.o build/lex.yy.o buid/test.o
	$(CC) -o bin/test.out build/Record.o build/Comparison.o buld/ComparisonEngine.o build/Schema.o build/File.o build/DBFile.o build/y.tab.o build/lex.yy.o build/test.o $(lfl)

utest: build/utest.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o bin/$@

build/utest.o: src/utest.cc $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -o build/utest.o -c src/utest.cc

build/test.o: src/test.cc
	$(CC) -g -c -I include -o build/test.o src/test.cc

build/main.o: src/main.cc
	$(CC) -g -c -I include -o build/main.o src/main.cc

build/Comparison.o: src/Comparison.cc
	$(CC) -g -c -I include -o build/Comparison.o src/Comparison.cc

build/ComparisonEngine.o: src/ComparisonEngine.cc
	$(CC) -g -c -I include -o build/ComparisonEngine.o src/ComparisonEngine.cc

build/DBFile.o: src/DBFile.cc
	$(CC) -g -c -I include -o build/DBFile.o src/DBFile.cc

build/File.o: src/File.cc
	$(CC) -g -c -I include -o build/File.o src/File.cc

build/Record.o: src/Record.cc
	$(CC) -g -c -I include -o build/Record.o src/Record.cc

build/Schema.o: src/Schema.cc
	$(CC) -g -c -I include -o build/Schema.o src/Schema.cc

build/y.tab.o: src/Parser.y
	(cd src;yacc -d Parser.y)
	$(sed)
	(mv src/y.tab.h include)
	g++ -c -I include -o build/y.tab.o src/y.tab.c

build/lex.yy.o: src/Lexer.l
	(cd src;lex  Lexer.l)
	gcc  -c -I include -o build/lex.yy.o src/lex.yy.c

clean:
	rm -f *.o
	rm -f build/*
	rm -f bin/*
	rm -f *.out
	rm -f src/y.tab.c
	rm -f src/lex.yy.c
	rm -f src/y.tab.h
	rm -f include/y.tab.h
