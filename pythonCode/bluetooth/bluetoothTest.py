# BUETOOTH TEST


file=open('/dev/rfcomm0','r')

for row in file:
	print(row)

file.close()