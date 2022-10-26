#include <bits/stdc++.h>
using namespace std;

//#define min_sup 1

//node of the FP Tree
class fp_node {

    public:
	   int itemname;
	   fp_node *parent;
	   vector<fp_node*> children;
	   int count;
	   fp_node *peer;
};

//typedef struct fp_node fp_node;
long long int total_transactions = 0; 
/*hash table naming unique_items containing the name of items and 
their respective frequencies */
map<int, int> unique_items;

map<int, int>* sorting_order;

/*header table containing the name of items and the pointers 
to the chain of nodes of the same itemname*/
map<int , fp_node*> header_table;

/*a list containing the frequent items: the most frequent item is at
index 0 and the least one is at index (n-1)*/
vector<int> orderedList;

//answer
vector<vector<string>> frequent_itemset_list;

/*function to create a new node for the fp-tree
and the count of the respective node is initialized to 1*/
fp_node* createNewNode(int itemname){

	//fp_node *new_node = (fp_node*)malloc(sizeof(fp_node));
    fp_node *new_node = new fp_node();
	new_node -> itemname = itemname;
	new_node -> parent = NULL;
	new_node -> children = {};
	new_node -> count = 1;
	new_node -> peer = NULL;

	return new_node;
}

/*function to create the chain of peers(nodes with same itemname)
with the first pointer from the header table:
peer pointer connects nodes of the same item
input: f = new node*/
void updatePeerPointer(fp_node *f, map<int , fp_node*> &header_table){
	int name = f -> itemname;
	fp_node *start = header_table[name];
	fp_node *n, *np;
	n = start;
	while(n != NULL){
		np = n;
		n = n -> peer;
	}

	np -> peer = f;
}

//scan the database file and find the frequency of each item
void findUniqueItems(ifstream &file, string fname){

	file.open(fname);

    if(file.is_open()) ; //cout<< "Succesfully opened "<< fname << "\n";
    else {
        //cout << "Failed to open " << fname << endl;   
        exit(-1);
    }

    //int total_transactions = 0; 

    string transaction;
    

    while(getline(file, transaction)){
        //read transaction by transaction from the database
        //getline(file, transaction);
        total_transactions ++;

        istringstream trans(transaction);
        //trans.clear();
        //trans.str(transaction);

        for(string item; trans >> item;){
            if (unique_items.find(stoi(item)) == unique_items.end())
                unique_items.insert({stoi(item), 1});
            else
                unique_items[stoi(item)]++;
            
            //items_in_tr.push_back(stoi(item));
            //cout << stoi(item) << endl;
            }
            //cout << "\n END OF LINE" << endl;  

    }//end of while for each line ; now goto next line

    file.close();
}

void filterFreqItems(map<int,int> &unique_items, float minSupCount){
    map<int, int>:: iterator it;
    for(it = unique_items.begin(); it != unique_items.end();){
        if(it->second < minSupCount){
            unique_items.erase(it++);
        }
        else ++it;
    }
}

bool isFreq(int itemname){
    if (unique_items.find(itemname) == unique_items.end())
        return false;
    else
        return true;
}

bool cmp_sort(int x, int y){

    if ((*sorting_order)[x] > (*sorting_order)[y]) 
        return true; 

    else if((*sorting_order)[x] == (*sorting_order)[y]) {
            if(x < y)
                return true;
            else
                return false;
        }

    else
        return false;
    }

//function to compare items to sort in order of decreasing frequency
bool compare_fn(int x, int y) 
{ 
    if (unique_items[x] < unique_items[y]) 
        return true; 
    else  
        return false; 
}

//function to compare items to sort in order of ascending frequency
bool compare_fn_asc(int x, int y) 
{ 
    if (unique_items[x] > unique_items[y]) 
        return true; 
    else  
        return false; 
}

bool comp_for_creating_sorting_order(int x, int y) 
{ 
    if (unique_items[x] > unique_items[y]) 
        return true; 
    else  
        return false; 
}

void createSortingOrder(vector<int> &temp){
    //vector<int> temp;
    map<int, int>:: iterator it;
    for(it = unique_items.begin(); it != unique_items.end(); it++){
        temp.push_back(it->first);
    }

    sorting_order = &unique_items;
    sort(temp.begin(), temp.end(), cmp_sort);
    //for(it = unique_items.begin(); it != unique_items.end();it++){
        //it->second = distance(temp.begin(), find(temp.begin(), temp.end(), it->first));
    //}

}


