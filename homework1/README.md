# Homework 1
# Problem 1:
Ackermann's function A (m,n) is defined as follows:

$$
A(m, n) = 
\begin{cases}
n + 1 & \text{if } m = 0 \\
A(m-1, 1) & \text{if } n = 0 \\
A(m-1, A(m, n-1)) & \text{otherwise}
\end{cases}
$$

This function is studied because it grows very fast for small values of m and n. Write a recursive function for computing this function. Then write a nonrecursive algorithm for computing Ackermann's function.
# Problem 2:
If S is a set of n elements, the powerset of S is the set of all possible subsets of S. For example, if S = (a,b,c), then powerset (S) = {(), (a), (b), (c), (a,b), (a,c), (b,c), (a,b,c)}. Write a recursive function to compute powerset (S)
