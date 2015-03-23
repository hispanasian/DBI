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
		FILE *tableFile = fopen ("data/10M/lineitem.tbl", "r");

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
//
//TEST_F(RelationalOpIntTest, Something) {
//
//		cout << " query6 \n";
//		char *pred_s = "(s_suppkey = s_suppkey)";
//		init_SF_s (pred_s, 100);
//		SF_s.Run (dbf_s, _s, cnf_s, lit_s); // 10k recs qualified
//
//		char *pred_ps = "(ps_suppkey = ps_suppkey)";
//		init_SF_ps (pred_ps, 100);
//
//		Join J;
//			// left _s
//			// right _ps
//			Pipe _s_ps (pipesz);
//			CNF cnf_p_ps;
//			Record lit_p_ps;
//			get_cnf ("(s_suppkey = ps_suppkey)", s->schema(), ps->schema(), cnf_p_ps, lit_p_ps);
//
//		int outAtts = sAtts + psAtts;
//		Attribute s_nationkey = {"s_nationkey", Int};
//		Attribute ps_supplycost = {"ps_supplycost", Double};
//		Attribute joinatt[] = {IA,SA,SA,s_nationkey,SA,DA,SA,IA,IA,IA,ps_supplycost,SA};
//		Schema join_sch ("join_sch", outAtts, joinatt);
//
//
//		// GROUPS HERE
//		GroupBy G;
//			// _s (input pipe)
//			Pipe _out (1);
//			Function func;
//				char *str_sum = "(ps_supplycost)";
//				get_cnf (str_sum, &join_sch, func); ///////////
//				func.Print ();
//				OrderMaker grp_order (&join_sch);
//		G.Use_n_Pages (1);
//
//		SF_ps.Run (dbf_ps, _ps, cnf_ps, lit_ps); // 161 recs qualified
//		J.Run (_s, _ps, _s_ps, cnf_p_ps, lit_p_ps);
//		G.Run (_s_ps, _out, grp_order, func);
//
//		SF_ps.WaitUntilDone ();
//		J.WaitUntilDone ();
//		G.WaitUntilDone ();
//
//		Schema sum_sch ("sum_sch", 1, &DA);
//		int cnt = clear_pipe (_out, &sum_sch, true);
//		cout << " query6 returned sum for " << cnt << " groups (expected 25 groups)\n";
//
//}
