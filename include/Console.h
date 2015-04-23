/*
 * Console.h
 *
 *  Created on: Apr 23, 2015
 *      Author: cvasquez
 */

#ifndef INCLUDE_CONSOLE_H_
#define INCLUDE_CONSOLE_H_

#include "SQL.h"
#include "SQLEngine.h"
#include "Defs.h"
#include <string>

/**
 * Console is a CLI for SQLEngine that will take input and modify SQLEngine according
 * to the input.
 */
class Console {
public:
	SQLEngine &engine;
	Output_Type output = None;
	FILE *file;

public:
	Console(SQLEngine &engine);
	virtual ~Console();

	/**
	 * Starts the console. It will immediately being reading input
	 */
	void Start();

	void CreateTable(SQL *sql);

	void Insert(SQL *sql);

	void DropTable(SQL *sql);

	void Select(SQL *sql);

	void QueryPlan(SQL *sql);

	void SetOutput(SQL *sql);
};

#endif /* INCLUDE_CONSOLE_H_ */
