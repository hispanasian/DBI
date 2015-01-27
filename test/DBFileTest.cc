#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockClasses.h"
#include "DBFile.h"

using ::testing::_;

class DBFileTest: public ::testing::Test {
public:
	DBFile file;
	off_t CurPage();
	File GetFile();

	int Create (char *fpath, fType file_type, void *startup, File file);
	int Open (char *fpath, File file);
	int Close (File file);
	void Load (Schema &myschema, char *loadpath, File file);
	void MoveFirst (File file);
	void Add (Record &addme, File file);
	int GetNext (Record &fetchme, File file);
	int GetNext (Record &fetchme, CNF &cnf, Record &literal, File file);
};

int DBFileTest::Create (char *f_path, fType f_type, void *startup, File file) {
	return this->file.Create(f_path, f_type, startup, file);
}

void DBFileTest::Load (Schema &f_schema, char *loadpath, File file) {
	this->file.Load(f_schema, loadpath, file);
}

int DBFileTest::Open (char *f_path, File file) {
	return this->file.Open(f_path, file);
}

void DBFileTest::MoveFirst (File file) {
	this->file.MoveFirst(file);
}

int DBFileTest::Close (File file) {
	return this->file.Close(file);
}

void DBFileTest::Add (Record &rec, File file) {
	this->file.Add(rec, file);
}

int DBFileTest::GetNext (Record &fetchme, File file) {
	return this->file.GetNext(fetchme, file);
}

int DBFileTest::GetNext (Record &fetchme, CNF &cnf, Record &literal, File file) {
	return this->file.GetNext(fetchme, cnf, literal, file);
}

off_t DBFileTest::CurPage() { return file.curPage; }

File DBFileTest::GetFile() { return file.file; }

TEST_F(DBFileTest, Create1) {

}
