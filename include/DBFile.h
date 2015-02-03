#ifndef DBFILE_H
#define DBFILE_H

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "RawFile.h"
#include "DBConfig.h"
#include <sys/stat.h>

typedef enum {heap, sorted, tree} fType;

// stub DBFile header..replace it with your own DBFile.h 

class DBFile {
friend class DBFileTest;
private:
	off_t curPage;
	File &file;
	File myFile;
	Page *page;
	RawFile &rfile;
	RawFile myRFile;
	DBConfig &config;
	DBConfig myConfig;
	bool recordAdded;
	bool recordRead;
	DBFile(File &file, RawFile &rfile, DBConfig &config); // Strictly for Testing.
public:
	DBFile ();
    virtual ~DBFile();

    /**
     * Creates a file at fpath of file_type heap, sorted, or tree. It will also create a file to
     * store meta-data located at filepath.header.
     * @param fpath		The path to the file that should be created.
     * @param file_type	The type of the file as defined by ftype
     * @param startup	Unknown
     * @return			1 if successful and 0 if failure.
     */
	virtual int Create (char *fpath, fType file_type, void *startup);

	/**
	 * Opens the file and associated header located at fpath and fpath.header respectivel. This
	 * function does not load any data from the file into memory.
	 * @param fpath	The path to the DBFile
	 * @return 		1 if successful and 0 if failure.
	 */
	virtual int Open (char *fpath);

	/**
	 * Closes the file held by this object.
	 * @return 1 if successful and 0 if failure.
	 */
	virtual int Close ();

	/**
	 * Appends the data located at loadpath to memory.
	 * @param myschema	The schema of the file being loaded to memory
	 * @param loadpath	The path to the file being loaded
	 */
	virtual void Load (Schema &myschema, char *loadpath);

	/**
	 * Moves the pointer from the current record to the first record of the first page.
	 */
	virtual void MoveFirst ();

	/**
	 * Adds addme to a file and consumes addme.
	 * @param addme	The Record being added to DBFile
	 */
	virtual void Add (Record &addme);

	/**
	 * Puts the current record in fetchme and moves the pointer to the next record. Upon use, the
	 * record referenced by fetchme is consumed and the new record in fetchme is removed from DB
	 * File.
	 * @param fetchme	A reference that will be used to return the current Record
	 * @return			0 if there is no valid record to be returned.
	 */
	virtual int GetNext (Record &fetchme);

	/**
	 * Looks for the next record that passes applyMe when applied between a Record in DBFile and
	 * literal. This function increments the pointer accordingly.
	 * @param fetchme	A reference that will be used to return the current Record
	 * @param applyMe	The CNF that will be applied between the Record(s) in DBFile and literal
	 * @param literal	The Record that will be compared with the Records in DBFile.
	 * @return			0 if there is no valid record to be returned.
	 */
	virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal);
};

/**
 * Checks if a file located at path exists. This code was stolen from:
 * http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
 * @param name	The name of the file
 * @return True if the file exists
 */
inline bool FileExists(const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

#endif
