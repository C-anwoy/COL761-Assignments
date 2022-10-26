----------------------------------------------------------------
Files :
----------------------------------------------------------------
1)apriori.cpp 	(Implementation of the Apriori algorithm)
2)fptree.cpp	  (Implementation of the FP-Tree algorithm)
3)compile.sh	
4)run.sh
5)plot.py		    (Python code for plotting the graph between execution time and support threshold)
6)README.txt



----------------------------------------------------------------
Explaination of Q3
----------------------------------------------------------------
On plotting the graph between the execution time of the two algorithms and the support thresholds, we observe that the time required for 
execution of the two is very close in case of higher thresholds but as the support threshold is decreased, the difference between the 
execution time of the two increases with FP-Tree algorithm being substantially faster than the Apriori algorithm. The reason behind it 
is that for Apriori, the transaction database is scanned once for every Candidate set of k-sized itemsets, and hence as the support 
threshold is decreased, the largest value of k is likely to go up, resulting in greater number of database scanning. 

On the other hand, for FP-Tree algorithm, the transaction database is scanned exactly twice - once for identifying the distinct items
and their frequency, and second time while constructing the FP-Tree. Thus even if the support threshold is decresed, the number of times 
transaction database is scanned is still two. For this reason, the difference between the execution time of the two is significant for lower 
support thresholds.
