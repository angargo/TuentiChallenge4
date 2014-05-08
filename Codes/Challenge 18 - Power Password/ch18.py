
input() #Skips 'wrong!'
i0 = int(input())
i1 = int(input())
i2 = int(input())

#We know that sol < 10^14, so m = sqrt(10^14)
m = 10000000

#i0^(-1) (mod i1)
i0_1 = pow(i0, i1 - 2, i1)

#i0^(-m) (mod i1)
i0_m = pow(i0_1, m, i1)

s = set()

#baby-step giant-step algorithm

#store m powers of i0 in a set
t = 1
for i in range(m):
    s.add(t)
    t = (t * i0) % i1


#compute powers of i0^(-m) multiplied by i2 and look for them in the set
sol = 0
t = i2
for j in range(m):
    if t in s:
        #A matching is found, so exists 'i' such that i2 * (i0^(-m))^j = i0^i
        sol = j
        break
    t = (t * i0_m) % i1

#look for that 'i'
goal = (i2 * pow(i0_m, sol, i1)) % i1
t = 1
for i in range(m):
    if t == goal:
        #A solution is found: i2 = i0^(j * m + i)
        #Print solution
        print(j * m + i)
        break
    t = (t * i0) % i1
