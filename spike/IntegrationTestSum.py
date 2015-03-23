import string

file = open('data/10M/lineitem.tbl', 'r')
orderkey = 0 
partkey = 0
suppkey = 0
linenumber = 0 
quantity = 0.0
extendedprice = 0.0
discount = 0.0
tax = 0.0
str

for line in file:
	str = line.split('|')
	orderkey += int(str[0]) 
	partkey += int(str[1]) 
	suppkey += int(str[2]) 
	linenumber += int(str[3]) 
	quantity += float(str[4])
	extendedprice += float(str[5]) 
	discount += float(str[6]) 
	tax += float(str[7]) 

print "l_orderkey sum:", orderkey
print "l_partkey sum:", partkey
print "l_suppkey sum:", suppkey
print "l_linenumber sum:", linenumber
print "l_quantity sum:", quantity
print "l_extendedprice sum:", extendedprice
print "l_discount sum:", discount
print "l_tax sum:", tax
