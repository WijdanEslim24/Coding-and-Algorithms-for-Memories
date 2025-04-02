# SCS Algorithm

This project implements an algorithm to compute the **Shortest Common Supersequence (SCS)** of several sequences when it is 
known that they all result from exactly *t* deletions from a master sequence of length *n*. In this setting, each sequence is 
exactly of length *n-t*, and they can differ one from another in at most *2t* positions. This property allows us to use a **banded
dynamic programming (banded DP - which takes advantage of the input t)** approach for computing the **Longest Common Subsequence
(LCS)** efficiently, and then use a pairwise (chain) merge to obtain the SCS with an overall
complexity much better than the naive approach.
time complexity: O(k! × n × t × k)  while k is the number of sequences in the test.

## Prerequisites

You will need:
- A C++ compiler 
- CMake (if you wish to use it for building the project)

# Running the Tests
You can run the executable with an input file.

Input file example:
8
ttttpp 
ittntp
izntpq
titqpp
titztn
ztnptt
pttqpp
ztntpt 
7

This corresponds to:
8: The number of sequences(k).
The next 8 lines: The sequences.
7: The value of t.

Expected Output:
SCS: titztnptptqpp
SCS Length: 13
