#include "RelationalOpIntTest.h"
#include "SelectPipe.h"
#include "SelectFile.h"
#include "Project.h"
#include "ComparisonEngine.h"
#include "DBFile.h"


// Simple test, 25 records expected
TEST_F(RelationalOpIntTest, SelectPipe) {
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
	FILE *tableFile = fopen ("data/10M/lineitem.tbl", "r");
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