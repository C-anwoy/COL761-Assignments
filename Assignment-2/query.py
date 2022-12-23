import sys
from matplotlib import pyplot as plt
import time
import subprocess
import networkx as nx
import numpy as np
import pickle
from networkx.algorithms import isomorphism

def generate_fsg_input(input_file, output_file):
    
    read_n_vertices = False
    read_n_edges = False
    
    reading_vertices = False
    reading_edges = False
    n_vertices = 0
    n_edges = 0
    
    #map_tid_gid= {}
    
    with open(input_file) as f_in, open(output_file, 'w') as f_out:
        
        #label = 0
        it = 0
        iv=0
        ie = 0
        
        for line in f_in:
           #start of transaction
            if line[0]=='#':
               # print(line)
                #f_out.write("t "+line)
                
                f_out.write("t # "+str(it)+"\n")
                            
                #map_tid_gid[it] = int(line[1:])
                
                it = it + 1
                
                read_n_vertices = True
                
                read_n_edges = False
                reading_vertices = False
                reading_edges = False
                
                n_vertices = 0
                n_edges = 0
                
                iv = 0
                ie = 0
            
            # read no. of vertices
            elif read_n_vertices == True:
                n_vertices = int(line)
                #print(n_vertices)
                read_n_vertices = False
                reading_vertices = True
                
            #read vertices
            elif (reading_vertices == True) and (iv < n_vertices):
                f_out.write("v "+str(iv)+" "+line)
                iv = iv+1
                
                if iv == n_vertices:
                    reading_vertices = False
                    read_n_edges = True
                
            # read no. of edges
            elif read_n_edges == True:
                n_edges = int(line)
                #print(n_edges)
                read_n_edges = False
                reading_edges = True
                
            #read edges
            elif (reading_edges == True) and (ie < n_edges):
                f_out.write("u "+line)
                ie = ie+1
                
                if ie == n_edges:
                    reading_edges = False
                    read_n_edges = False
                
           
    f_in.close()
    f_out.close()
    #print(output_file+" generated !!!")
    #return map_tid_gid

def get_feature_graphs(feature_ids):
    
    f_graphs = []
    f_ids = []
    
    for i in range(len(feature_ids)):
        with open("dataset.fp") as f:
            start = False
            done = False
            #end = False
        
            G = nx.Graph()
            v = []
            e = []
            #j = 0
        
            for line in f:
                #if done == True:
                    #break
                if line[0]=='#':
                    continue
            
                elif line[0]=='t':
                    #start = False
                    #end = True
                    
                    if start == True and done == False:
                        G.add_nodes_from(map(lambda x: (int(x[0]), {'label' : x[1]}), v))
                        G.add_edges_from(map(lambda x: (int(x[0]), int(x[1]), {'label' : x[2]}), e))
                        f_graphs.append(G)
                        done = True
                        break
                    
                    
                    w = line.split()
                    #count = int(w[3])
                    freq_pattern_id = w[2][:-1]
                
                    v = []
                    e = []
                
                    if freq_pattern_id == feature_ids[i]:
                        f_ids.append( w[2][:-1])
                        start = True
                        G = nx.Graph()
                        #count_dict[freq_pattern_id] = count
                    else:
                        start = False
            
                elif start==True:
                
                    if line[0]=='v':
                        v.append(line[2:].split())
                    
                    elif line[0]=='u':
                        e.append(line[2:].split())
                        #
                        #
                        #
                        #
                
                
                
    return f_graphs, f_ids

def get_all_graphs():
    
    graphs = {}


    with open("dataset.txt") as f:
        #start = False
        #end = False
        
        tid = -1
        G = nx.Graph()
        v = []
        e = []
        i = 0
        
        for line in f:
            if line[0]=='t':
                #start = True
                #end = True
                
                if i > 0:
                    G.add_nodes_from(map(lambda x: (int(x[0]), {'label' : x[1]}), v))
                    G.add_edges_from(map(lambda x: (int(x[0]), int(x[1]), {'label' : x[2]}), e))
                    graphs[tid] = G
                
                w = line.split()
                #count = int(w[3])
                tid = int(w[2])
                v = []
                e = []
                G = nx.Graph()
                i = i+1
                #count_dict[freq_pattern_id] = count
            
            elif line[0]=='v':
                    v.append(line[2:].split())
                    
            elif line[0]=='u':
                    e.append(line[2:].split())
            
        G.add_nodes_from(map(lambda x: (int(x[0]), {'label' : x[1]}), v))
        G.add_edges_from(map(lambda x: (int(x[0]), int(x[1]), {'label' : x[2]}), e))
        graphs[tid] = G
                
                
    return graphs    

