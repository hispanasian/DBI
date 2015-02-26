#include "SortedDBFileTest.h"
#include "SortedDBFile.h"

/*
	If we're in reading mode and the file is not empty,
	read the first page into the cursor
*/
TEST_F(SortedDBFileTest, MoveFirst1) {
	
}

/*
	If we're in writing mode and the file is not empty,
	flush the pipe, read the first page into memory, and 
	set the cursor index to 0
*/

/*
	If we're in reading mode and the file is empty,
	set the cursor index to 0
*/

/*
	If we're in writing mode and the file is empty,
	flush the pipe and set teh cursor index to 0
*/