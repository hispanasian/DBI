#ifndef MOCKCLASSES_H
#define MOCKCLASSES_H

#include <gmock/gmock.h>
#include <vector>
#include <unordered_map>
#include "Record.h"
#include "File.h"
#include "RawFile.h"
#include "DBFile.h"
#include "DBConfig.h"
#include "Schema.h"
#include "ComparisonEngine.h"
#include "BigQ.h"
#include "Pipe.h"
#include "SortedDBFile.h"
#include "TreeDBFile.h"
#include "PipedPage.h"
#include "LinearScanner.h"
#include "SelectPipe.h"
#include "DuplicateRemoval.h"
#include "Project.h"
#include "SelectFile.h"
#include "WriteOut.h"
#include "Function.h"
#include "Sum.h"
#include "Relation.h"
#include "InMemoryRelation.h"
#include "FileRelation.h"
#include "JoinRelation.h"
#include "GroupBy.h"
#include "Join.h"
#include "Expression.h"
#include "OpNode.h"
#include "SQL.h"
#include "JoinOptimizer.h"

class MockRecord: public Record {
public:
    // void Consume (Record *fromMe);
    MOCK_METHOD1(Consume, void(Record *fromMe));
    // void Copy (Record *copyMe);
    MOCK_METHOD1(Copy, void(Record *copyMe));
    // int SuckNextRecord (Schema *mySchema, FILE *textFile);
    MOCK_METHOD2(SuckNextRecord, int(Schema *mySchema, FILE *textFile));
    // void Project (int *attsToKeep, int numAttsToKeep, int numAttsNow);
    MOCK_METHOD3(Project, void(int *attsToKeep, int numAttsToKeep, int numAttsNow));
    // void MergeRecords (Record *left, Record *right, int numAttsLeft,
    // int numAttsRight, int *attsToKeep, int numAttsToKeep, int startOfRight);
    MOCK_METHOD7(MergeRecords, void(Record *left, Record *right, int numAttsLeft,
    int numAttsRight, int *attsToKeep, int numAttsToKeep, int startOfRight));
    // void Print (Schema *mySchema);
    MOCK_METHOD1(Print, void(Schema *mySchema));
    // string ToSTring (Schema *mySchema);
	MOCK_METHOD1(ToString, string(Schema *mySchema));
//    int Size();
    MOCK_METHOD0(Size, int());
//    virtual int NumAtts();
    MOCK_METHOD0(NumAtts, int());
};

class MockPage: public Page {
public:
    //virtual void ToBinary (char *bits);
    MOCK_METHOD1(ToBinary, void(char *bits));
    //virtual void FromBinary (char *bits);
    MOCK_METHOD1(FromBinary, void(char *bits));
    //virtual int GetFirst (Record *firstOne);
    MOCK_METHOD1(GetFirst, int(Record *firstOne));
    //virtual int Append (Record *addMe);
    MOCK_METHOD1(Append, int(Record *addMe));
    //virtual void EmptyItOut ();
    MOCK_METHOD0(EmptyItOut, void());
};

class MockFile: public File {
public:
//	virtual off_t GetLength ();
	MOCK_METHOD0(GetLength, off_t());
    //virtual void Open (int length, char *fName);
    MOCK_METHOD2(Open, void(int length, const char *fname));
    //virtual void GetPage (Page *putItHere, off_t whichPage);
    MOCK_METHOD2(GetPage, void(Page *putItHere, off_t whichPage));
    //virtual void AddPage (Page *addMe, off_t whichPage);
    MOCK_METHOD2(AddPage, void(Page *addMe, off_t whichPage));
    //virtual int Close ();
    MOCK_METHOD0(Close, int());
};

