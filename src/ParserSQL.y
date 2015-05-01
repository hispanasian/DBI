 
%{

	#include "ParseTree.h" 
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <iostream>

	extern "C" int yysqllex();
	extern "C" int yysqlparse();
	extern "C" void yysqlerror(char *s);
  
	// these data structures hold the result of the parsing
	struct FuncOperator *finalFunction; // the aggregate function (NULL if no agg)
	struct TableList *tables; // the list of tables and aliases in the query
	struct AndList *boolean; // the predicate in the WHERE clause
	struct NameList *groupingAtts; // grouping atts (NULL if no grouping)
	struct NameList *attsToSelect; // the set of attributes in the SELECT (NULL if no such atts)
	struct NameList *attsToCreate; // the set of attributes in the SELECT (NULL if no such atts)
	struct CreateTable* createData; // data associated with creating a Table
	int distinctAtts; // 1 if there is a DISTINCT in a non-aggregate query 
	int distinctFunc;  // 1 if there is a DISTINCT in an aggregate query
	int command; // Says whether it is a create table, insert, drop table, set output, or select
	int outType; // The type of the output
	char *refFile; // a referenced file
	char *refTable; // a referenced table
	

%}

// this stores all of the types returned by production rules
%union {
 	struct FuncOperand *myOperand;
	struct FuncOperator *myOperator; 
	struct TableList *myTables;
	struct ComparisonOp *myComparison;
	struct Operand *myBoolOperand;
	struct OrList *myOrList;
	struct AndList *myAndList;
	struct NameList *myNames;
	struct CreateTable* createTable;
	struct AttDesc* attDesc;
	char *actualChars;
	char whichOne;
	int myInt;
}

%token <actualChars> Name
%token <actualChars> QualifiedName
%token <actualChars> FileName
%token <actualChars> Float
%token <actualChars> Int
%token <actualChars> String
%token QUIT
%token CREATE_TABLE
%token HEAP
%token SORTED
%token INSERT
%token INTO
%token DROP_TABLE
%token SET_OUTPUT
%token STDOUT;
%token NONE;
%token INTEGER;
%token DBL;
%token STR;
%token SELECT
%token GROUP 
%token DISTINCT
%token BY
%token FROM
%token WHERE
%token SUM
%token AS
%token AND
%token OR

%type <myOrList> OrList
%type <myAndList> AndList
%type <myOperand> SimpleExp
%type <myOperator> CompoundExp
%type <whichOne> Op 
%type <myComparison> BoolComp
%type <myComparison> Condition
%type <myTables> Tables
%type <myBoolOperand> Literal
%type <myNames> Atts
%type <actualChars> Output
%type <createTable> TableData
%type <attDesc> AttList
%type <myNames> NameList
%type <myInt> AttType

%start SQL


//******************************************************************************
// SECTION 3
//******************************************************************************
/* This is the PRODUCTION RULES section which defines how to "understand" the 
 * input language and what action to take for each "statment"
 */

%%

SQL: CREATE_TABLE TableData
{
	command = CREATE;
	createData = $2;
	
	finalFunction = NULL;
	tables = NULL;
	boolean = NULL;
	groupingAtts = NULL;
	attsToSelect = NULL;
	refFile = NULL;
}

| INSERT Insertion
{
	finalFunction = NULL;
	tables = NULL;
	boolean = NULL;
	groupingAtts = NULL;
	attsToSelect = NULL;
	createData = NULL;
	
	command = INSERT_INTO;
}

| DROP_TABLE Name ';'
{	
	finalFunction = NULL;
	tables = NULL;
	boolean = NULL;
	groupingAtts = NULL;
	attsToSelect = NULL;
	refFile = NULL;
	createData = NULL;
	
	command = DROP;
	refTable= $2;
}

| SET_OUTPUT Output ';'
{
	finalFunction = NULL;
	tables = NULL;
	boolean = NULL;
	groupingAtts = NULL;
	attsToSelect = NULL;
	refFile = NULL;
	refTable= NULL;
	createData = NULL;
	
	command = OUTPUT_SET;
	refFile = $2;
}

| QUIT ';'
{
	finalFunction = NULL;
	tables = NULL;
	boolean = NULL;
	groupingAtts = NULL;
	attsToSelect = NULL;
	refFile = NULL;
	refTable= NULL;
	createData = NULL;

	command = QUIT_SQL;
}

