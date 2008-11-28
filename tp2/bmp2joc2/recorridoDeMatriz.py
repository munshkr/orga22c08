#! /usr/bin/python

matriz = [[125, -1, 0, 0, 0, 0, 0, 0],
[0, 1, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 0],
[0, 0, 0, 0, 0, 0, 0, 25704]
]


print "Matriz:"
for h in range(8):
	# matriz[h] = range(1,9) #si quieren ver algo que comprime POCO, sacar el numeral de principio de linea
	print matriz[h]

print "Stream Codificado: "
codStream = []
cantCeros = 0
primero = True
for i in range(8):
	for j in range(i+1):
		if i % 2 == 0:
			row = i-j
			col = j
		else:
			row = j
			col = i-j
		valor = matriz[row][col]
		if valor != 0:
			if not(primero):
				codStream.append(cantCeros),
				print cantCeros,
			else:
				primero = False
			codStream.append(valor)
			print valor,
			cantCeros = 0
		else:
			cantCeros = cantCeros + 1
for j in range(7):
	for i in range(7,j,-1):
		if j % 2 == 0:
			row = i
			col = 8+j-i
		else:
			row = 8+j-i
			col = i
		valor = matriz[row][col]
		if valor != 0:
			codStream.append(cantCeros)
			codStream.append(valor)
			print cantCeros, valor,
			cantCeros = 0
		else:
			cantCeros = cantCeros + 1
codStream.append(0)
codStream.append(0)
print 0, 0

print "\nStream Codificado:"
print codStream

print "Matriz Decodificada:"
matriz=[]
for i in range(8):
	matriz.append([1]*8)
matriz[0][0] = codStream[0]
cantCeros = codStream[1]
streamAct = 2
primero = True
for i in range(8):
	for j in range(i+1):
		if i % 2 == 0:
			row = i-j
			col = j
		else:
			row = j
			col = i-j
		if cantCeros > 0:
			matriz[row][col] = 0
			cantCeros = cantCeros - 1
		else:
			if primero:
				primero = False
			else:
				matriz[row][col] = codStream[streamAct]
				streamAct = streamAct+1
				cantCeros = codStream[streamAct]
				streamAct = streamAct+1

for j in range(7):
	for i in range(7,j,-1):
		if j % 2 == 0:
			row = i
			col = 8+j-i
		else:
			row = 8+j-i
			col = i
		if cantCeros > 0:
			matriz[row][col] = 0
			cantCeros = cantCeros - 1
		else:
			matriz[row][col] = codStream[streamAct]
			streamAct = streamAct+1
			cantCeros = codStream[streamAct]
			streamAct = streamAct+1

for h in range(8):
	print matriz[h]