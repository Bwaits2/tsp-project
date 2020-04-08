import math
import random

# globals
nodes = []

# build list from txt file
def build():
	f = open('mat-test.txt', "r")

	for l in f:
		s = l.split(' ')
		nodes.append([float(s[1]), float(s[2])])


# distance funciton to compute distance of the entire tour
def distance(t):
	result = 0

	i = 1
	while i < len(t):
		result += math.sqrt((t[i][0] - t[i-1][0])**2 + (t[i][1] - t[i-1][1])**2)
		i+=1

	return result


# anneal function
def sa():
	t = 100000
	cr = .003

	current = nodes.copy()
	best = current.copy()

	while t > 1:
		print(t)
		new = current.copy();

		p1 = random.randint(0, len(new) - 1)
		p2 = random.randint(0, len(new) - 1)

		new[p1], new[p2] = new[p2], new[p1]

		c = distance(current)
		n = distance(new)

		if math.exp((c-n)/t) > random.random():
			current = new.copy()

		if n < c:
			best = new.copy()

		t *= 1 - cr

	return best


build()
print("distance before: " + str(distance(nodes)))
b = sa()
print("distance after: " + str(distance(b)))
