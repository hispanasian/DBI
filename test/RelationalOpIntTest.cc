#include "RelationalOpIntTest.h"
#include "SelectPipe.h"
#include "SelectFile.h"
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