| QUERY
{
	command = SELECT_TABLE;
	
	refFile = NULL;
	refTable= NULL;
	createData = NULL;
};

TableData: Name '(' AttList ')' AS HEAP ';'
{
	$$ = (struct CreateTable*) malloc (sizeof (struct CreateTable));
	refTable= $1;
	$$->atts = $3;
	$$->type = HEAP_DB;
	$$->sort = NULL;
}

| Name '(' AttList ')' AS SORTED NameList ';'
{
	$$ = (struct CreateTable*) malloc (sizeof (struct CreateTable));
	refTable= $1;
	$$->atts = $3;
	$$->type = SORTED_DB;
	$$->sort = $7;
};

Insertion: Name INTO Name ';'
{
	refFile = $1;
	refTable = $3;
}

| QualifiedName INTO Name ';'
{
	refFile = $1;
	refTable = $3;
}

| FileName INTO Name ';'
{
	refFile = $1;
	refTable = $3;
}

AttList: Name AttType
{
	$$ = (struct AttDesc*) malloc (sizeof (struct AttDesc));
	$$->name = $1;
	$$->type = $2;
	$$->next = NULL;
} 

| AttList ',' Name AttType
{
	$$ = (struct AttDesc*) malloc (sizeof (struct AttDesc));
	$$->name = $3;
	$$->type = $4;
	$$->next = $1;
}

AttType: INTEGER
{
	$$ = INT;
}

| DBL
{
	$$ = DOUBLE;
}

| STR
{
	$$ = STRING;
};

NameList: Name
{
	$$ = (struct NameList *) malloc (sizeof (struct NameList));
	$$->name = $1;
	$$->next = NULL;
}

| NameList ',' Name
{
	$$ = (struct NameList *) malloc (sizeof (struct NameList));
	$$->name = $3;
	$$->next = $1;
};

Output: STDOUT
{
	$$ = NULL;
	outType = SET_STDOUT;
}

| NONE
{
	$$ = NULL;
	outType = SET_NONE;
}

| Name
{
	$$ = $1;
	outType = SET_FILE;
}

| QualifiedName
{
	$$ = $1;
	outType = SET_FILE;
}

| FileName
{
	$$ = $1;
	outType = SET_FILE;
};

QUERY: SELECT WhatIWant FROM Tables ';'
{
	tables = $4;
	boolean = NULL;	
	groupingAtts = NULL;
}

| SELECT WhatIWant FROM Tables WHERE AndList ';'
{
	tables = $4;
	boolean = $6;	
	groupingAtts = NULL;
}

| SELECT WhatIWant FROM Tables WHERE AndList GROUP BY Atts ';'
{
	tables = $4;
	boolean = $6;	
	groupingAtts = $9;
};

WhatIWant: Function ',' Atts 
{
	attsToSelect = $3;
	distinctAtts = 0;
}

| Function
{
	attsToSelect = NULL;
	distinctAtts = 0;
}

| Atts 
{
	distinctAtts = 0;
	distinctFunc = 0;
	finalFunction = NULL;
	attsToSelect = $1;
}

| DISTINCT Atts
{
	distinctAtts = 1;
	distinctFunc = 0;
	finalFunction = NULL;
	attsToSelect = $2;
};

Function: SUM '(' CompoundExp ')'
{
	distinctFunc = 0;
	finalFunction = $3;
}

| SUM DISTINCT '(' CompoundExp ')'
{
	distinctFunc = 1;
	finalFunction = $4;
};

Atts: QualifiedName
{
	$$ = (struct NameList *) malloc (sizeof (struct NameList));
	$$->name = $1;
	$$->next = NULL;
} 

| Atts ',' QualifiedName
{
	$$ = (struct NameList *) malloc (sizeof (struct NameList));
	$$->name = $3;
	$$->next = $1;
}

| Name
{
	yysqlerror($1);
}

| Atts ',' Name
{
	yysqlerror($3);
};

Tables: Name AS Name 
{
	$$ = (struct TableList *) malloc (sizeof (struct TableList));
	$$->tableName = $1;
	$$->aliasAs = $3;
	$$->next = NULL;
}

| Tables ',' Name AS Name
{
	$$ = (struct TableList *) malloc (sizeof (struct TableList));
	$$->tableName = $3;
	$$->aliasAs = $5;
	$$->next = $1;
}



