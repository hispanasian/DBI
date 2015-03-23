#include "RelationalOpIntTest.h"
#include "SelectPipe.h"
#include "SelectFile.h"
#include "Project.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "DuplicateRemoval.h"
#include "WriteOut.h"
#include "RawFile.h"

// Simple test, expects an equal number of Records as was in the original file
TEST_F(RelationalOpIntTest, WriteOut) {
	char name[] = "WriteOutTestXXXXXX";
	RawFile rfile;
	rfile.MakeTemp(name);
	FILE *tableFile = fopen ("data/TPCH/10M/customer.tbl", "r");
	FILE *outFile = fopen(name, "w");
	Record temp;
	Schema schema ("data/catalog", "customer");
	Record lit;

	Pipe in(100);
	WriteOut op;
	op.Run(in, outFile, schema);

	// fill the input pipe
	int expected = 0;
	while (temp.SuckNextRecord (&schema, tableFile) == 1) {
		in.Insert(&temp);
		++expected;
    }
    in.ShutDown();
    op.WaitUntilDone();
    // count the number of records in the output (one Record per line)
    fclose(outFile);
    rfile.Open(name);
    int count = 0;
    string str;
    while (rfile.ReadLine(&str)) {
    	++count;
    }

	fclose(tableFile);
	rfile.Close();
	rfile.Remove(name);

	EXPECT_EQ(expected, count);
}