//build the FP Tree by scanning the database
void buildFPTree(ifstream &file, string fname, fp_node *root){

	file.open(fname);

    string transaction;
    

    while(getline(file, transaction)){

        //read transaction by transaction from the database
        vector<int> items_in_tr{};

        //trans.clear();
        //trans.str(transaction);
        istringstream trans(transaction);

        for(string item; trans >> item;){
            
            items_in_tr.push_back(stoi(item));
            //cout << stoi(item) << endl;
            }

            //cout << "\n END OF LINE" << endl;
            //for(auto i: item) cout << i << " ";
           	//cout << endl; 

        for(auto itr = items_in_tr.begin(); itr!= items_in_tr.end(); ++itr){
            if(!isFreq(*itr)){
                items_in_tr.erase(itr);
                itr--;
            }

        }

        sorting_order = &unique_items;
        sort(items_in_tr.begin(), items_in_tr.end(), cmp_sort);
        /*cout<< "Pruned Sorted Transaction: " ;
        for(auto itr = items_in_tr.begin(); itr!= items_in_tr.end(); ++itr){
                cout<<*itr<<" ";
        }
        cout<<endl;*/
         // to sort in descending order of frequency
        fp_node *n;
        n = root;
        int i =0;
        while(i < items_in_tr.size()){
        	//if items_in_tr[i] is not a child of n
        	/*
        		CHECK HERE
        	*/
            //int j =0;
                int flag = 0;
                for(int j =0; j < n->children.size(); j++){
                    if((n->children[j])->itemname == items_in_tr[i]){
                        flag = 1;
                        (n->children[j])-> count ++;
                        n = n->children[j];
                    }
                }

                if(flag==0){
                    fp_node *child = createNewNode(items_in_tr[i]);
                    n->children.push_back(child);
                    child -> parent = n;
                    n = child;
                    //update header table and/or peer pointer
                    if(header_table[items_in_tr[i]] == NULL)
                        header_table[items_in_tr[i]] = child;

                    else updatePeerPointer(child, header_table);

                }
            
            i++;

        } // end of while for i<items_in_tr.size()


    }//end of while for each line => goto next line

    file.close();
}

void updateHeaderTable(map<int,int> &unique_items, map<int, fp_node*> &header_table){
    map<int, int>:: iterator it;
    for(it = unique_items.begin(); it != unique_items.end(); it++){
        header_table.insert({it -> first, NULL});
    }
}

/*utility function to print hash map*/
void displayUniqueItemsUtil(map<int, int> &unique_items){
    map<int, int>:: iterator it;
    for(it = unique_items.begin(); it != unique_items.end(); it++){
        cout<< "I "<< it -> first << ":" << it->second << endl;
    }
}



void buildCondFPTree(fp_node *root, map<int, fp_node*> *header_table, 
                    const vector<int> &items_in_tr, int support)
{

    fp_node *n;
    n = root;
    int i =0;

    //items in tr contains the items sorted in descending order of frequency
    while(i < items_in_tr.size()){
            //if items_in_tr[i] is not a child of n
            /*
                CHECK HERE
            */
            //int j =0;
                int flag = 0;
                for(int j =0; j < n->children.size(); j++){
                    if((n->children[j])->itemname == items_in_tr[i]){
                        flag = 1;
                        (n->children[j])-> count += support;
                        n = n->children[j];
                    }
                }

                if(flag==0){
                    fp_node *child = createNewNode(items_in_tr[i]);
                    child -> count = support;
                    n->children.push_back(child);
                    child -> parent = n;
                    n = child;
                    //update header table and/or peer pointer
                    if((*header_table)[items_in_tr[i]] == NULL)
                        (*header_table)[items_in_tr[i]] = child;

                    else updatePeerPointer(child, (*header_table));

                }
            
            i++;

        } // end of while for i<items_in_tr.size()

}

void pushIntoFreqItemset(vector<int> &itemset){
    vector<string> line;
    for(int j=0; j<itemset.size(); j++){
        stringstream ss;
        string item;
        ss << itemset[j];
        ss >> item;
        line.push_back(item);
    }

    sort(line.begin(), line.end());
    frequent_itemset_list.push_back(line);
}

