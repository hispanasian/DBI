#include <iostream>
#include "Console.h"
#include "SQLEngine.h"

using namespace std;

void setupRelation(char* rel, char* catalogFName, const string& dbLocation,
	RelationData& relations, Statistics& stats)  {
	// first load the schema from the file
	Schema* s = new Schema(catalogFName, rel);
	// now use this to populate relations and stats
	relations.Insert(string(rel), dbLocation, string(catalogFName), *s);
	// stats.AddRel(rel, 100);
	// for(int i = 0; i < s->GetNumAtts(); ++i) {
	// 	stats.AddAtt(rel, s->GetAtts()[i].name.c_str(), -1);
	// }
}


int main(int argc, char const *argv[]) {
	char* catalogFName = "data/catalog";	
	string dbLocation = "data/DB/10M/";
	Statistics stats = Statistics();
	stats.Read("stats.txt");
	RelationData relations = RelationData();
	char* rels[] = {"supplier", "nation", "region", "customer", "part", "partsupp", "orders", "lineitem"};


	// add some relations
	for(int i = 0; i < 8; ++i) {
		setupRelation(rels[i], catalogFName, dbLocation+string(rels[i])+".bin", relations, stats);
	}

	SQLEngine engine = SQLEngine(stats, relations, dbLocation, string(catalogFName));

	Console console = Console(engine);
	console.Start();

	return 0;
}