#ifndef sorteddbDBFILETEST_H
#define sorteddbDBFILETEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "../include/MockClasses.h"
#include "../include/DBFile.h"
#include "../include/File.h"
#include "../include/Schema.h"
#include "SortedDBFile.h"

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Sequence;
using ::testing::StrictMock;
using ::testing::AtMost;
using ::testing::AtLeast;
using ::testing::ByRef;
using ::testing::Pointee;
using ::testing::NotNull;
using ::testing::SetArgPointee;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::Ref;

class PartialSortedDBFileMock: public SortedDBFile {
public:
	MOCK_METHOD1(Flush, void(HeapDBFile &temp));
	MOCK_METHOD0(Flush, void());
	MOCK_METHOD2(BinarySearch, bool(Record &literal, OrderMaker &query));
	MOCK_METHOD2(GetBSTPage, void(Page &page, off_t index));
	MOCK_METHOD3(FindValidRecord, bool(Record &literal, OrderMaker &query, off_t index));
private:
	virtual void Load (Schema &myschema, char *loadpath, Record &record);
	virtual void Flush(File &temp);
//	virtual void Flush(HeapDBFile &temp);

public:
	PartialSortedDBFileMock(File &file, RawFile &rfile, DBConfig &config, ComparisonEngine &comp, char *f_path, SortInfo *sortInfo);
	virtual ~PartialSortedDBFileMock();
	virtual void Load (Schema &myschema, char *loadpath);
	virtual void MoveFirst ();
	virtual void Add (Record &addme);
	virtual int GetNext (Record &fetchme);
	virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal);
//	virtual void Flush();
	virtual void Initialize();
	virtual void Reset();
//	virtual bool BinarySearch(Record &literal, OrderMaker &query);
	virtual bool BinarySearch(Record &literal, OrderMaker &query, ComparisonEngine &comp, Record &rec, Page &page);
//	virtual void GetBSTPage(Page &page, off_t index);
	virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal, ComparisonEngine &comp);
//	virtual bool FindValidRecord(Record &literal, OrderMaker &query, int index);
	virtual bool FindValidRecord(Record &literal, OrderMaker &query, int index, Record &rec, Page &page, Page &buff, ComparisonEngine &comp);
};

class SortedDBFileTest: public ::testing::Test {
public:
	StrictMock<MockFile> mockFile;
	StrictMock<MockDBConfig> config;
	StrictMock<MockRawFile> rfile;
	StrictMock<MockComparisonEngine> comp;
	StrictMock<MockPage> cursor;
	StrictMock<MockPage> last;
	StrictMock<MockSchema> schema;
	PartialSortedDBFileMock *mock = NULL;
	SortInfo *sortInfo = new SortInfo{ new OrderMaker("0 int 5 string 6 int"), 5 };

	char *path = "asdasdasd";
	char *header = "asdasdasd.header";

	DBFile file = DBFile(mockFile, rfile, config, comp);
	SortedDBFile *sorteddb = new SortedDBFile(mockFile, rfile, config, comp, path, sortInfo);

	void MakeSortedDBFile(SortInfo *sortInfo) {
		delete sorteddb;
		sorteddb = new SortedDBFile(mockFile, rfile, config, comp, path, sortInfo);
	}

	void MakeFlushlessSorted(SortInfo *sortInfo) {
		mock = new PartialSortedDBFileMock(mockFile, rfile, config, comp, path, sortInfo);
		sorteddb = mock;
	}

	File GetFile() { return file.file; }

	void Load(Schema &myschema, char *loadpath, Record &record) { file.Load(myschema, loadpath, record); }

	void SetDBTosorteddb() {
		file.delegate = sorteddb;
	}

	GenericDBFile* Delegate() { return file.delegate; }

	void Flush(File &temp) { sorteddb -> Flush(temp); }

	void Flush(HeapDBFile &temp) { sorteddb -> Flush(temp); }

	void SetDBNull() { file.delegate = NULL; }

	void SetSortInfo(SortInfo *sortInfo) { ((SortedDBFile*)file.delegate)-> sortInfo = sortInfo; }

	OrderMaker* GetOrder() { return ((SortedDBFile*)file.delegate)-> sortInfo -> myOrder; }

	int GetRunLength() { return sorteddb -> sortInfo -> runLength; }

	void SetIn(Pipe *in) { sorteddb -> in = in; }

	Pipe * GetIn() { return sorteddb -> in;}

	void SetOut(Pipe *out) { sorteddb -> out = out; }

	Pipe * GetOut() { return sorteddb -> out;}

	void SetRWState(RWState state) { sorteddb -> rwState = state; }

	bool GetRWState() { return sorteddb -> rwState; }

	GetNextState GetGetNextState() { return sorteddb -> getNextState; }

	void SetCursor(Page* cursor) { sorteddb->cursor = cursor;}

	void SetCursorIndex(int i) { sorteddb->cursorIndex = i;}

	int GetCursorIndex() { return sorteddb->cursorIndex;}

	virtual bool BinarySearch(Record &literal, OrderMaker &query, ComparisonEngine &comp, Record &rec, Page &page) { return sorteddb -> BinarySearch(literal, query, comp, rec, page); }

	virtual void GetBSTPage(Page &page, off_t index) { sorteddb -> GetBSTPage(page, index); }

	virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal, ComparisonEngine &comp) { return sorteddb -> GetNext(fetchme, cnf, literal, comp);}

	virtual bool FindValidRecord(Record &literal, OrderMaker &query, off_t index, Record &rec, Page &page, Page &buff, ComparisonEngine &comp) { return sorteddb -> FindValidRecord(literal, query, index, rec, page, buff, comp); }
};

#endif
