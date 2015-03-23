#include "RelationalOpIntTest.h"
#include "SelectPipe.h"
#include "SelectFile.h"
#include "Project.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "DuplicateRemoval.h"
#include "Join.h"


// Simple test, 25 records expected
TEST_F(RelationalOpIntTest, SelectPipe) {
	FILE *tableFile = fopen ("data/TPCH/10M/lineitem.tbl", "r");
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
	op.Use_n_Pages(10);
	op.Run(in, out, cnf, lit);

	// fill the input pipe
	while (temp.SuckNextRecord (&schema, tableFile) == 1) {
		in.Insert(&temp);
    }
    in.ShutDown();
    op.WaitUntilDone();
    // count the number of records in the output
    int count = 0;
    while(out.Remove(&temp)) {
    	++count;
    }

	fclose(tableFile);

	EXPECT_EQ(25, count);
}

// Simple test, 25 records expected
TEST_F(RelationalOpIntTest, SelectFile) {
	FILE *tableFile = fopen ("data/TPCH/10M/lineitem.tbl", "r");
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

	char* tempFilename = "tempSelectFileTest.bin";
	char* tempHeaderFilename = "tempSelectFileTest.bin.header";
	DBFile in;
	in.Create(tempFilename, heap, NULL);
	Pipe out(100);

	// fill the input file
	in.Open(tempFilename);
	while (temp.SuckNextRecord (&schema, tableFile) == 1) {
		in.Add(temp);
    }
    in.Close();


	SelectFile op;
	in.Open(tempFilename); // responsibility of the caller
	op.Use_n_Pages(10);
	op.Run(in, out, cnf, lit);
	op.WaitUntilDone();
	
    // count the number of records in the output
    int count = 0;
    while(out.Remove(&temp)) {
    	++count;
    }
    in.Close();

	fclose(tableFile);
	remove(tempFilename);
	remove(tempHeaderFilename);

	EXPECT_EQ(25, count);
}

// (l_partkey = 17)
// project it to the 1st and only attribute
// check that there are 28 records in output
// they should all have 1 attribute of type int and value 17
// Note: we're using a DBFile here to get records with a value
// that we expect 
TEST_F(RelationalOpIntTest, Project) {
	FILE *tableFile = fopen ("data/TPCH/10M/lineitem.tbl", "r");
	Record temp;
	Schema schema ("data/catalog", "lineitem");
	Record lit;
	CNF cnf;

	// CAREFUL
	// If you change this query, you might cause a dealock
	// because the output pipe will fill up before the input pipe
	// can be emptied. You'll need another thread.
	char* predicate = "(l_partkey = 17)";
	SetupCNF (predicate, &schema, cnf, lit);

	char* tempFilename = "tempSelectFileTest.bin";
	char* tempHeaderFilename = "tempSelectFileTest.bin.header";
	DBFile in;
	in.Create(tempFilename, heap, NULL);
	Pipe out(100);

	// fill the input file
	in.Open(tempFilename);
	while (temp.SuckNextRecord (&schema, tableFile) == 1) {
		in.Add(temp);
    }
    in.Close();


	Pipe inPipe(100);
	int keepMe[] = {1};
	int* keepMePtr = keepMe;
	int numAttsInput = 16;
	int numAttsOutput = 1;
	Project op;
	op.Use_n_Pages(10);
	op.Run(inPipe, out, keepMePtr, numAttsInput, numAttsOutput);

	in.Open(tempFilename);
	while(in.GetNext(temp, cnf, lit)) {
		inPipe.Insert(&temp);
	}
	in.Close();
	inPipe.ShutDown();

	op.WaitUntilDone();
	
    // count the number of records in the output
    ComparisonEngine comp;
    Attribute* atts = new Attribute[1];
    atts[0] = *new Attribute{"int", Int};
    char* schemaName = new (std::nothrow) char[3];
    Schema out_sch (schemaName, 1, atts);
    int count = 0;
    while(out.Remove(&temp)) {
    	++count;
    	EXPECT_STREQ("int: [17]\n", temp.ToString(&out_sch).c_str());
    }

	fclose(tableFile);
	remove(tempFilename);
	remove(tempHeaderFilename);

	EXPECT_EQ(28, count);
}