class MockRawFile: public RawFile {
public:
//    virtual bool Open(std::string fname);
    MOCK_METHOD1(Open, bool(std::string fname));
//    virtual bool Open(FILE *_file);
	MOCK_METHOD1(Open, bool(FILE *_file));
//    virtual bool Close();
    MOCK_METHOD0(Close, bool());
//    virtual int Read(void* buf, size_t count);
    MOCK_METHOD2(Read, int(void* buf, size_t count));
//    virtual int Write(const void* buf, size_t count);
    MOCK_METHOD2(Write, int(const void* buf, size_t count));
//    virtual std::string ReadLine();
    MOCK_METHOD1(ReadLine,  bool(std::string*));
//    virtual bool Append(std::string value);
    MOCK_METHOD1(Append, bool(std::string value));
//    virtual bool Truncate();
    MOCK_METHOD0(Truncate, bool());
//    virtual void LSeek(off_t offset);
    MOCK_METHOD1(LSeek, void(off_t offset));
//    virtual bool RawFile::FileExists(const char* fileName);
    MOCK_METHOD1(FileExists, bool(const char* fileName));
//    virtual void MakeTemp(char *scheme);
    MOCK_METHOD1(MakeTemp, void(char *scheme));
//    virtual int Rename(char *oldName, char *newName);
    MOCK_METHOD2(Rename, int(const char *oldName, const char *newName));
//    virtual int Remove(char *name);
    MOCK_METHOD1(Remove, int(const char *name));
};

class MockDBConfig: public DBConfig {
public:
//    virtual bool Read(RawFile file);
    MOCK_METHOD1(Read, bool(RawFile &file));
//    virtual bool Write(RawFile file);
    MOCK_METHOD1(Write, bool(RawFile &file));
//    virtual bool Open(std::string fname);
    MOCK_METHOD1(Open, bool(std::string fname));
//    virtual bool Close();
    MOCK_METHOD0(Close, bool());
//    virtual void AddKey(std::string key, std::string value);
    MOCK_METHOD2(AddKey, void(std::string key, std::string value));
//    virtual std::string GetKey(std::string key);
    MOCK_METHOD1(GetKey, std::string(std::string key));
//    virtual std::string ReplaceKey(std::string key, std::string value);
    MOCK_METHOD2(ReplaceKey, void(std::string key, std::string value));
//    virtual void Clear();
    MOCK_METHOD0(Clear, void());
};

class MockDBFile: public DBFile {
public:
//	virtual int Create (char *fpath, fType file_type, void *startup);
	MOCK_METHOD3(Create, int(char *fpath, fType file_type, void *startup));
//	virtual int Open (char *fpath);
	MOCK_METHOD1(Open, int(char *fpath));
//	virtual int Close ();
	MOCK_METHOD0(Close, int());
//	virtual void Load (Schema &myschema, char *loadpath);
	MOCK_METHOD2(Load, void(Schema &myschema, char *loadpath));
//	virtual void MoveFirst ();
	MOCK_METHOD0(MoveFirst, void());
//	virtual void Add (Record &addme);
	MOCK_METHOD1(Add, void(Record &addme));
//	virtual int GetNext (Record &fetchme);
	MOCK_METHOD1(GetNext, int(Record &fetchme));
//	virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal);
	MOCK_METHOD3(GetNext, int(Record &fetchme, CNF &cnf, Record &literal));
//	virtual void Reset();
	MOCK_METHOD0(Reset, void());
};

class MockSchema: public Schema {
//	Attribute *GetAtts ();
	MOCK_METHOD0(GetAtts, Attribute*());
//	int GetNumAtts ();
	MOCK_METHOD0(GetNumAtts, int());
//	int Find (char *attName);
	MOCK_METHOD1(Find, int(char *attName));
//	Type FindType (char *attName);
	MOCK_METHOD1(FindType, Type(char *attName));
//	int GetSortOrder (OrderMaker &order);
	MOCK_METHOD1(GetSortOrder, int(OrderMaker &order));
};

class MockComparisonEngine: public ComparisonEngine {
public:
    // int Compare(Record *left, Record *right, OrderMaker *orderUs);
    MOCK_METHOD3(Compare, int(Record *left, Record *right, OrderMaker *orderUs));

    // int Compare(Record *left, OrderMaker *order_left, Record *right, OrderMaker *order_right);
    MOCK_METHOD4(Compare, int(Record *left, OrderMaker *order_left, Record *right, OrderMaker *order_right));

    // int Compare(Record *left, Record *right, Record *literal, CNF *myComparison);
    MOCK_METHOD4(Compare, int(Record *left, Record *right, Record *literal, CNF *myComparison));

    // int Compare(Record *left, Record *literal, CNF *myComparison);
    MOCK_METHOD3(Compare, int(Record *left, Record *literal, CNF *myComparison));

