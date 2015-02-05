#include "DBFileTest.h"

/*
 * GetNext() should call GetFirst() on the cursor
 * and pass addMe as a paramter
 */

/*
 * If cursor has no more records, GetNext() should increment
 * the cursor index, read the next page into cursor,
 * and return the next record
 */

/*
* GetNext() should seek across empty pages to find
* the next non-empty page to get a record from
*/

/*
 * If there are no pages in the file,
 * GetNext() should return zero
 */

/*
 * If all of the pages in the file contain
 * 0 records, GetNext() should return 0
 */

/*
 * After reading all records from the file,
 * GetNext() should continue to return 0 
 * on successive calls
 */