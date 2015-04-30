/*
 * Console.cc
 *
 *  Created on: Apr 23, 2015
 *      Author: cvasquez
 */

#include "Console.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <vector>

using namespace std;

Console::Console(SQLEngine &_engine): engine(_engine), output(None), file(NULL)  {}

Console::~Console() {
	delete file;
}

void Console::Start() {
	SQL *sql = engine.CreateSQL();
	bool quit = false;
	Output_Type output = None;
	FILE *file;
	string buff;
	while(!quit) {
		buff.clear();
		getline(cin, buff, ';');
		cout << buff << endl;
		buff.append(";");
		try {
			switch(sql->Parse(buff)) {
			case Create_Table: CreateTable(sql);
				cout << "Create Table" << endl;
				break;
			case Insert_Into: Insert(sql);
				cout << "Insert" << endl;
				break;
			case Drop_Table: DropTable(sql);
				cout << "Drop Table" << endl;
				break;
			case Set_Output: SetOutput(sql);
				cout << "Set Output" << endl;
				break;
			case Select: Query(sql);
				break;
			case Quit: quit = true;
				cout << "Bye!" << endl;
				break;
			default: cerr << "unknown command" << endl;
				break;
			}
		}
		catch (invalid_argument &e) {
			cerr << "An error occurred: " << e.what() << endl;
		}

		sql = engine.CreateSQL();
	}
}

void Console::CreateTable(SQL *sql) {
	vector<AttTypePair> *atts = new vector<AttTypePair>;
	vector<string> *order = new vector<string>;
	string tableName;
	DB_Type type;

	type = sql->GetCreateTable(*atts, *order, tableName);
	engine.CreateTable(sql, atts, order, tableName, type);
}

void Console::Insert(SQL *sql) {
	string file;
	string table;

	sql->GetInsertInto(file, table);
	engine.Insert(sql, file, table);
}

void Console::DropTable(SQL *sql) {
	string table = sql->GetDropTable();
	engine.DropTable(sql, table);
}

void Console::Query(SQL *sql) {
	if(output == None) {
		// Simply print the Query plan to the screen
		cout << "QueryPlan" << endl;
		cout << engine.QueryPlan(sql) << endl;
	}
	else {
		// Run the query
		cout << "Query" << endl;
		engine.Query(sql, file);
	}
}

void Console::SetOutput(SQL *sql) {
	string fpath;
	output = sql->GetSetOutput(fpath);

	if(file != NULL && file != stdout) fclose(file);
	delete file;
	file = NULL;
	switch (output) {
		file = fopen(fpath.c_str(), "a");
	case Stdout:
		file = stdout;
		break;
	case File_Path:
		break;
	case None:
		// Do nothing
		break;
	default:
		// Do nothing
		break;
	}

	delete sql;
}