    // int CompareForSearch(Record *left, OrderMaker *order_left, Record *right, OrderMaker *order_right);
    MOCK_METHOD4(CompareForSearch, int(Record *left, OrderMaker *order_left, Record *right, OrderMaker *order_right));
};

class MockOrderMaker: public OrderMaker {
public:
	// int GetNumAtts();
	MOCK_METHOD0(GetNumAtts, int());

	// gets the index of the ith attribute
	// int GetAttIndex(int i);
	MOCK_METHOD1(GetAttIndex, int(int i));

	// get the Type of the ith attribute
	// Type GetAttType(int i);
	MOCK_METHOD1(GetAttType, Type(int i));

	// print to the screen
	// void Print ();
	MOCK_METHOD0(Print, void());

	// returns a string representation of OrderMaker in the following format:
	// OrderMaker = (whichAtt whichType)*
	// std::string ToString();
	MOCK_METHOD0(ToString, std::string());
};

class MockBigQ: public BigQ {

};

class MockPipe: public Pipe {
public:
//	void Insert (Record *insertMe);
	MOCK_METHOD1(Insert, void(Record *insertMe));
//	int Remove (Record *removeMe);
	MOCK_METHOD1(Remove, int(Record *insertMe));
//	void ShutDown ();
	MOCK_METHOD0(ShutDown, void());
};

class MockTPMMS: public TPMMS {
public:
//	virtual void SortRun();
	MOCK_METHOD0(ShortRun, void());
//	virtual void PageToRun();
	MOCK_METHOD0(PageToRun, void());
//	void RunToFile(off_t &totalPageCount);
	MOCK_METHOD1(RunToFile, void(off_t &totalPageCount));
//	virtual void AddRecord(Record* &rec);
	MOCK_METHOD1(AddRecord, bool(Record* &rec));
//	virtual void Phase1();
	MOCK_METHOD0(Phase1, void());
//	virtual void Phase2();
	MOCK_METHOD0(Phase2, void());
//	virtual void Sort();
	MOCK_METHOD0(Sort, void());
//	virtual void GetNextRecord(int min, Record **&heads, off_t *&runIndex, Page **&pages, int &runsLeft);
	MOCK_METHOD5(GetNextRecord, void(int min, Record **&heads, off_t *&runIndex, Page **&pages, int &runsLeft));
//	virtual int FindMin(int size, Record **&heads);
	MOCK_METHOD2(FindMin, int(int size, Record **&heads));
//	virtual void Merge(PipedPage *p1, PipedPage *p2);
	MOCK_METHOD2(Merge, void(PipedPage *p1, PipedPage *p2));
};

class MockGenericDBFile: public GenericDBFile {
public:
//	virtual void Load (Schema &myschema, char *loadpath);
	MOCK_METHOD2(Load, void(Schema &myschema, char *loadpath));
//	virtual void MoveFirst ();
	MOCK_METHOD0(MoveFirst, void());
//	virtual void Add (Record &addme);
	MOCK_METHOD1(Add, void(Record &addme));
//	virtual int GetNext (Record &fetchme);
	MOCK_METHOD1(GetNext, int(Record &fetchme));
//	virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal);
	MOCK_METHOD3(GetNext, int(Record &fetchme, CNF &cnf, Record &literal));
//	virtual void Reset();
	MOCK_METHOD0(Reset, void());
//	virtual bool Flush();
	MOCK_METHOD0(Flush, void());
//	virtual void Initialize();
	MOCK_METHOD0(Initialize, void());
};

class MockHeapDBFile: public HeapDBFile {
public:
//	virtual void Load (Schema &myschema, char *loadpath);
	MOCK_METHOD2(Load, void(Schema &myschema, char *loadpath));
//	virtual void MoveFirst ();
	MOCK_METHOD0(MoveFirst, void());
//	virtual void Add (Record &addme);
	MOCK_METHOD1(Add, void(Record &addme));
//	virtual int GetNext (Record &fetchme);
	MOCK_METHOD1(GetNext, int(Record &fetchme));
//	virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal);
	MOCK_METHOD3(GetNext, int(Record &fetchme, CNF &cnf, Record &literal));
//	virtual void Reset();
	MOCK_METHOD0(Reset, void());
//	virtual bool Flush();
	MOCK_METHOD0(Flush, void());
//	virtual void Initialize();
	MOCK_METHOD0(Initialize, void());
};