CompoundExp: SimpleExp Op CompoundExp
{
	$$ = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	$$->leftOperator = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));
	$$->leftOperator->leftOperator = NULL;
	$$->leftOperator->leftOperand = $1;
	$$->leftOperator->right = NULL;
	$$->leftOperand = NULL;
	$$->right = $3;
	$$->code = $2;	

}

| '(' CompoundExp ')' Op CompoundExp
{
	$$ = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	$$->leftOperator = $2;
	$$->leftOperand = NULL;
	$$->right = $5;
	$$->code = $4;	

}

| '(' CompoundExp ')'
{
	$$ = $2;

}

| SimpleExp
{
	$$ = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	$$->leftOperator = NULL;
	$$->leftOperand = $1;
	$$->right = NULL;	

}

| '-' CompoundExp
{
	$$ = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	$$->leftOperator = $2;
	$$->leftOperand = NULL;
	$$->right = NULL;	
	$$->code = '-';

}
;

Op: '-'
{
	$$ = '-';
}

| '+'
{
	$$ = '+';
}

| '*'
{
	$$ = '*';
}

| '/'
{
	$$ = '/';
}
;

AndList: '(' OrList ')' AND AndList
{
        // here we need to pre-pend the OrList to the AndList
        // first we allocate space for this node
        $$ = (struct AndList *) malloc (sizeof (struct AndList));

        // hang the OrList off of the left
        $$->left = $2;

        // hang the AndList off of the right
        $$->rightAnd = $5;

}

| '(' OrList ')'
{
        // just return the OrList!
        $$ = (struct AndList *) malloc (sizeof (struct AndList));
        $$->left = $2;
        $$->rightAnd = NULL;
}
;

OrList: Condition OR OrList
{
        // here we have to hang the condition off the left of the OrList
        $$ = (struct OrList *) malloc (sizeof (struct OrList));
        $$->left = $1;
        $$->rightOr = $3;
}

| Condition
{
        // nothing to hang off of the right
        $$ = (struct OrList *) malloc (sizeof (struct OrList));
        $$->left = $1;
        $$->rightOr = NULL;
}
;

Condition: Literal BoolComp Literal
{
        // in this case we have a simple literal/variable comparison
        $$ = $2;
        $$->left = $1;
        $$->right = $3;
}
;

BoolComp: '<'
{
        // construct and send up the comparison
        $$ = (struct ComparisonOp *) malloc (sizeof (struct ComparisonOp));
        $$->code = LESS_THAN;
}

| '>'
{
        // construct and send up the comparison
        $$ = (struct ComparisonOp *) malloc (sizeof (struct ComparisonOp));
        $$->code = GREATER_THAN;
}

| '='
{
        // construct and send up the comparison
        $$ = (struct ComparisonOp *) malloc (sizeof (struct ComparisonOp));
        $$->code = EQUALS;
}
;

Literal : String
{
        // construct and send up the operand containing the string
        $$ = (struct Operand *) malloc (sizeof (struct Operand));
        $$->code = STRING;
        $$->value = $1;
}

| Float
{
        // construct and send up the operand containing the FP number
        $$ = (struct Operand *) malloc (sizeof (struct Operand));
        $$->code = DOUBLE;
        $$->value = $1;
}

| Int
{
        // construct and send up the operand containing the integer
        $$ = (struct Operand *) malloc (sizeof (struct Operand));
        $$->code = INT;
        $$->value = $1;
}

| QualifiedName
{
        // construct and send up the operand containing the name
        $$ = (struct Operand *) malloc (sizeof (struct Operand));
        $$->code = NAME;
        $$->value = $1;
}

| Name
{
		yysqlerror($1);
}
;


SimpleExp: 

Float
{
        // construct and send up the operand containing the FP number
        $$ = (struct FuncOperand *) malloc (sizeof (struct FuncOperand));
        $$->code = DOUBLE;
        $$->value = $1;
} 

| Int
{
        // construct and send up the operand containing the integer
        $$ = (struct FuncOperand *) malloc (sizeof (struct FuncOperand));
        $$->code = INT;
        $$->value = $1;
} 

| QualifiedName
{
        // construct and send up the operand containing the name
        $$ = (struct FuncOperand *) malloc (sizeof (struct FuncOperand));
        $$->code = NAME;
        $$->value = $1;
}

| Name
{
		yysqlerror($1);
}
;

%%

