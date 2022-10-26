#! /bin/bash

if [ $1 == '-apriori' ] 
then 
	./apriori $3 $2 $4
	
elif [ $1 == '-fptree' ] 
then 
	./fptree $3 $2 $4

elif [ $1 == '-plot' ] 
then 
	python3 plot.py $2 $3

else 
	echo "Argument is invalid"
fi