class MockSortedDBFile: public SortedDBFile {
public:
//	virtual void Load (Schema &myschema, char *loadpath);
	MOCK_METHOD2(Load, void(Schema &myschema, char *loadpath));
//	virtual void MoveFirst ();
	MOCK_METHOD0(MoveFirst, void());
//	virtual void Add (Record &addme);
	MOCK_METHOD1(Add, void(Record &addme));
//	virtual int GetNext (Record &fetchme);
	MOCK_METHOD1(GetNext, int(Record &fetchme));
//	virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal);
	MOCK_METHOD3(GetNext, int(Record &fetchme, CNF &cnf, Record &literal));
//	virtual void Reset();
	MOCK_METHOD0(Reset, void());
//	virtual bool Flush();
	MOCK_METHOD0(Flush, void());
//	virtual void Initialize();
	MOCK_METHOD0(Initialize, void());
//	virtual void Flush(File &temp);
	MOCK_METHOD1(Flush, void(File &temp));
//	virtual void Flush(HeapDBFile &temp);
	MOCK_METHOD1(Flush, void(HeapDBFile &temp));
//	virtual bool BinarySearch(Record &literal, OrderMaker &query);
	MOCK_METHOD2(BinarySearch, bool(Record &literal, OrderMaker &query));
	//	virtual bool BinarySearch(Record &literal, OrderMaker &query, ComparisonEngine &comp, Record &rec, Page &page);
	MOCK_METHOD5(BinarySearch, bool(Record &literal, OrderMaker &query, ComparisonEngine &comp, Record &rec, Page &page));
//	virtual void GetBSTPage(Page &page, off_t index);
	MOCK_METHOD2(SortedDBFile, void(Page &page, off_t index));
//	virtual bool FindValidRecord(Record &literal, OrderMaker &query, int index);
	MOCK_METHOD3(FindValidRecord, bool(Record &literal, OrderMaker &query, int index));
//	virtual bool FindValidRecord(Record &literal, OrderMaker &query, int index, Record &rec, Page &page, Page &buff, ComparisonEngine &comp);
	MOCK_METHOD7(FindValidRecord, bool(Record &literal, OrderMaker &query, int index, Record &rec, Page &page, Page &buff, ComparisonEngine &comp));
};

class MockTreeDBFile: public TreeDBFile {
public:
//	virtual void Load (Schema &myschema, char *loadpath);
	MOCK_METHOD2(Load, void(Schema &myschema, char *loadpath));
//	virtual void MoveFirst ();
	MOCK_METHOD0(MoveFirst, void());
//	virtual void Add (Record &addme);
	MOCK_METHOD1(Add, void(Record &addme));
//	virtual int GetNext (Record &fetchme);
	MOCK_METHOD1(GetNext, int(Record &fetchme));
//	virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal);
	MOCK_METHOD3(GetNext, int(Record &fetchme, CNF &cnf, Record &literal));
//	virtual void Reset();
	MOCK_METHOD0(Reset, void());
//	virtual bool Flush();
	MOCK_METHOD0(Flush, void());
//	virtual void Initialize();
	MOCK_METHOD0(Initialize, void());
};

class MockPipedPage: public PipedPage {
public:
    //virtual void ToBinary (char *bits);
    MOCK_METHOD1(ToBinary, void(char *bits));
    //virtual void FromBinary (char *bits);
    MOCK_METHOD1(FromBinary, void(char *bits));
    //virtual int GetFirst (Record *firstOne);
    MOCK_METHOD1(GetFirst, int(Record *firstOne));
    //virtual int Append (Record *addMe);
    MOCK_METHOD1(Append, int(Record *addMe));
    //virtual void EmptyItOut ();
    MOCK_METHOD0(EmptyItOut, void());
};

