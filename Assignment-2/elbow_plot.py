from sklearn.cluster import KMeans
import pandas as pd
from matplotlib import pyplot as plt
import numpy as np 
import sys

dataset = sys.argv[1] 
dimension = sys.argv[2]
roll = sys.argv[3]

output_name = "q3_"+dimension+"_"+roll+".png"
#i = dataset.find('.')
#output_name = dataset[:i]+"_plot.png"

data_set=np.array(pd.read_csv(dataset, header=None, sep=" "))
#print(data_set.shape)
wcss=[]

for k in range(1,16):
    km=KMeans(n_clusters=k)
    kmean=km.fit(data_set)
    error=kmean.inertia_
    wcss.append(error)

K=np.arange(1,16)
plt.plot(K, wcss, marker='x')
plt.title('Elbow plot')
plt.xlabel('K (number of clusters)')
plt.ylabel('Variation within clusters \n(sum of squared distanes of points \nto their closest cluster center)')
#plt.legend()
plt.savefig(output_name, dpi=500)
plt.show()  