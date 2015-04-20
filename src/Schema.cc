#include "../include/Schema.h"
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int Schema :: Find (const char *attName) const {

	for (int i = 0; i < numAtts; i++) {
		if (!myAtts[i].name.compare(attName) ||
				!string(myAtts[i].relation).append(".").append(myAtts[i].name).compare(attName) ) {
			return i;
		}
	}

	// if we made it here, the attribute was not found
	return -1;
}

bool Schema :: operator == (const Schema &other) const {
	if(numAtts != other.numAtts) {
		for(int i = 0; i < numAtts; i++) {
			if(myAtts[i].relation != other.myAtts[i].relation ||
					myAtts[i].name != other.myAtts[i].name ||
					myAtts[i].myType != other.myAtts[i].myType)
				return false;
		}
	}
	return true;
}

int Schema :: Find (const char *relName, const char *attName) const {
	return Find(string(relName).append(".").append(attName).c_str());
}

Type Schema :: FindType (const char *attName) const {

	for (int i = 0; i < numAtts; i++) {
		if (!myAtts[i].name.compare(attName) ||
				!string(myAtts[i].relation).append(".").append(myAtts[i].name).compare(attName) ) {
			return myAtts[i].myType;
		}
	}

	// if we made it here, the attribute was not found
	return Int;
}

Type Schema :: FindType (const char *relName, const char *attName) const {
	return FindType(string(relName).append(".").append(attName).c_str());
}

int Schema :: GetNumAtts () const {
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
					throw invalid_argument("Could not find the schema for the specified relation.\n");
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
			throw invalid_argument("Received a bad attribute type");
			exit (1);
		}
	}

	fclose (foo);
}

Schema :: Schema (char *fpath, int num_atts, Attribute *atts) {
	if(fpath != NULL) fileName = strdup (fpath);
	else fileName = NULL;
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
			cout << "Bad attribute type for " << atts[i].relation << "." << atts[i].name
					<< " with " << atts[i].myType << "\n";
			throw invalid_argument("Received a bad attribute type");
			delete [] myAtts;
			exit (1);
		}
		myAtts[i].name = atts[i].name;
		myAtts[i].relation = atts[i].relation;
	}
}

Schema :: Schema (const Schema &copyMe) {
	fileName = NULL;
	myAtts = NULL;
	Copy(copyMe);
}

Schema :: Schema (const Schema &copyMe, const vector<RelAttPair> &pairs) {
	fileName = NULL;
	myAtts = NULL;
	Filter(copyMe, pairs);
}

Schema :: Schema (const Schema *left, const Schema *right) {
	myAtts = NULL;
	Join(left, right);
}

Schema :: Schema (const Schema &left, const Schema &right) {
	myAtts = NULL;
	Join(&left, &right);
}

void Schema :: Copy (const Schema &copyMe) {
	fileName = strdup(copyMe.fileName);
	numAtts = copyMe.numAtts;
	delete myAtts;
	myAtts = new Attribute[numAtts];

	for(int i = 0; i < copyMe.numAtts; i++) {
		myAtts[i].name = copyMe.myAtts[i].name;
		myAtts[i].relation = copyMe.myAtts[i].relation;
		myAtts[i].myType = copyMe.myAtts[i].myType;
	}
}

void Schema :: Join  (const Schema *left, const Schema *right) {
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
		myAtts[i + loff].name = right->myAtts[i].name;
		myAtts[i + loff].relation = right->myAtts[i].relation;
		myAtts[i + loff].myType = right->myAtts[i].myType;
	}
}

void Schema :: Filter (const Schema &copyMe, const vector<RelAttPair> &pairs) {
	fileName = NULL;
	numAtts = pairs.size();
	delete myAtts;
	myAtts = new Attribute[numAtts];

	int index = -1;
	Type type;

	for(int i = 0; i < numAtts; i++) {
		string temp = pairs[i].Relation();
		index = copyMe.Find(pairs[i].Relation().c_str(), pairs[i].Attribute().c_str());
		type = copyMe.FindType(pairs[i].Relation().c_str(), pairs[i].Attribute().c_str());

		if(index == -1) {
			cerr << "Failed to find " << pairs[i].Relation() << "." << pairs[i].Attribute()
					<< " during filter." << endl;
			throw invalid_argument("Unknown Relation/Attribute pair found (Schema(Schema, vector<RelAttPair>))");
		}
		myAtts[i].name = pairs[i].Attribute();
		myAtts[i].relation = pairs[i].Relation();
		myAtts[i].myType = type;
	}
}

void Schema :: SetRelation(const char *relation) {
	for(int i = 0; i < numAtts; i++) {
		myAtts[i].relation = string(relation);
	}
}

string Schema :: ToString() const {
	return ToString("");
}

string Schema :: ToString(string prefix) const {
	string temp;
	for(int i = 0; i < numAtts; i++) {
		temp.append(prefix);
		// Don't ignore the relation
		if(myAtts[i].relation.compare("") != 0) temp.append(myAtts[i].relation).append(".");
		temp.append(myAtts[i].name).append(": ");

		if(myAtts[i].myType == Int) temp.append("int");
		else if(myAtts[i].myType == Double) temp.append("double");
		else if(myAtts[i].myType == String) temp.append("string");
		else throw runtime_error("Unknown type found (Schema::ToString)");
		temp.append("\n");
	}
	return temp;
}

Schema :: ~Schema () {
	free(fileName);
	delete [] myAtts;
	myAtts = 0;
}