class MockCNF: public CNF {
public:
//	int GetSortOrders (OrderMaker &left, OrderMaker &right);
	MOCK_METHOD2(GetSortOrders, int(OrderMaker &left, OrderMaker &right));
//	bool MakeQuery(const OrderMaker *sortOrder, OrderMaker &query);
	MOCK_METHOD2(MakeQuery, bool(const OrderMaker &sortOrder, OrderMaker &query));
//	bool IsSortableAttribute(const int &attr);
	MOCK_METHOD1(IsSortableAttribute, bool(const int &attr));
//	void Print ();
	MOCK_METHOD0(Print, void());
//	void GrowFromParseTree (struct AndList *parseTree, Schema *leftSchema, Schema *rightSchema, Record &literal);
	MOCK_METHOD4(GrowFromParseTree, void(struct AndList *parseTree, Schema *leftSchema, Schema *rightSchema, Record &literal));
//	void GrowFromParseTree (struct AndList *parseTree, Schema *mySchema, Record &literal);
	MOCK_METHOD3(GrowFromParseTree, void(struct AndList *parseTree, Schema *mySchema, Record &literal));
};

class MockLinearScanner: public LinearScanner {
public:
//	virtual int GetNext(Record& rec);
	MOCK_METHOD1(GetNext, int(Record &rec));
};

class MockSelectPipe: public SelectPipe {
public:
//	virtual void Run (Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal);
	MOCK_METHOD4(Run, void(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal));
//	virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
//	virtual void Select(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal);
	MOCK_METHOD4(Select, void(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal));
//	virtual void Select(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, Record &rec);
	MOCK_METHOD6(Select, void(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, Record &rec));
};

class MockDuplicateRemoval: public DuplicateRemoval {
public:
//	virtual void Run (Pipe &inPipe, Pipe &outPipe, Schema &mySchema);
	MOCK_METHOD3(Run, void(Pipe &inPipe, Pipe &outPipe, Schema &mySchema));
//	virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
//	virtual void Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema);
	MOCK_METHOD3(Remove, void(Pipe &inPipe, Pipe &outPipe, Schema &mySchema));
//	virtual void Remove(Pipe &inPipe, Pipe &outPipe, Schema &mySchema, Record &rec, Record &prev, ComparisonEngine &comp, OrderMaker &order);
	MOCK_METHOD7(Select, void(Pipe &inPipe, Pipe &outPipe, Schema &mySchema, Record &rec, Record &prev, ComparisonEngine &comp, OrderMaker &order));
//	virtual int GetPageLimit();
	MOCK_METHOD0(GetPageLimit, int());
};

class MockProject: public Project {
public:
//	virtual void Run (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput)
	MOCK_METHOD5(Run, void(Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput));
//	virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
//	virtual void Work (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput);
	MOCK_METHOD5(Work, void((Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput)));
//	virtual void Work (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput, Record &rec);
	MOCK_METHOD6(Work, void(Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput, Record &rec));
};

class MockSelectFile: public SelectPipe {
public:
//	virtual void Run (Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal);
	MOCK_METHOD4(Run, void(Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal));
//	virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
//	virtual void Select(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal);
	MOCK_METHOD4(Select, void(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal));
//	virtual void Select(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, Record &rec);
	MOCK_METHOD6(Select, void(DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal, ComparisonEngine &comp, Record &rec));
};

class MockWriteOut: public WriteOut {
public:
//	virtual void Run (Pipe &inPipe, FILE *outFile, Schema &mySchema);
	MOCK_METHOD3(Run, void(Pipe &inPipe, FILE *outFile, Schema &mySchema));
//	virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
//	virtual void Write(Pipe &inPipe, FILE *outFile, Schema &mySchema);
	MOCK_METHOD3(Write, void(Pipe &inPipe, FILE *outFile, Schema &mySchema));
//	virtual void Write(Pipe &inPipe, FILE *outFile, Schema &mySchema, RawFile &file, Record &rec);
	MOCK_METHOD5(Write, void(Pipe &inPipe, FILE *outFile, Schema &mySchema, RawFile &file, Record &rec));
};

