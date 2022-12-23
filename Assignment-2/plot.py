import sys
from matplotlib import pyplot as plt
import timeit
import subprocess

def generate_fsg_input(input_file, output_file):
    
    read_n_vertices = False
    read_n_edges = False
    
    reading_vertices = False
    reading_edges = False
    n_vertices = 0
    n_edges = 0
    
    with open(input_file) as f_in, open(output_file, 'w') as f_out:
        
        iv=0
        ie = 0
        
        for line in f_in:
           #start of transaction
            if line[0]=='#':
                #print(line)
                f_out.write("t "+line)
                
                
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
    print(output_file+" generated !!!")
    

def generate_gspan_input(input_file, output_file):
    
    read_n_vertices = False
    read_n_edges = False
    
    reading_vertices = False
    reading_edges = False
    n_vertices = 0
    n_edges = 0
    
    v_label_dict= {}
    
    with open(input_file) as f_in, open(output_file, 'w') as f_out:
        label = 0
        it = 0
        iv=0
        ie = 0
        
        for line in f_in:
           #start of transaction
            if line[0]=='#':
                #print(line)
                f_out.write("t # "+str(it)+"\n")
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
                v_l = (line.split())[0]
                
                if v_l in v_label_dict:
                    f_out.write("v "+str(iv)+" "+ str(v_label_dict[v_l]))
                    f_out.write("\n")
                else:
                    v_label_dict[v_l] = label
                    label = label+1
                    f_out.write("v "+str(iv)+" "+ str(v_label_dict[v_l]))
                    f_out.write("\n")
                    
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
                #w = line.split()
                f_out.write("e "+line)
                ie = ie+1
                
                if ie == n_edges:
                    reading_edges = False
                    read_n_edges = False
                    
    f_in.close()
    f_out.close()
    print(output_file+" generated !!!")



def generate_gaston_input(input_file, output_file):
    
    read_n_vertices = False
    read_n_edges = False
    
    reading_vertices = False
    reading_edges = False
    n_vertices = 0
    n_edges = 0
    
    it = 0
    
    v_label_dict= {}
    
    with open(input_file) as f_in, open(output_file, 'w') as f_out:
        label = 0
        iv=0
        ie = 0
        
        for line in f_in:
           #start of transaction
            if line[0]=='#':
                #print(line)
                f_out.write("t # "+str(it)+"\n")
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
                v_l = (line.split())[0]
                
                if v_l in v_label_dict:
                    f_out.write("v "+str(iv)+" "+ str(v_label_dict[v_l]))
                    f_out.write("\n")
                else:
                    v_label_dict[v_l] = label
                    label = label+1
                    f_out.write("v "+str(iv)+" "+ str(v_label_dict[v_l]))
                    f_out.write("\n")
                    
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
                #w = line.split()
                f_out.write("e "+line)
                ie = ie+1
                
                if ie == n_edges:
                    reading_edges = False
                    read_n_edges = False               
           
    f_in.close()
    f_out.close()
    print("Total graphs: ", it)
    print(output_file+" generated !!!") 
    return it

###############################################################################  
    
dataset = sys.argv[1]   

i = dataset.find('.')
input_name = dataset[:i]

fsg_input = input_name+'_fsg.txt'
gspan_input = input_name+'_gspan.txt'
gaston_input = input_name+'_gaston.txt'


#generate_fsg_input('test_data.txt', 'test_data_fsg.txt')
generate_fsg_input(dataset, fsg_input)
#generate_gspan_input('test_data.txt', 'test_data_gspan.txt')
generate_gspan_input(dataset, gspan_input)
#n_graphs = generate_gaston_input('test_data.txt', 'test_data_gaston.txt')
n_graphs = generate_gaston_input(dataset, gaston_input)


"""
output_name = ""

if (output[-4:] == ".png"):
    output_name = output
    
else:
    output_name = str(output)+".png"
    
"""  


supports = [95, 50, 25, 10, 5]
#supports = [95, 50]

fsg_ex_times=[]
gspan_ex_times=[]
gaston_ex_times=[]


for s in supports:
    fsg_ex_times.append(timeit.timeit("subprocess.run(\"./library/fsg -s " + str(float(s)) + " " + fsg_input + "\", shell=True)", setup = "import subprocess", number=1))
    gspan_ex_times.append(timeit.timeit("subprocess.run(\"./library/gSpan -f "+ " " + gspan_input+" -s " + str(float(s)*0.01)  + "\", shell=True)", setup = "import subprocess", number=1))
    gaston_ex_times.append(timeit.timeit("subprocess.run(\"./library/gaston " + str(int(float(s)*0.01*n_graphs)) + " " + gaston_input + "\", shell=True)", setup = "import subprocess", number=1))
    
plt.figure()
plt.plot(supports, fsg_ex_times, marker='D', label='FSG')
plt.plot(supports, gspan_ex_times, marker='D', label='gSpan')
plt.plot(supports, gaston_ex_times, marker='D', label='Gaston')
plt.title('Plot of Execution time v/s Support thresholds')
plt.xlabel('Support thresholds (in %)')
plt.ylabel('Execution time (in seconds)')
plt.legend()
plt.savefig("plot_"+input_name+".png")
