#include "PrintVisitorTest.h"


// SelectFileNode with a CNF
TEST_F(PrintVisitorTest, VisitSelectFileNode1) {
	stringstream ss;
	PrintVisitor pv;
	vector<AttTypePair> atts;
	atts.push_back(AttTypePair("x", INT));
	atts.push_back(AttTypePair("y", DOUBLE));
	atts.push_back(AttTypePair("z", STRING));
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
	atts.push_back(AttTypePair("y", DOUBLE));
	atts.push_back(AttTypePair("z", STRING));
	Schema schema = Schema(atts);
	SelectFileNode node = SelectFileNode(0, schema, NULL, "file.bin");
	PrintVisitorData data {ss};	
	node.Visit(pv, (void*) &data);	
	cout << ss.str();
}

// SelectFileNode with a more complicated CNF
TEST_F(PrintVisitorTest, VisitSelectFileNode3) {
	stringstream ss;
	PrintVisitor pv;
	vector<AttTypePair> atts;
	atts.push_back(AttTypePair("x", INT));
	atts.push_back(AttTypePair("y", DOUBLE));
	atts.push_back(AttTypePair("z", STRING));
	Schema schema = Schema(atts);
	char *cnf = "(x = 0) AND (y = 10.0) AND (z = 'bleh')";
    yy_scan_string(cnf);
    yyparse();
	SelectFileNode node = SelectFileNode(0, schema, final, "file.bin");
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
	atts.push_back(AttTypePair("z", STRING));
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
	atts.push_back(AttTypePair("z", STRING));
	vector<RelAttPair> attsToKeep;
	attsToKeep.push_back(RelAttPair("", "y"));
	Schema schema = Schema(atts);
	char *cnf = "(x = 0)";
    yy_scan_string(cnf);
    yyparse();
	SelectFileNode child = SelectFileNode(0, schema, final, "file.bin");
	ProjectNode node = ProjectNode(1, &child, attsToKeep);
	PrintVisitorData data {ss};	
	node.Visit(pv, (void*) &data);		
	cout << ss.str();
}

TEST_F(PrintVisitorTest, VisitJoinNode1) {
	stringstream ss;
	PrintVisitor pv;
	vector<AttTypePair> leftAtts;
	leftAtts.push_back(AttTypePair("x", INT));
	leftAtts.push_back(AttTypePair("y", DOUBLE));
	leftAtts.push_back(AttTypePair("z", STRING));
	Schema schemaLeft = Schema(leftAtts);
	char *cnf = "(x = 0)";
    yy_scan_string(cnf);
    yyparse();
    AndList* leftAnd = final;
	SelectFileNode leftChild = SelectFileNode(0, schemaLeft, leftAnd, "left.bin");

	vector<AttTypePair> rightAtts;
	rightAtts.push_back(AttTypePair("a", INT));
	rightAtts.push_back(AttTypePair("b", DOUBLE));
	rightAtts.push_back(AttTypePair("c", STRING));
	Schema schemaRight = Schema(rightAtts);
	cnf = "(a = 0)";
    yy_scan_string(cnf);
    yyparse();
    AndList* rightAnd = final;
	SelectFileNode rightChild = SelectFileNode(1, schemaRight, rightAnd, "file.bin");

	cnf = "(a = x)";
    yy_scan_string(cnf);
    yyparse();
    AndList* joinAnd = final;
	JoinNode node = JoinNode(2, &leftChild, 10, &rightChild, 100, joinAnd);	
	PrintVisitorData data {ss};	
	node.Visit(pv, (void*) &data);		
	cout << ss.str();
}

TEST_F(PrintVisitorTest, VisitDuplicateRemovalNode1) {
	stringstream ss;
	PrintVisitor pv;
	vector<AttTypePair> atts;
	atts.push_back(AttTypePair("x", INT));
	atts.push_back(AttTypePair("y", DOUBLE));
	atts.push_back(AttTypePair("z", STRING));
	vector<RelAttPair> dupAtts;
	dupAtts.push_back(RelAttPair("", "y"));
	Schema schema = Schema(atts);
	char *cnf = "(x = 0)";
    yy_scan_string(cnf);
    yyparse();
	SelectFileNode child = SelectFileNode(0, schema, final, "file.bin");
	DuplicateRemovalNode node = DuplicateRemovalNode(1, &child, dupAtts);
	PrintVisitorData data {ss};	
	node.Visit(pv, (void*) &data);		
	cout << ss.str();
}
//	SumNode(int id, OpNode *_child, const struct FuncOperator *_funcOp);

TEST_F(PrintVisitorTest, VisitSumNode1) {
	stringstream ss;
	PrintVisitor pv;
	vector<AttTypePair> atts;
	atts.push_back(AttTypePair("x", INT));
	atts.push_back(AttTypePair("y", DOUBLE));
	atts.push_back(AttTypePair("z", STRING));
	Schema schema = Schema(atts);
	schema.SetRelation("A");
	char *cnf = "(x = 0)";
    yy_scan_string(cnf);
    yyparse();
	SelectFileNode child = SelectFileNode(0, schema, final, "file.bin");

	string sql = "SELECT SUM( (A.x * A.y) )";
	sql.append(" FROM rel AS A");
	sql.append(" WHERE (A.x = 5)");	
	yysql_scan_string(sql.c_str());
	yysqlparse();
	SumNode node = SumNode(1, &child, finalFunction);

	PrintVisitorData data {ss};	
	node.Visit(pv, (void*) &data);		
	cout << ss.str();
}

// GroupByNode::GroupByNode(int id, OpNode *_child, const std::vector<RelAttPair> &_group,
// 		const struct FuncOperator *_funcOp): OpNode(id),funcOp(_funcOp), group(_group) {
TEST_F(PrintVisitorTest, VisitGroupByNode1) {
	stringstream ss;
	PrintVisitor pv;
	vector<AttTypePair> atts;
	atts.push_back(AttTypePair("x", INT));
	atts.push_back(AttTypePair("y", DOUBLE));
	atts.push_back(AttTypePair("z", STRING));
	Schema schema = Schema(atts);
	schema.SetRelation("A");
	char *cnf = "(x = 0)";
    yy_scan_string(cnf);
    yyparse();
	SelectFileNode child = SelectFileNode(0, schema, final, "file.bin");

	vector<RelAttPair> group;
	group.push_back(RelAttPair("A", "x"));

	string sql = "SELECT SUM( (A.x * A.y) )";
	sql.append(" FROM rel AS A");
	sql.append(" WHERE (A.x = 5)");	
	yysql_scan_string(sql.c_str());
	yysqlparse();
	GroupByNode node = GroupByNode(1, &child, group, finalFunction);

	PrintVisitorData data {ss};	
	node.Visit(pv, (void*) &data);		
	cout << ss.str();
}
	// WriteOutNode(int id, OpNode *_child, FILE *_outFile);
TEST_F(PrintVisitorTest, VisitWriteOutNode1) {
	stringstream ss;
	PrintVisitor pv;
	vector<AttTypePair> atts;
	atts.push_back(AttTypePair("x", INT));
	atts.push_back(AttTypePair("y", DOUBLE));
	atts.push_back(AttTypePair("z", STRING));
	Schema schema = Schema(atts);
	SelectFileNode child = SelectFileNode(0, schema, final, "file.bin");
	FILE* file;
	WriteOutNode node = WriteOutNode(1, &child, file);
	PrintVisitorData data {ss};	
	node.Visit(pv, (void*) &data);		
	cout << ss.str();
}