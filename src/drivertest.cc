#include <iostream>
#include "Console.h"
#include "SQLEngine.h"

using namespace std;

int main(int argc, char const *argv[])
{
	Statistics stats = Statistics();
	// stats.AddRel("nation", 25);
	// stats.AddAtt("nation", "n_nationkey", -1);
	// stats.AddAtt("nation", "n_name", -1);
	// stats.AddAtt("nation", "n_regionkey", -1);
	// stats.AddAtt("nation", "n_comment", -1);

	RelationData relations = RelationData();
	SQLEngine engine = SQLEngine(stats, relations, "data/DB/10M/", "catalog");

	Console console = Console(engine);
	console.Start();

	return 0;
}