#include "RelationalOpIntTest.h"
#include "SelectPipe.h"
#include "SelectFile.h"
#include "Project.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "DuplicateRemoval.h"
#include "Function.h"
#include "Sum.h"
#include "Defs.h"

// Simple test, expect a Record with an int of 1802759573 (sum of orderkey)
TEST_F(RelationalOpIntTest, SumInt) {
	FILE *tableFile = fopen ("data/TPCH/10M/lineitem.tbl", "r");
	Record temp;
	Schema schema ("data/catalog", "lineitem");
	Record lit;
	CNF cnf;
	Function fun;

	char *str_sum = "(l_orderkey)";
	SetupCNF (str_sum, &schema, fun);
	Pipe in(100);
	Pipe out(1);

	Sum op;
	op.Use_n_Pages(1);
	op.Run(in, out, fun);

	// fill the input pipe
	while (temp.SuckNextRecord (&schema, tableFile) == 1) {
		in.Insert(&temp);
    }
    in.ShutDown();
    op.WaitUntilDone();
    // count the number of records in the output. There should only be one Record
    int count = 0;
    while(out.Remove(&temp)) {
    	++count;
    }

	fclose(tableFile);
	char *attname = "i";
	Attribute att = { attname, "", Int };
	Attribute atts[] = { att };
	char *path = "";
	Schema intSchema = Schema(path, 1, atts);
	std::string expected = "i: [1802759573]\n"; // the expected sum

	EXPECT_EQ(0, expected.compare(temp.ToString(&intSchema)));
	EXPECT_EQ(1, count);
}

// Simple test, expect a Record with a double of 2420.51 (sum of t)
TEST_F(RelationalOpIntTest, SumDouble) {
	FILE *tableFile = fopen ("data/TPCH/10M/lineitem.tbl", "r");
	Record temp;
	Schema schema ("data/catalog", "lineitem");
	Record lit;
	CNF cnf;
	Function fun;

	char *str_sum = "(l_tax)";
	SetupCNF (str_sum, &schema, fun);
	Pipe in(100);
	Pipe out(1);

	Sum op;
	op.Use_n_Pages(1);
	op.Run(in, out, fun);

	// fill the input pipe
	while (temp.SuckNextRecord (&schema, tableFile) == 1) {
		in.Insert(&temp);
    }
    in.ShutDown();
    op.WaitUntilDone();
    // count the number of records in the output. There should only be one Record
    int count = 0;
    while(out.Remove(&temp)) {
    	++count;
    }

	fclose(tableFile);
	char *attname = "d";
	Attribute att = { attname, "", Double };
	Attribute atts[] = { att };
	char *path = "";
	Schema intSchema = Schema(path, 1, atts);
	std::string expected = "d: [2420.510000]\n"; // the expected sum

	EXPECT_EQ(0, expected.compare(temp.ToString(&intSchema)));
	EXPECT_EQ(1, count);
}
