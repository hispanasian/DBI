#include "../include/Schema.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int Schema :: Find (const char *attName) {

	for (int i = 0; i < numAtts; i++) {
		if (!myAtts[i].name.compare(attName) ||
				!string(myAtts[i].relation).append(".").append(myAtts[i].name).compare(attName) ) {
			return i;
		}
	}

	// if we made it here, the attribute was not found
	return -1;
}

int Schema :: Find (const char *relName, const char *attName) {
	return Find(string(relName).append(".").append(attName).c_str());
}

Type Schema :: FindType (const char *attName) {

	for (int i = 0; i < numAtts; i++) {
		if (!myAtts[i].name.compare(attName) ||
				!string(myAtts[i].relation).append(".").append(myAtts[i].name).compare(attName) ) {
			return myAtts[i].myType;
		}
	}

	// if we made it here, the attribute was not found
	return Int;
}

Type Schema :: FindType (const char *relName, const char *attName) {
	return FindType(string(relName).append(".").append(attName).c_str());
}

int Schema :: GetNumAtts () {
	return numAtts;
}

Attribute *Schema :: GetAtts () {
	return myAtts;
}

Schema :: Schema (char *fName, char *relName) {

	FILE *foo = fopen (fName, "r");
	
	// this is enough space to hold any tokens
	char space[200];

	fscanf (foo, "%s", space);
	int totscans = 1;

	// see if the file starts with the correct keyword
	if (strcmp (space, "BEGIN")) {
		cout << "Unfortunately, this does not seem to be a schema file.\n";
		exit (1);
	}	
		
	while (1) {

		// check to see if this is the one we want
		fscanf (foo, "%s", space);
		totscans++;
		if (strcmp (space, relName)) {

			// it is not, so suck up everything to past the BEGIN
			while (1) {

				// suck up another token
				if (fscanf (foo, "%s", space) == EOF) {
					cerr << "Could not find the schema for the specified relation.\n";
					exit (1);
				}

				totscans++;
				if (!strcmp (space, "BEGIN")) {
					break;
				}
			}

		// otherwise, got the correct file!!
		} else {
			break;
		}
	}

	// suck in the file name
	fscanf (foo, "%s", space);
	totscans++;
	fileName = strdup (space);

	// count the number of attributes specified
	numAtts = 0;
	while (1) {
		fscanf (foo, "%s", space);
		if (!strcmp (space, "END")) {
			break;		
		} else {
			fscanf (foo, "%s", space);
			numAtts++;
		}
	}

	// now actually load up the schema
	fclose (foo);
	foo = fopen (fName, "r");

	// go past any un-needed info
	for (int i = 0; i < totscans; i++) {
		fscanf (foo, "%s", space);
	}

	// and load up the schema
	myAtts = new Attribute[numAtts];
	for (int i = 0; i < numAtts; i++ ) {

		// read in the attribute name
		fscanf (foo, "%s", space);	
		myAtts[i].name = strdup (space);

		// read in the attribute type
		fscanf (foo, "%s", space);
		if (!strcmp (space, "Int")) {
			myAtts[i].myType = Int;
		} else if (!strcmp (space, "Double")) {
			myAtts[i].myType = Double;
		} else if (!strcmp (space, "String")) {
			myAtts[i].myType = String;
		} else {
			cout << "Bad attribute type for " << myAtts[i].name << "\n";
			exit (1);
		}
	}

	fclose (foo);
}

Schema :: Schema (char *fpath, int num_atts, Attribute *atts) {
	fileName = strdup (fpath);
	numAtts = num_atts;
	myAtts = new Attribute[numAtts];
	for (int i = 0; i < numAtts; i++ ) {
		if (atts[i].myType == Int) {
			myAtts[i].myType = Int;
		}
		else if (atts[i].myType == Double) {
			myAtts[i].myType = Double;
		}
		else if (atts[i].myType == String) {
			myAtts[i].myType = String;
		} 
		else {
			cout << "Bad attribute type for " << atts[i].myType << "\n";
			delete [] myAtts;
			exit (1);
		}
		myAtts[i].name = atts[i].name;
		myAtts[i].relation = atts[i].relation;
	}
}

Schema :: Schema (const Schema &copyMe) {
	fileName = NULL;
	numAtts = copyMe.numAtts;
	myAtts = new Attribute[numAtts];

	for(int i = 0; i < copyMe.numAtts; i++) {
		myAtts[i].name = copyMe.myAtts[i].name;
		myAtts[i].relation = copyMe.myAtts[i].relation;
		myAtts[i].myType = copyMe.myAtts[i].myType;
	}
}

Schema :: Schema (Schema *left, Schema *right) {
	myAtts = NULL;
	Join(left, right);
}

void Schema :: Join  (Schema *left, Schema *right) {
	fileName = NULL;
	numAtts = left->numAtts + right->numAtts;
	delete myAtts;
	myAtts = new Attribute[numAtts];

	for(int i = 0; i < left->numAtts; i++) {
		myAtts[i].name = left->myAtts[i].name;
		myAtts[i].relation = left->myAtts[i].relation;
		myAtts[i].myType = left->myAtts[i].myType;
	}

	int loff = left->numAtts; // offset from left schema
	for(int i = 0; i < right->numAtts; i++) {
		myAtts[i].name = right->myAtts[i].name;
		myAtts[i].relation = right->myAtts[i].relation;
		myAtts[i + loff].myType = right->myAtts[i].myType;
	}
}

void Schema :: SetRelation(const char *relation) {
	for(int i = 0; i < numAtts; i++) {
		myAtts[i].relation = string(relation);
	}
}

Schema :: ~Schema () {
	free(fileName);
	delete [] myAtts;
	myAtts = 0;
}

