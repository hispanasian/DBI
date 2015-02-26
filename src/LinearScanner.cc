#include "LinearScanner.h"

LinearScanner::LinearScanner(): file(file), cursor(cursor), cursorIndex(cursorIndex), lastIndex(0) {}

LinearScanner::LinearScanner(File& _file, Page& _cursor, off_t& _cursorIndex, const off_t& _lastIndex) :
	file(_file), cursor(_cursor), cursorIndex(_cursorIndex), lastIndex(_lastIndex) {}

LinearScanner::LinearScanner(File& _file, Page& _cursor, off_t& _cursorIndex) :
	file(_file), cursor(_cursor), cursorIndex(_cursorIndex), lastIndex(NEGATIVE_ONE) {}


LinearScanner::~LinearScanner() {}

int LinearScanner::GetNext(Record& rec) {
	// cout << lastIndex << endl;
	// cout << GetLength() << endl;
	if(GetLength() == 0) {
		// this file is empty, we can't return any records
		return 0;
	}

	if(cursor.GetFirst(&rec)) {
		// there was a record available in the cursor
		return 1;
	} else {
		// we need to find the next page with a record in it
		// we look through the pages until we find one with a record
		// or we reach the end
		cursorIndex++;
		while(cursorIndex < GetLength()) {
			cursor.EmptyItOut();
			file.GetPage(&cursor, cursorIndex);
			if(cursor.GetFirst(&rec)) {
				return 1;
			}
			cursorIndex++;
		}
		// we read the last page without finding anything, make sure
		// our index stays in range
		cursorIndex--;
		return 0;
	}
}

int LinearScanner::GetLength() {
	off_t zero = 0;
	return lastIndex == -1 ? max(zero, file.GetLength()-1) : lastIndex+1;
}
