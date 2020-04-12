import math
import random
import sys

from statistics import stdev, mean

# globals
nodes = []

# build list from txt file
def build():
    f = open(sys.argv[1], "r")

    for l in f:
        s = l.split(' ')
        nodes.append([float(s[1]), float(s[2]), s[0]])

    f.close()


# distance funciton to compute distance of the entire tour
def distance(t):
    result = 0

    i = 1
    while i < len(t):
    	result += math.sqrt((t[i][0] - t[i-1][0])**2 + (t[i][1] - t[i-1][1])**2)
    	i+=1

    return result

# write results to file
def result(best):
    f = open(sys.argv[2], "w")

    f.write(str(distance(best)) + "\n")

    for b in best:
    	f.write(str(b[2]) + " ")

    f.close()


def compute():
	results = []

	for i in range(10):
		results.append(distance(sa()))

	print(results)

	print("stdev: " + str(stdev(results)))
	print("mean: " + str(mean(results)))



# anneal function
def sa():
    t = 1000000
    cr = .0001

    current = nodes.copy()
    best = current.copy()

    while t > 1:
    	new = current.copy()

    	p1 = random.randint(0, len(new) - 1)
    	p2 = random.randint(0, len(new) - 1)

    	new[p1], new[p2] = new[p2], new[p1]

    	c = distance(current)
    	n = distance(new)

    	try:
    		if math.exp((c-n)/t) > random.random():
    			current = new.copy()
    	except OverflowError:
    		pass

    	if n < c:
    		best = new.copy()

    	t *= 1 - cr

    return best



build()
#print("distance before: " + str(distance(nodes)))
compute()
#b = sa()
#print("distance after: " + str(distance(b)))
#result(b)
