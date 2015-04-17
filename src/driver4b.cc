
#include <iostream>
#include "ParseTree.h"

using namespace std;

extern "C" {
	int yysqlparse(void);   // defined in y.tab.c
}

int main(int argc, char *argv[]) {

	yysqlparse();
}


