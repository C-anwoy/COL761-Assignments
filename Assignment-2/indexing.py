import sys
from matplotlib import pyplot as plt
import time
import subprocess
import numpy as np
import pickle

def generate_fsg_input(input_file, output_file):
    
    read_n_vertices = False
    read_n_edges = False
    
    reading_vertices = False
    reading_edges = False
    n_vertices = 0
    n_edges = 0
    
    map_tid_gid= {}
    
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
                            
                map_tid_gid[it] = int(line[1:])
                
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
    return map_tid_gid

def process_pc_list(pc_file):
    
    pc_dict={}
    n = 0
    with open(pc_file) as f:
        for line in f:
            n = n+1
            node_list = line.split()
            parent = node_list[0]
            if len(node_list)>1:
                pc_dict[parent] = node_list[1:]
            else:
                pc_dict[parent] = []
    
    #print("Total number of frequent patterns = ", n)
    return pc_dict 

def process_tid_list(tid_file):
    
    #key = frequent-pattern-id(in str format), value = list of graph tids (in str format) which contains the frequent pattern
    contain_dict = {}
    n = 0
    with open(tid_file) as f:
        for line in f:
            n = n+1
            tid_list = line.split()
            freq_pattern_id = tid_list[0]
            contain_dict[freq_pattern_id] = tid_list[1:]
            
    
    #print("Total number of frequent patterns from tid = ", n) 
    return contain_dict  

def process_fp_file(fp_file):
    #key = frequent-pattern-id(in str format), value = count/support of the freq pattern
    count_dict = {}
    with open(fp_file) as f:
        for line in f:
            if line[0]=='t':
                w = line.split()
                count = int(w[3])
                freq_pattern_id = w[2][:-1]
                count_dict[freq_pattern_id] = count
                
    return count_dict 
        
######################################### MAIN ################################
start = time.time()
    
dataset = sys.argv[1]   

input_name = 'dataset'

fsg_input = input_name+'.txt'


map_tid_gid = generate_fsg_input(dataset, fsg_input)


with open("tid_gid_dict.pickle", 'wb') as f:
        pickle.dump(map_tid_gid, f)

subprocess.run("./library/fsg -s " + str(float(40)) + " -pt "+ fsg_input, shell=True)


pc_dict = process_pc_list(input_name+'.pc')
contain_dict = process_tid_list(input_name+'.tid')
count_dict = process_fp_file(input_name+'.fp')

freq_pattern_ids = list(contain_dict.keys())

feature_ids =[]

for fid in freq_pattern_ids:
    if len(pc_dict[fid])>0:
        intersect_list=[]
        
        for child in pc_dict[fid]:
            intersect_list.append(set(contain_dict[child]))
        
        numerator = len(set.intersection(*intersect_list))
        d_ratio = numerator/(float(count_dict[fid]))
        
        if d_ratio > 1.4:
            feature_ids.append(fid)
            
    else:
        feature_ids.append(fid)

#print("Total features = ", len(feature_ids))   

with open("feature_ids.pickle", 'wb') as f:
        pickle.dump(feature_ids, f)
        
index = {}

for tid in map_tid_gid.keys():
    index[tid] = []
    
    for feature in feature_ids:
        if str(tid) in contain_dict[feature]:
            index[tid].append(1)
        else:
            index[tid].append(0)

with open("index.pickle", 'wb') as f:
        pickle.dump(index, f)
        
end = time.time()   

print("Index structure building completed.")
#print("Total time taken  = {} seconds".format(end - start))
#print(index)
        
    
