#ifndef DEFS_H
#define DEFS_H


#define MAX_ANDS 20
#define MAX_ORS 20

#define PAGE_SIZE 131072
#define PIPE_SIZE 100
#define RUN_LENGTH 100


enum Target {Left, Right, Literal};
enum CompOperator {LessThan, GreaterThan, Equals};
enum Type {Int, Double, String};
enum DB_Type {Heap, Sorted};
enum Output_Type {Stdout, File_Path, None};
enum SQL_Command { Create_Table, Insert_Into, Drop_Table, Set_Output, Select, Quit, Unknown };

unsigned int Random_Generate();


#endif

