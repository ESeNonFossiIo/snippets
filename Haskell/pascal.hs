pas(i,j) = if (i == j) || (j==1) then 1 else pas(i-1, j) + pas(i-1, j-1)