class MockFunction: public Function {
public:
//	void GrowFromParseTree (struct FuncOperator *parseTree, Schema &mySchema);
	MOCK_METHOD2(GrowFromParseTree, void(struct FuncOperator *parseTree, Schema &mySchema));
//	Type RecursivelyBuild (struct FuncOperator *parseTree, Schema &mySchema);
	MOCK_METHOD2(RecursivelyBuild, Type(struct FuncOperator *parseTree, Schema &mySchema));
//	void Print ();
	MOCK_METHOD0(Print, void());
//	Type Apply (Record &toMe, int &intResult, double &doubleResult);
	MOCK_METHOD3(Apply, Type(Record &toMe, int &intResult, double &doubleResult));
//	bool ReturnsInt();
	MOCK_METHOD0(ReturnsInt, bool());
};

class MockSum: public Sum {
public:
//	virtual void Run (Pipe &inPipe, Pipe &outPipe, Function &computeMe);
	MOCK_METHOD3(Run, void(Pipe &inPipe, FILE *outFile, Schema &mySchema));
//	virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
//	virtual void Work (Pipe &inPipe, Pipe &outPipe, Function &computeMe);
	MOCK_METHOD3(Work, void(Pipe &inPipe, Pipe &outPipe, Function &computeMe));
//	virtual void Work (Pipe &inPipe, Pipe &outPipe, Function &computeMe, Record &rec);
	MOCK_METHOD4(Work, void(Pipe &inPipe, Pipe &outPipe, Function &computeMe, Record &rec));
};

class MockRelation: public Relation {
public:
//	virtual bool Add(Record *rec) = 0;
	MOCK_METHOD1(Add, bool(Record *rec));
//	virtual bool GetNext(Record *&rec) = 0;
	MOCK_METHOD1(GetNext, bool(Record *&rec));
//	virtual void Reset() = 0;
	MOCK_METHOD0(Reset, void());
//	virtual void Clear() = 0;
	MOCK_METHOD0(Clear, void());
//	virtual int MemUsed();
	MOCK_METHOD0(MemUsed, int());
};

class MockInMemoryRelation: public InMemoryRelation {
public:
//	virtual bool Add(Record *rec) = 0;
	MOCK_METHOD1(Add, bool(Record *rec));
//	virtual bool GetNext(Record *&rec) = 0;
	MOCK_METHOD1(GetNext, bool(Record *&rec));
//	virtual void Reset() = 0;
	MOCK_METHOD0(Reset, void());
//	virtual void Clear() = 0;
	MOCK_METHOD0(Clear, void());
//	virtual int MemUsed();
	MOCK_METHOD0(MemUsed, int());
//	virtual void SetMemLimit(int limit);
	MOCK_METHOD1(SetMemLimit, void(int limit));
};

class MockFileRelation: public FileRelation {
public:
//	virtual bool Add(Record *rec) = 0;
	MOCK_METHOD1(Add, bool(Record *rec));
//	virtual bool Add(Pipe &pipe) = 0;
	MOCK_METHOD1(Add, bool(Pipe &pipe));
//	virtual bool Add(Relation &rel) = 0;
	MOCK_METHOD1(Add, bool(Relation &rel));
//	virtual bool GetNext(Record *&rec) = 0;
	MOCK_METHOD1(GetNext, bool(Record *&rec));
//	virtual void Reset() = 0;
	MOCK_METHOD0(Reset, void());
//	virtual void Clear() = 0;
	MOCK_METHOD0(Clear, void());
//	virtual int MemUsed();
	MOCK_METHOD0(MemUsed, int());
};

class MockJoinRelation: public JoinRelation {
public:
//	virtual bool Add(Record *rec) = 0;
	MOCK_METHOD1(Add, bool(Record *rec));
//	virtual bool Populate(Pipe &in);
	MOCK_METHOD1(Populate, bool(Pipe &in));
//	virtual bool GetNext(Record *&rec) = 0;
	MOCK_METHOD1(GetNext, bool(Record *&rec));
//	virtual void Reset() = 0;
	MOCK_METHOD0(Reset, void());
//	virtual void Clear() = 0;
	MOCK_METHOD0(Clear, void());
//	virtual int MemUsed();
	MOCK_METHOD0(MemUsed, int());
//	virtual bool IsMemoryMode();
	MOCK_METHOD0(IsMemoryMode, bool());
};

