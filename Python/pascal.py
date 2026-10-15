
p = lambda i,j : 1 if (j == 1) or (i==j) else  p(i-1, j-1) + p(i-1, j)

for i in range(1, 10):
    for j in range(1, i+1):
        print(str(p(i,j))+" ", end='')
    print()