// put a bunch of the same record in the input,
// and only one should come out
TEST_F(RelationalOpIntTest, DuplicateRemoval) {
	Record rec;
	Attribute* atts = new Attribute[3];
    atts[0] = *new Attribute{"int", Int};
    atts[1] = *new Attribute{"double", Double};
    atts[2] = *new Attribute{"string", String};
    char* schemaName = new (std::nothrow) char[3];
    Schema schema(schemaName, 3, atts);

    Pipe in = Pipe(10);
    Pipe out = Pipe(10);

    DuplicateRemoval op;
    op.Use_n_Pages(10);
    op.Run(in, out, schema);
	for(int i = 0; i < 7; ++i) {
		rec.ComposeRecord(&schema, "1|2.0|Three|");
		in.Insert(&rec);
	}
	in.ShutDown();
	op.WaitUntilDone();
	EXPECT_EQ(1, out.Remove(&rec));
	// rec.Print(&schema);
	EXPECT_STREQ("int: [1], double: [2.000000], string: [Three]\n", rec.ToString(&schema).c_str());
	EXPECT_EQ(0, out.Remove(&rec));
}

// Do an equi-join which will result in a sort-merge join
TEST_F(RelationalOpIntTest, Join1) {
	Record rec;
	Attribute* attsLeft = new Attribute[3];
    attsLeft[0] = *new Attribute{"l_int", Int};
    attsLeft[1] = *new Attribute{"l_double", Double};
    attsLeft[2] = *new Attribute{"l_string", String};
    char* schemaNameLeft = new (std::nothrow) char[3];
    Schema schemaLeft(schemaNameLeft, 3, attsLeft);

    Attribute* attsRight = new Attribute[3];
    attsRight[0] = *new Attribute{"r_int", Int};
    attsRight[1] = *new Attribute{"r_double", Double};
    attsRight[2] = *new Attribute{"r_string", String};
    char* schemaNameRight = new (std::nothrow) char[3];
    Schema schemaRight(schemaNameRight, 3, attsRight);

    Attribute* attsMerged = new Attribute[6];
    attsMerged[0] = *new Attribute{"l_int", Int};
    attsMerged[1] = *new Attribute{"l_double", Double};
    attsMerged[2] = *new Attribute{"l_string", String};
    attsMerged[3] = *new Attribute{"r_int", Int};
    attsMerged[4] = *new Attribute{"r_double", Double};
    attsMerged[5] = *new Attribute{"r_string", String};
    char* schemaNameMerged = new (std::nothrow) char[3];
    Schema schemaMerged(schemaNameMerged, 6, attsMerged);

    CNF cnf;
    Record literal;
    SetupCNF("(l_int = r_int)", &schemaLeft, &schemaRight, cnf, literal);

    Pipe inLeft = Pipe(10);
    Pipe inRight = Pipe(10);
    Pipe out = Pipe(10);

    Join op;
    op.Use_n_Pages(10);
    op.Run(inLeft, inRight, out, cnf, literal);

	rec.ComposeRecord(&schemaLeft, "1|1.0|Three|"); inLeft.Insert(&rec);
	rec.ComposeRecord(&schemaLeft, "1|2.0|Two|"); inLeft.Insert(&rec);
	rec.ComposeRecord(&schemaLeft, "2|3.0|Three|"); inLeft.Insert(&rec);
	rec.ComposeRecord(&schemaLeft, "2|4.0|Four|"); inLeft.Insert(&rec);
	rec.ComposeRecord(&schemaLeft, "2|5.0|Five|"); inLeft.Insert(&rec);
	rec.ComposeRecord(&schemaLeft, "3|6.0|Six|"); inLeft.Insert(&rec);
	rec.ComposeRecord(&schemaLeft, "3|7.0|Seven|"); inLeft.Insert(&rec);
	inLeft.ShutDown();

	rec.ComposeRecord(&schemaRight, "1|8.0|Eight|"); inRight.Insert(&rec);
	rec.ComposeRecord(&schemaRight, "1|9.0|Nine|"); inRight.Insert(&rec);
	rec.ComposeRecord(&schemaRight, "2|10.0|One-Zero|"); inRight.Insert(&rec);
	rec.ComposeRecord(&schemaRight, "2|11.0|One-One|"); inRight.Insert(&rec);
	rec.ComposeRecord(&schemaRight, "2|12.0|One-Two|"); inRight.Insert(&rec);
	rec.ComposeRecord(&schemaRight, "3|13.0|One-Three|"); inRight.Insert(&rec);
	rec.ComposeRecord(&schemaRight, "3|14.0|One-Four|"); inRight.Insert(&rec);
	inRight.ShutDown();

	int count = 0;
	while(out.Remove(&rec)) {
		++count;
		rec.Print(&schemaMerged);
	}
	EXPECT_EQ(17, count);
}

