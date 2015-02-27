#ifndef LINEARSCANNER_H
#define LINEARSCANNER_H 

#include "Record.h"
#include "File.h"

class LinearScanner {
	friend class MockLinearScanner;
private:
	File& file;
	Page& cursor;
	off_t& cursorIndex;
	const off_t& lastIndex;
	const off_t NEGATIVE_ONE = -1;
	int GetLength();
	LinearScanner();
public:
	/*
	 * Reads from the first record of cursor, and records from subsequent pages
	 * in file until the end of the file
	*/
	LinearScanner(File& file, Page& cursor, off_t& cursorIndex);
	/*
	 * Reads from the first record of cursor, and records from subsequent pages
	 * in file up to and including lastIndex.
	*/
	LinearScanner(File& file, Page& cursor, off_t& cursorIndex, const off_t& lastIndex);
	virtual ~LinearScanner();
	
	/*
	 * Gets the next record from the file. Returns 1 if a record was
	 * found 0 otherwise.
	*/
	virtual int GetNext(Record& rec);
};

#endif
