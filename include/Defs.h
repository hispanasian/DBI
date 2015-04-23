#ifndef DEFS_H
#define DEFS_H


#define MAX_ANDS 20
#define MAX_ORS 20

#define PAGE_SIZE 131072
#define PIPE_SIZE 100
#define RUN_LENGTH 100

#include "ParseTree.h"

enum Target {Left, Right, Literal};
enum CompOperator {LessThan, GreaterThan, Equals};
enum Type {Int = INT, Double = DOUBLE, String = STRING};
enum DB_Type {Heap = HEAP_DB, Sorted = SORTED_DB};
enum Output_Type {Stdout = SET_STDOUT, File_Path = SET_FILE, None = SET_NONE};
enum SQL_Command {
	Create_Table = CREATE,
	Insert_Into = INSERT_INTO,
	Drop_Table = DROP,
	Set_Output = OUTPUT_SET,
	Select = SELECT_TABLE,
	Quit = QUIT_SQL,
	Unknown = -1
};

unsigned int Random_Generate();


#endif

