import random

def generate(n):
	i = 1

	f = open("mat-" + str(n) + ".txt", "w")

	while i <= n:

		x = round(random.uniform(20000, 30000), 4)
		y = round(random.uniform(10000, 20000), 4)

		f.write(str(i) + " " + str(x) + " " + str(y) + "\n")

		i+=1

	f.close()



generate(100)
generate(1000)