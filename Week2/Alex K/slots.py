import random
import sys
import threading
import time

NUM_TOKENS = 10 # Number of valid tokens on the wheels
DELAY = .3 # Time to pause between each random number we spin
QUIET = False # Set quiet = True to squelch prints (for testing)
MESSAGE = "Press enter to play, type 'quit' to quit, or 'test' to self-test."

# These are the values displayed on our wheels.
a = 0
b = 0
c = 0

# Returns a fair random number on the wheel.
def rndnum():
	return random.randint(0,NUM_TOKENS-1)

# A print helper method that doesn't print in quiet mode.
def _print(text):
	if not QUIET: print(text)

# Displays the wheel to the console.
def printWheel(a, b, c):
	sa = str(a).ljust(3,' ')
	sb = str(b).ljust(3,' ')
	sc = str(c).ljust(3,' ')

	_print(sa + sb + sc)	

# This displays the wheels spinning to their target.
# It randomly switches any wheels that are not specified as None,
# finally locking them into their specified value.
# Wheels that are specified as None keep their current value.
def spin_wheels(_a, _b, _c):
	global a
	global b
	global c

	# Spin them randomly a few times
	for i in range(0, 2):
		time.sleep(DELAY);

		if (_a != None): a = rndnum()
		if (_b != None): b = rndnum()
		if (_c != None): c = rndnum()
		
		printWheel(a, b, c)

	# And finally lock the desired ones into position.
	if (_a != None): a = _a
	if (_b != None): b = _b
	if (_c != None): c = _c

	time.sleep(DELAY);
	printWheel(a, b, c)

# Play a round of slots. 
def playSlots():
	global a
	global b
	global c

	_print("Slots started.")

	# Initialize all wheels to fair random numbers to start off.
	a = b = c = rndnum()

	# Spin all the wheels and lock in our first number
	spin_wheels(a, rndnum(), rndnum())

	# The second wheel locks in; there's a 2/3 + 1/n chance of it being x!
	if (random.randint(0,2) > 0):
		b = a
	else:
		b = rndnum()

	# Keep spinning b,c and lock in 'b'
	spin_wheels(None, b, rndnum())

	# The final wheel locks into place. 
	# Here, there's only a 1/3n chance of equaling x!
	c = rndnum()
	while (c == a and random.randint(0,2) > 0):
		c = rndnum()

	# Keep spinning and lock in 'c'
	spin_wheels(None, None, c)

	# The game is finished. Check results. 
	# Total probability 
	# = (2/3+1/n) * (1/3n) 
	# = 2/9n + 1/3nn 
	# (2.5% chance for 10 tokens)

	if (a == b == c):
		_print("You win!")
	else:
		_print("Better luck next time")

	_print(MESSAGE)
	return (a == b == c)

# This function performs a self-test of the system. 
#It runs a large number of games, and reports the outcomes.
def selfTest():
	# Reduce the delay to zero in the interest of time.
	global DELAY
	old_delay = DELAY
	DELAY = 0

	print "Beginning self-test sequence..."

	# Silence regular output.
	global QUIET
	QUIET = True

	# Try 10,000 runs. 
	total = 10000
	num_won = 0
	for i in range(0, total):
		if playSlots():
			num_won = num_won + 1

	print "Self test complete. %d games won out of %d" % (num_won, total)

	# return the delay constant to its real value.
	DELAY = old_delay
	QUIET = False

# Initialize the random number generator.
random.seed()
print MESSAGE

while True:
	s = raw_input()
	if (s == 'test'):
		# Warning! self-test is blocking, UI may become unresponsive
		selfTest()
	elif (s == 'quit'):
		sys.exit(0)
	else:
		# Actual slots round is on a second thread, to not block your UI
		t = threading.Thread(target=playSlots)
		t.start()

