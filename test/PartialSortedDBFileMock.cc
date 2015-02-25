#include "../include/SortedDBFileTest.h"
#include "../include/SortedDBFile.h"

PartialSortedDBFileMock::PartialSortedDBFileMock(File &file, RawFile &rfile, DBConfig &config, ComparisonEngine &comp, char *f_path, SortInfo *sortInfo):
	SortedDBFile(file, rfile, config, comp, f_path, sortInfo) {	}

PartialSortedDBFileMock::~PartialSortedDBFileMock() {}

void PartialSortedDBFileMock::Load (Schema &myschema, char *loadpath, Record &record) { SortedDBFile::Load(myschema, loadpath, record); }

void PartialSortedDBFileMock::Flush(File &temp) { SortedDBFile::Flush(temp); }

//void PartialSortedDBFileMock::Flush(HeapDBFile &temp) { SortedDBFile::Flush(temp); }

void PartialSortedDBFileMock::Load (Schema &myschema, char *loadpath) { SortedDBFile::Load(myschema, loadpath); }

void PartialSortedDBFileMock::MoveFirst () { SortedDBFile::MoveFirst(); }

void PartialSortedDBFileMock::Add (Record &addme) { SortedDBFile::Add(addme); }

int PartialSortedDBFileMock::GetNext (Record &fetchme) { return SortedDBFile::GetNext(fetchme); }

int PartialSortedDBFileMock::GetNext (Record &fetchme, CNF &cnf, Record &literal) { return SortedDBFile::GetNext(fetchme, cnf, literal); }

//void PartialSortedDBFileMock::Flush() { SortedDBFile::Flush(); }

void PartialSortedDBFileMock::Initialize() { SortedDBFile::Initialize(); }

void PartialSortedDBFileMock::Reset() { SortedDBFile::Reset(); }

bool PartialSortedDBFileMock::BinarySearch(Record &literal, OrderMaker &query, ComparisonEngine &comp, Record &rec, Page &page) { SortedDBFile::BinarySearch(literal, query, comp, rec, page);}

int PartialSortedDBFileMock::GetNext (Record &fetchme, CNF &cnf, Record &literal, ComparisonEngine &comp) { SortedDBFile::GetNext(fetchme, cnf, literal, comp); };

bool PartialSortedDBFileMock::FindValidRecord(Record &literal, OrderMaker &query, int index, Record &rec, Page &page, Page &buff, ComparisonEngine &comp) { return FindValidRecord(literal, query, index, rec, page, buff, comp); }
