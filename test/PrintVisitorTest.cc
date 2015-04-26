#include "PrintVisitorTest.h"


// SelectFileNode with a CNF
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
	node.Visit(pv, (void*) &data);	
	cout << ss.str();
}

// SelectFileNode without a CNF
TEST_F(PrintVisitorTest, VisitSelectFileNode2) {
	stringstream ss;
	PrintVisitor pv;
	vector<AttTypePair> atts;
	atts.push_back(AttTypePair("x", INT));
	Schema schema = Schema(atts);
	SelectFileNode node = SelectFileNode(0, schema, NULL, "file.bin");
	PrintVisitorData data {ss};	
	node.Visit(pv, (void*) &data);	
	cout << ss.str();
}

TEST_F(PrintVisitorTest, VisitSelectPipeNode1) {
	stringstream ss;
	PrintVisitor pv;
	vector<AttTypePair> atts;
	atts.push_back(AttTypePair("x", INT));
	atts.push_back(AttTypePair("y", DOUBLE));
	Schema schema = Schema(atts);
	char *cnf = "(x = 0)";
    yy_scan_string(cnf);
    yyparse();
	SelectFileNode child = SelectFileNode(0, schema, final, "file.bin");
	SelectPipeNode node = SelectPipeNode(1, &child, final);
	PrintVisitorData data {ss};	
	node.Visit(pv, (void*) &data);		
	cout << ss.str();
}

TEST_F(PrintVisitorTest, VisitProjectNode1) {
	stringstream ss;
	PrintVisitor pv;
	vector<AttTypePair> atts;
	atts.push_back(AttTypePair("x", INT));
	atts.push_back(AttTypePair("y", DOUBLE));
	Schema schema = Schema(atts);
	char *cnf = "(x = 0)";
    yy_scan_string(cnf);
    yyparse();
	SelectFileNode child = SelectFileNode(0, schema, final, "file.bin");
	SelectPipeNode node = SelectPipeNode(1, &child, final);
	PrintVisitorData data {ss};	
	node.Visit(pv, (void*) &data);		
	cout << ss.str();
}