#! /usr/bin/python

for i in range(8):
	for j in range(i+1):
		if i % 2 == 0:
			print i-j, j
		else:
			print j, i-j

for j in range(7):
	for i in range(7,j,-1):
		if j % 2 == 0:
			print i, 8+j-i
		else:
			print 8+j-i, i
