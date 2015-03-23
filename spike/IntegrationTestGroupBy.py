import string

file = open('data/10M/lineitem.tbl', 'r')
dic = dict(); # dictionary of groups
str

for line in file:
	str = line.split('|')
	group = str[1] + '|' + str[2] # a group of partkey and suppkey
	dic[group] = 1

print "the number of unique (partkey,suppkey) pairs is:", len(dic.keys())