class MockGroupBy: public GroupBy {
public:
	// void Run (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe);
	MOCK_METHOD4(Run, void(Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe));
	// virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
	// virtual void Work (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe);
	MOCK_METHOD4(Work, void(Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe));
	// virtual void Work (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe, Record &rec, Record &prev, ComparisonEngine &comp);
	MOCK_METHOD8(Work, void(Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe,
		Record &rec, Record &prev, Record& mergeInto, ComparisonEngine &comp));
	// virtual int GetPageLimit();1
	MOCK_METHOD0(GetPageLimit, int());
};

class MockJoin: public Join{
public:
//	virtual void Use_n_Pages (int n);
	MOCK_METHOD1(Use_n_Pages, void(int n));
//	virtual void Run (Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal);
	MOCK_METHOD5(Run, void(Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal));
};

class MockExpression : public Expression {
public:
	// virtual bool Combine(Expression &e) = 0;
	MOCK_METHOD1(Combine, bool(Expression& e));

	// virtual double Tuples() = 0;
	MOCK_METHOD0(Tuples, double());

	// virtual double Numerator() = 0;
	MOCK_METHOD0(Numerator, double());

	// virtual double Denominator() = 0;
	MOCK_METHOD0(Denominator, double());
};

class MockOpNode : public OpNode {
public:
	MOCK_METHOD0(GetID, int());
	MOCK_METHOD0(GetSchema, const Schema*());

};

class MockSQL: public SQL {

//	virtual const struct FuncOperator* Function() const;
	MOCK_METHOD0(Function, const FuncOperator*());
//	virtual const Statistics& GetStatistics() const;
	MOCK_METHOD0(GetStatistics, const Statistics&());
//	virtual void Parse(const std::string &sql);
	MOCK_METHOD1(Parse, void(const std::string));
//	virtual void Parse();
	MOCK_METHOD0(Parse, void());
//	virtual void GetWhere(SelectMap &selects, JoinMap &joins);
	MOCK_METHOD2(GetWhere, void(SelectMap &selects, JoinMap &joins));
//	virtual void GetGroup(std::vector<RelAttPair> &pairs);
	MOCK_METHOD1(GetGroup, void(std::vector<RelAttPair> &pairs));
//	virtual void GetSelect(std::vector<RelAttPair> &pairs);
	MOCK_METHOD1(GetSelect, void(std::vector<RelAttPair> &pairs));
//	virtual void GetTables(std::vector<RelAliasPair> &pairs);
	MOCK_METHOD1(GetTables, void(std::vector<RelAliasPair> &pairs));
//	virtual void GetFunctionAttributes(std::vector<RelAttPair> &pair);
	MOCK_METHOD1(GetFunctionAttributes, void(std::vector<RelAttPair> &pair));
//	virtual bool DistinctAggregate();
	MOCK_METHOD0(DistinctAggregate, bool());
//	virtual bool DistinctSelect();
	MOCK_METHOD0(DistinctSelect, bool());
//	virtual void ParseWhere(struct AndList *where, SelectMap &selects, JoinMap &joins);
	MOCK_METHOD3(ParseWhere, void(struct AndList *where, SelectMap &selects, JoinMap &joins));
//	virtual void ParseNameList(struct NameList *list, std::vector<RelAttPair> &pair);
	MOCK_METHOD2(ParseNameList, void(struct NameList *list, std::vector<RelAttPair> &pair));
//	virtual void ParseFuncOperator(FuncOperator *func, std::vector<RelAttPair> &pair);
	MOCK_METHOD2(ParseFuncOperator, void(FuncOperator *func, std::vector<RelAttPair> &pair));
//	virtual void ParseTableList(TableList *list, std::vector<RelAliasPair> &pairs);
	MOCK_METHOD2(ParseTableList, void(TableList *list, std::vector<RelAliasPair> &pairs));
};

class MockJoinOptimizer: public JoinOptimizer {

//   void Optimize(unordered_map<string, AndList*> &selects,
//                       unordered_map<string, unordered_map<string, AndList*> > &joins,
//                       Statistics& stats,
//                       vector<string> &rels,
//                       vector<TupleCount> &counts);
   MOCK_METHOD5(Optimize, void(unordered_map<string, AndList*> &selects,
                       unordered_map<string, unordered_map<string, AndList*> > &joins,
                       Statistics& stats,
                       vector<string> &rels,
                       vector<TupleCount> &counts));
};

#endif
