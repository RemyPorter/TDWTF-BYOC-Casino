import roulette

PER_ROW = 20
ROWS = 10

r = roulette.Roulette()

for i in xrange(ROWS):
	row = []
	for j in xrange(PER_ROW):
		row.append(r.random())
	print '  '.join('{0: >2}'.format(x) for x in row)