def get_query_graphs(path):
    
    generate_fsg_input(path, "query_processed.txt")
    
    q_graphs = []

    with open("query_processed.txt") as f:
        #start = False
        #end = False
        
        G = nx.Graph()
        v = []
        e = []
        i = 0
        
        for line in f:
            if line[0]=='t':
                #start = True
                #end = True
                
                if i > 0:
                    #print("#########################")
                    #print(map(lambda x: (int(x[0]), {'label' : x[1]}), v))
                    #print(map(lambda x: (int(x[0]), int(x[1]), {'label' : x[2]}), e))
                    
                    G.add_nodes_from(map(lambda x: (int(x[0]), {'label' : x[1]}), v))
                    G.add_edges_from(map(lambda x: (int(x[0]), int(x[1]), {'label' : x[2]}), e))
                    q_graphs.append(G)
                
                w = line.split()
                #count = int(w[3])
                #tid = w[2]
                v = []
                e = []
                G = nx.Graph()
                i = i+1
                #count_dict[freq_pattern_id] = count
            
            elif line[0]=='v':
                    v.append(line[2:].split())
                    
            elif line[0]=='u':
                    e.append(line[2:].split())
                    
        G.add_nodes_from(map(lambda x: (int(x[0]), {'label' : x[1]}), v))
        G.add_edges_from(map(lambda x: (int(x[0]), int(x[1]), {'label' : x[2]}), e))
        q_graphs.append(G)
    
    #print(q_graphs[0].number_of_nodes())
    #print(q_graphs[0].number_of_edges())            
    return q_graphs 

######################################### MAIN ################################
#load index structure
    
with open("index.pickle",'rb') as f:
        index = dict(pickle.load(f))

with open("feature_ids.pickle",'rb') as f:
        feature_ids = list(pickle.load(f))

with open("tid_gid_dict.pickle",'rb') as f:
        map_tid_gid = dict(pickle.load(f))



     
#convert the frequent pattern in feature ids into networkx graph
feature_graphs, f_ids = get_feature_graphs(feature_ids)


#convert all graphs into networkx graphs
graphs = {}
graphs = get_all_graphs()


#take query file as input  
query_path = input("Enter the query file path: ")
#query_path = "data/sample_dataset.txt"
      
        
#convert graphs in query file into networkx graph
query_graphs = get_query_graphs(query_path)      


start = time.time()

#build index structure for query graphs
query_graph_index = []

time_array = []

for i in range(len(query_graphs)):
    q_start = time.time()
    ind = []
    for j in range(len(feature_graphs)):
        GM = isomorphism.GraphMatcher(query_graphs[i], feature_graphs[j], node_match=isomorphism.categorical_node_match('label', 'C'), edge_match=isomorphism.categorical_edge_match('label', '1'))
        if(GM.subgraph_is_isomorphic()):
            ind.append(1)
        else:
            ind.append(0)
        
    query_graph_index.append(ind)
    q_end = time.time()
    time_array.append(q_end - q_start)


answer =[]

label = True

for i in range(len(query_graphs)):
    q_start2 = time.time()
    a = []
    
    for tid in list(index.keys()):
    
        label = True
        for j in range(len(index[tid])):
            if index[tid][j] == 0 and query_graph_index[i][j]==1:
                label = False
                
        if label == True:
            
            GM = isomorphism.GraphMatcher(graphs[tid], query_graphs[i], node_match=isomorphism.categorical_node_match('label', 'C'), edge_match=isomorphism.categorical_edge_match('label', '1'))
            if(GM.subgraph_is_isomorphic()):
                a.append(map_tid_gid[tid])
            
    answer.append(a)
    q_end2 = time.time()
    time_array[i]+= (q_end2 - q_start2)

end = time.time()

with open('output_2022AIB2681.txt', 'w') as f:
    for line in answer:
        for word in line:
            f.write(str(word))
            f.write(' ')
        
        f.write('\n') 

for i in range(len(query_graphs)):
    print("Time taken for query-{}: {} milli-seconds [or, {} seconds]".format(i+1, time_array[i]*1000, time_array[i]))

print("-----------------------------------")       
print("Total time taken for querying = {} milli-seconds [or, {} seconds]".format((end-start)*1000, end-start))
