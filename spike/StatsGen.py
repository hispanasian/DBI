from sets import Set
from sys import argv

def genStats(filename, catalogName, relname):
	with open(filename) as file:
		lines = file.readlines()
		print "%s=%s" % (relname, len(lines))
		vals = []
		for i in range(0, len(lines[0].split("|"))-1):
			vals.append(Set())

		for l in lines :
			tokens = l.split("|")
			for i in range(0, len(tokens)-1):
				vals[i].add(tokens[i])
		with open(catalogName) as catalog:
			# find the relation in the catalog
			catlines = catalog.readlines()
			for i in range(0, len(catlines)):
				# print catlines[i].strip()
				# print catlines[i+1].strip()
				if catlines[i].strip() == "BEGIN" and catlines[i+1].strip() == relname :
					for k in range(0, len(vals)):
						print "%s.%s=%s" % (relname, catlines[i+3+k].strip().split()[0], len(vals[k]))
					exit()

genStats(argv[1], argv[2], argv[3]);