void mineFPTree(fp_node* head, vector<int> prefix, float minSupCount)
    {
        int sup = 0;
        fp_node *n;
        n = head;
        while(n!=NULL){
            sup += n->count;
            n=n->peer;
        }
        //cout<<"Support: "<< sup << endl;

        if(sup >= minSupCount){
            //map containing items and count
            map<int, int> itemsNfreq;

            prefix.push_back(head -> itemname);

            //for(int i=0; i<prefix.size(); i++){cout<<"prefix: "<< prefix[i]<<" ";}

            pushIntoFreqItemset(prefix);

            fp_node *t;
            t = head;

            while(t!=NULL){
                fp_node *temp;
                temp = t -> parent;
                while(temp->itemname != INT_MIN){
                    //prefix.push_back(n->itemname);
                    itemsNfreq[temp->itemname] += t->count;
                    temp = temp -> parent;
                }//end of while(t->itemname != INT_MIN)

                t=t->peer;
            }//end of while(t!=NULL)

        //displayUniqueItemsUtil(itemsNfreq);

        //create new orderedlist
        vector<int> new_orderedList;
       
        for(auto it = itemsNfreq.begin(); it != itemsNfreq.end(); it++){
            if(it -> second >= minSupCount)
                new_orderedList.push_back(it->first);
        }

        /*cout<<"new ordered list: ";
        for(int i=0; i<new_orderedList.size(); i++){cout<<new_orderedList[i]<<" ";}
        cout<<endl;*/

        //sort new ordered list
        sorting_order = &itemsNfreq;
        sort(new_orderedList.begin(), new_orderedList.end(), cmp_sort);

        /*cout<<"sorted new ordered list: ";
        for(int i=0; i<new_orderedList.size(); i++){cout<<new_orderedList[i]<<" ";}
        cout<<endl;*/

        //creating new tree
        fp_node *new_root = new fp_node();
        new_root -> itemname = INT_MIN;
        new_root -> parent = NULL;
        new_root -> children = {};
        new_root -> count = 0;
        new_root -> peer = NULL;

        //new header table
        map<int , fp_node*> new_header_table;
            

        t = head;
        while(t!=NULL){

                vector<int> trans;
                fp_node *temp;
                temp = t -> parent;
                while(temp->itemname != INT_MIN){
                    //prefix.push_back(n->itemname);
                    if(itemsNfreq[temp->itemname] >= minSupCount){
                        //create transaction
                        trans.push_back(temp->itemname);    
                    }
                    //itemsNfreq[temp->itemname] += t->count;
                    temp = temp -> parent;
                }//end of while(t->itemname != INT_MIN)


                //sort the transaction
                sorting_order = &itemsNfreq;
                sort(trans.begin(), trans.end(), cmp_sort);

                buildCondFPTree(new_root, &new_header_table, trans, t->count);

                t=t->peer;
            }//end of while(t!=NULL)
        
            for(int i=new_orderedList.size()-1; i>=0; i--){
            //cout<<new_orderedList[i]<< " ";
            //cout<<"---Mining for "<<new_orderedList[i]<<endl;
            mineFPTree(new_header_table[new_orderedList[i]], prefix, minSupCount);
            }//end of for

        }//end of if 

}


int main(int argc, char* argv[]){

    ifstream file;

    /*create a hash table with itemname as key and frequency as value
    Name of the hashtable (global variable): unique_items*/ 
    
    int min_sup = atoi(argv[1]);
    string input_path, output_path;
    input_path = argv[2];
    output_path = argv[3];

    findUniqueItems(file, input_path);
    //cout<<endl<<"Total transactions:"<<total_transactions<<endl;

    float minSupCount = (float(min_sup)/100) * total_transactions;
    
    filterFreqItems(unique_items, minSupCount);
    createSortingOrder(orderedList);

    //dislay the hashmap
    //displayUniqueItemsUtil(unique_items);

    //initialize the header table
    updateHeaderTable(unique_items, header_table);

    /*creating the root node : itemname = -1 for the root*/

    fp_node *root = new fp_node();
    root -> itemname = INT_MIN;
    root -> parent = NULL;
    root -> children = {};
    root -> count = 0;
    root -> peer = NULL;
    

    buildFPTree(file, input_path, root);
    //cout<<"FP Tree successfully built..."<<endl;

    //cout<<"Starting to mine the tree..."<<endl;
    
    for(int i=orderedList.size()-1; i>=0; i--){
        //cout<<orderedList[i]<< " ";
        //cout<<"Mining for "<<orderedList[i]<<endl;
        mineFPTree(header_table[orderedList[i]], vector<int>(), minSupCount);
    }
    
    sort(frequent_itemset_list.begin(), frequent_itemset_list.end());

    /*cout<<"frequent itemsets..."<<endl; 
    for(int i=0; i<frequent_itemset_list.size(); i++){
        cout<< "(" << i+1 << ") ";
        for(int j=0; j<frequent_itemset_list[i].size(); j++)
            cout<< frequent_itemset_list[i][j]<<" ";

        cout<<endl;
    }*/
    ofstream outputFile(output_path);

    for(int i=0; i<frequent_itemset_list.size(); i++){
        //cout<< "(" << i+1 << ") ";
        for(int j=0; j<frequent_itemset_list[i].size(); j++){
            outputFile << frequent_itemset_list[i][j];
            outputFile <<" ";
        }
        outputFile << "\n";
    }

    return 0;
}