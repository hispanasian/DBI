#include "PrintVisitorTest.h"


TEST_F(PrintVisitorTest, VisitSelectFileNode1) {
	stringstream ss;
	PrintVisitor pv;
	vector<AttTypePair> atts;
	atts.push_back(AttTypePair("x", INT));
	Schema schema = Schema(atts);
	char *cnf = "(x = 0)";
    yy_scan_string(cnf);
    yyparse();
	SelectFileNode node = SelectFileNode(0, schema, final, "file.bin");
	PrintVisitorData data {ss};	
	pv.VisitSelectFileNode(&node, (void*) &data);
}