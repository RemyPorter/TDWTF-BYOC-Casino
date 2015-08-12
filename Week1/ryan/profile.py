import roulette
import random

ITERATIONS = 100000
SLOTS = roulette.SLOTS

r = roulette.Roulette()
best_guess = [str(x) for x in xrange(SLOTS-1)]
best_guess.append('00')

recent1 = 'NA'
recent2 = 'NA'
doubles = 0
triples = 0
occurances = {}
for guess in best_guess:
	occurances[guess] = 0

wins = 0.0
losses = 0.0
random_wins = 0.0
random_losses = 0.0
for i in xrange(ITERATIONS):
    bad_guess = str(random.randrange(0, SLOTS))
    if bad_guess == '37':
        bad_guess = '00'
    result = r.random()
    if result == best_guess[0]:
        wins += 1
    else:
        losses += 1
    if result == bad_guess:
        random_wins += 1
    else:
        random_losses += 1
    occurances[result] += 1
    if result == recent1:
        doubles += 1
        if result == recent2:
            triples +=1
            print 'triple:', result, ':', history[x]
    recent2 = recent1
    recent1 = result
    best_guess.remove(result)
    best_guess.append(result)
    # print probabilities
print '00 :',occurances['00']
for x in xrange(SLOTS-1):
    print x,':',occurances[str(x)]
print ''
print 'doubles:', doubles
print 'triples:', triples
print ''
print 'Random betting'
print 'wins:', random_wins
print 'losses', random_losses
print 'odds = 1:' + str(random_losses/random_wins)
print ''
print 'Informed betting'
print 'wins:', wins
print 'losses', losses
print 'odds = 1:' + str(losses/wins)


