#include "../include/GenericDBFile.h"
#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include <iostream>
#include <string.h>
#include <stdio.h>

GenericDBFile::GenericDBFile(): file(file), rfile(rfile), config(config), comp(comp) {

}

GenericDBFile::GenericDBFile(File &_file, RawFile &_rfile, DBConfig &_config,
		ComparisonEngine &_comp): file(_file), rfile(_rfile), config(_config), comp(_comp) {

}

GenericDBFile::~GenericDBFile() {

}

int GenericDBFile::GetLength() {
	off_t zero = 0;
	return max(zero, file.GetLength()-1);
}
