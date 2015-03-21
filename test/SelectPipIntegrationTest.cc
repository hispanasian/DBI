#include <gtest/gtest.h>
#include "Schema.h"
#include "Record.h"
#include "Pipe.h"
#include "SelectPipe.h"

extern "C" {
	int yyparse(void);   // defined in y.tab.c
	int yyfuncparse(void);   // defined in yyfunc.tab.c
	void init_lexical_parser (char *); // defined in lex.yy.c (from Lexer.l)
	void close_lexical_parser (); // defined in lex.yy.c
	void init_lexical_parser_func (char *); // defined in lex.yyfunc.c (from Lexerfunc.l)
	void close_lexical_parser_func (); // defined in lex.yyfunc.c
}

extern struct AndList *final;

void SetupCNF(char *input, Schema *left, CNF &cnf_pred, Record &literal) {
	init_lexical_parser (input);
  	if (yyparse() != 0) {
		cout << " Error: can't parse your CNF " << input << endl;
		exit (1);
	}
	cnf_pred.GrowFromParseTree (final, left, literal); // constructs CNF predicate
	close_lexical_parser ();
}

// Simple test, 25 records expected
TEST(SelectPipeIntegrationTest, Test1) {
	FILE *tableFile = fopen ("data/10M/lineitem.tbl", "r");
	Record temp;
	Schema schema ("data/catalog", "lineitem");
	Record lit;
	CNF cnf;

	// CAREFUL
	// If you change this query, you might cause a dealock
	// because the output pipe will fill up before the input pipe
	// can be emptied. You'll need another thread.
	char* predicate = "(l_orderkey < 10)";
	SetupCNF (predicate, &schema, cnf, lit);
	Pipe in(100);
	Pipe out(100);

	SelectPipe op;
	op.Run(in, out, cnf, lit);

	// fill the input pipe
	while (temp.SuckNextRecord (&schema, tableFile) == 1) {
		in.Insert(&temp);
    }
    in.ShutDown();

    // count the number of records in the output
    int count = 0;
    while(out.Remove(&temp)) {
    	++count;
    }

	fclose(tableFile);

	EXPECT_EQ(25, count);
}
