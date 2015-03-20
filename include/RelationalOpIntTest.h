#ifndef RELATIONALOPINTTES_H
#define RELATIONALOPINTTES_H

#include <gtest/gtest.h>
#include "Schema.h"
#include "Record.h"
#include "Pipe.h"

extern "C" {
	int yyparse(void);   // defined in y.tab.c
	int yyfuncparse(void);   // defined in yyfunc.tab.c
	void init_lexical_parser (char *); // defined in lex.yy.c (from Lexer.l)
	void close_lexical_parser (); // defined in lex.yy.c
	void init_lexical_parser_func (char *); // defined in lex.yyfunc.c (from Lexerfunc.l)
	void close_lexical_parser_func (); // defined in lex.yyfunc.c
}

extern struct AndList *final;

class RelationalOpIntTest: public ::testing::Test {
public:
	void SetupCNF(char *input, Schema *left, CNF &cnf_pred, Record &literal) {
		init_lexical_parser (input);
		if (yyparse() != 0) {
			cout << " Error: can't parse your CNF " << input << endl;
			exit (1);
		}
		cnf_pred.GrowFromParseTree (final, left, literal); // constructs CNF predicate
		close_lexical_parser ();
	}
};

#endif /* RELATIONALOPINTTES_H */
