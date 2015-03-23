#include "RelationalOpIntTest.h"
#include "SelectPipe.h"
#include "SelectFile.h"
#include "Project.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "DuplicateRemoval.h"
#include "Function.h"
#include "GroupBy.h"
#include "Defs.h"
#include "Comparison.h"
#include <pthread.h>

//Attribute IA = {"int", Int};
//Attribute SA = {"string", String};
//Attribute DA = {"double", Double};

// Simple test, expect a 7996 Records (the number of unique (partkey,suppkey) pairs in lineitem)
// use spike/IntegrationTestGrouBy.py to find this number
TEST_F(RelationalOpIntTest, GroupBy) {
	Schema schema ("data/catalog", "lineitem");
	Record temp;
	CNF cnf;
	Function fun;
	int thread_id;
	pthread_t worker;

	char *str_sum= "(l_quantity)"; // the summed att
	SetupCNF (str_sum, &schema, fun);
	Pipe in(100);
	Pipe out(100);
	OrderMaker groupAtts ("1 int 2 int");

	GroupBy op;
	op.Use_n_Pages(1);
	op.Run(in, out, groupAtts, fun);

	Pipe* data = &in;

	// Thread that will insert Records so we don't block
	thread_id = pthread_create(&worker, NULL, [] (void* args) -> void* {
		Pipe* in = (Pipe*) args;
		Schema schema ("data/catalog", "lineitem");
		Record temp;
		FILE *tableFile = fopen ("data/TPCH/10M/lineitem.tbl", "r");

		// fill the input pipe
		while (temp.SuckNextRecord (&schema, tableFile) == 1) {
			in->Insert(&temp);
		}
		in->ShutDown();
		fclose(tableFile);
	}, (void*) data);

    // count the number of records in the output
    int count = 0;
    while(out.Remove(&temp)) {
    	++count;
    }

	EXPECT_EQ(7996, count);
}
