#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<map>
#include<algorithm>
//#include<bits/stdc++.h>

using namespace std;

float min_sup;
long long int total_transaction=0;

void map_to_set(map<vector<int>,int> freq_table,vector<vector<int>>& Lk_1){
    
    for(auto itr=freq_table.begin();itr!=freq_table.end();itr++){
        Lk_1.push_back(itr->first);  
    }
}

bool is_mergeable(vector<int> a,vector<int> b){
    //sort(a.begin(),a.end());
    //sort(b.begin(),b.end());
    /*for(int i=0;i<=(a.size()-2);i++){
        cout<<"entered loop"<<endl;
        if(a[i]==b[i]) continue;
        else return false;
    }*/
    int i=0;
    while(i<=(int(a.size())-2)){
        if(a[i]==b[i]) {i++;continue;}
        else return false;
    }
    return true;
}

vector<int> merge(vector<int> a, vector<int> b){
    //sort(a.begin(),a.end());
    //sort(b.begin(),b.end());
    vector<int> v;
    
    for(int i=0;i<int(a.size())-1;i++){
        //cout<<"a.size()-1 in merge fun= "<<a.size()-1<<endl;
        //cout<<"a[i]= "<<a[i]<<endl;
        v.push_back(a[i]);
        //cout<<"V[i]= "<<a[i]<<endl;
    }
    //cout<<"a[a.size()-1] = "<<a[a.size()-1]<<"b[b.size()-1] = "<<b[b.size()-1]<<endl;
    if((a[a.size()-1])<(b[b.size()-1])){
        //cout<<"entered 1st loop \n";
            v.push_back(a[a.size()-1]);
            v.push_back(b[b.size()-1]);
    }
    else{
        //cout<<"entered 2nd loop \n";
        v.push_back(b[b.size()-1]);
        v.push_back(a[a.size()-1]);
    } 
    /*for(int i=0;i<v.size();i++){
        cout<<v[i]<<" ";
    }*/
    //cout<<endl;
    return v;
}

void copy_to_freqset(vector<vector<int>> Lk_1,vector<vector<string>>& freq_itemset){
    
    for(int i=0;i<Lk_1.size();i++){
        vector<string> line;
        for(int j=0;j<Lk_1[i].size();j++){
            stringstream ss;
            string item;
            ss<<Lk_1[i][j];
            ss>>item;
            line.push_back(item);
        }
        sort(line.begin(),line.end());
        freq_itemset.push_back(line);
    }
    
}

void merged(vector<vector<int>> Lk_1,vector<vector<int>>& Ck){
    vector<int> a; vector<int> b;
    //cout<<"entered in 1st loop \n";
    //cout<<"Lk_1.size= "<<Lk_1.size()<<endl;
    for(int i=0;i<Lk_1.size()-1;i++){
       // cout<<"Lk_1.size-1 inside loop= "<<Lk_1.size()-1<<endl;
        for(int j=i+1;j<Lk_1.size();j++){
            a=Lk_1[i];
            b=Lk_1[j];
            for(int p=0;p<a.size();p++){
                //cout<<"a[i]= "<<a[p]<<" b[i]= "<<b[p]<<endl;
            }
            if(is_mergeable(a,b)){
                //cout<<"inside 3rd loop";
                Ck.push_back(merge(a,b));
            }
        }
    } 
}

void printfun(vector<vector<string>> freq_itemset, string output_file){
    ofstream file(output_file);

    //int index=1;
    sort(freq_itemset.begin(),freq_itemset.end());
    for(int i=0;i<freq_itemset.size();i++){
        
        //cout<<"("<<index++<<") ";
        
        for(int j=0;j<(freq_itemset[i].size());j++){
            //cout<<freq_itemset[i][j]<<" ";
            file<<freq_itemset[i][j];
            file<<" ";
        }
        file<<"\n";
        //cout<<endl;
        
    }
    //cout<<endl;
}

void scan_transcation(vector<vector<int>> Ck,map<vector<int>,int>& freq_table,string testcase_file,float min_sup){
    ifstream in(testcase_file);
    
    string line;
    //string digit;
    vector<int> itemset{};
    while(getline(in,line)){
        istringstream is(line);
        for(string digit; is>>digit;){
            itemset.push_back(stoi(digit));
        }
        /*
        is.clear();
        is.str(line);
        vector<int> itemset{};
        while(is.good()){
            is>>digit;
            itemset.push_back(stoi(digit));
        }*/
        sort(itemset.begin(),itemset.end());
        for(int i=0;i<Ck.size();i++){
           // cout<<"entered 1st loop \n";
            sort(Ck[i].begin(),Ck[i].end());
            if(includes(itemset.begin(),itemset.end(),Ck[i].begin(),Ck[i].end())){
                if(freq_table.find(Ck[i])!=freq_table.end()){
                    //cout<<"entered 2nd loop"<<endl;
                    map<vector<int>,int> :: iterator itr = freq_table.find(Ck[i]);
                    itr->second++;
                }
                else{
                    // cout<<"entered 3rd loop"<<endl;
                    freq_table[Ck[i]]=1;
                }
            }
        }
        itemset.clear();
    }
    for(auto itr=freq_table.begin();itr!=freq_table.end();){
        if((itr->second)<(min_sup*total_transaction)){
            //cout<<itr->first[0]<<" "<<itr->second<<endl;
            freq_table.erase(itr++);                 
        } 
        else {itr++;}
    }    
}

vector<vector<int>> subset(vector<int> set){
    vector<vector<int>> result;
    /*cout<<"entered in subset \n";
    for(int i=0;i<set.size();i++){
        cout<<set[i]<<" ";
    }
    cout<<endl;*/
    for(int i=0;i<set.size();i++){
        //cout<<"entered the loop \n";
        vector<int> dulplicate_set(set);
        vector<int> :: iterator itr;
        itr=dulplicate_set.begin()+i;
        dulplicate_set.erase(itr);
        /*for(int j=0;j<dulplicate_set.size();j++){
        cout<<dulplicate_set[j]<<" ";
          }
         cout<<endl;*/

        result.push_back(dulplicate_set);
        
        //cout<<"came here \n";
        /*vector<int> element,r;
        element.push_back(set[i]);
        vector<int> :: iterator itr,itr2;
        itr = std::set_difference(set.begin(),set.end(),element.begin(),element.end(),r.begin());
        for(itr2=r.begin();itr2<itr;itr2++){
            result[i].push_back(*itr);
        }*/
    }
    return result;
}


void prune_candidate(vector<vector<int>>& candidate,map<vector<int>,int> freq_table){
    for(int i=0;i<candidate.size();i++){
        //cout<<"entered loop \n";
        vector<vector<int>> sub_set;
        
        /*for(int j=0;j<candidate[i].size();j++){
             cout<<candidate[i][j]<<" ";
        }
            cout<<endl;*/
        
        sub_set=subset(candidate[i]);
       // cout<<"subset fun not terminating \n";
       /*for(int i=0;i<sub_set.size();i++){
        for(int j=0;j<sub_set[i].size();j++){
            cout<<sub_set[i][j]<<" ";
        }
        cout<<endl;
       }*/
        for(int j=0;j<sub_set.size();j++){
            if(freq_table.find(sub_set[j])==freq_table.end()){
                vector<vector<int>> :: iterator itr = candidate.begin()+i;
                candidate.erase(itr);
                i--;
                break;
            }
        }
    }
}



int main(int argc, char * argv[]){
    //cout<<"Enter the testcase file name: "<<endl;
    string testcase_file,output_file;
    //cin>>testcase_file;
    int support;
    support=atoi(argv[1]);
    testcase_file=argv[2];
    output_file=argv[3];
    ifstream in(testcase_file);

    string set_line;
    
    map<vector<int>,int> freq_table1;
    vector<vector<int>> Lk_1; 
    vector<vector<int>> reading_set;
    vector<vector<string>> freq_itemset; 
    vector<vector<int>> Candidate; 

    min_sup=float(support)/100;

    while(getline(in,set_line)){
        istringstream is(set_line);
        vector<int> item{};
        total_transaction++;
        for(string digit; is>>digit;){
            item.push_back(stoi(digit));
            reading_set.push_back(item);
            item.clear();
        }
        /*vector<int> item{};
        is.clear();
        is.str(set_line);
        while(is.good()){
            is>>digit;
            item.push_back(stoi(digit));
            reading_set.push_back(item);
            item.clear();
        }*/


        /*for(int i=0;i<reading_set.size();i++){
            vector<int> j=reading_set[i];
            for(int t=0;t<j.size();t++){
                 cout<<j[t]<<" ";   
            }
            cout<<endl;
        }*/
        
        for(int i=0;i<reading_set.size();i++){
            if(freq_table1.find(reading_set[i])!=freq_table1.end()){
                map<vector<int>,int> :: iterator itr;
                itr=freq_table1.find(reading_set[i]);
                itr->second++;
            }
            else{
                freq_table1[reading_set[i]]=1;
            }
        }
        reading_set.clear();
        /*
        string delim=" ";
        size_t position=0;
        while((position=itemset.find(delim))!=string:: npos){
              //cout<<itemset.substr(0,position)<<" ";  
              item.push_back(stoi(itemset.substr(0,position)));
              itemset.erase(0,position+delim.length());
        }
        //cout<<itemset<<" ";
        item.push_back(stoi(itemset));
        */
       
    }
    in.close();
    
    for(auto itr=freq_table1.begin();itr!=freq_table1.end();){
        if((itr->second)<(min_sup*total_transaction)){
            freq_table1.erase(itr++);                 
        } 
        else {itr++;}
    }

    map_to_set(freq_table1,Lk_1);
    int i=1;
    while(!freq_table1.empty()){
        //cout<<"candidate generated of size "<<i++<<endl;
        copy_to_freqset(Lk_1,freq_itemset);
        Candidate.clear();
        merged(Lk_1,Candidate);
        //cout<<"merged Lk-1 to generate Ck of size "<<i<<endl;
        if(i>=3){
            prune_candidate(Candidate,freq_table1);
        }
        freq_table1.clear();
        Lk_1.clear();
        scan_transcation(Candidate,freq_table1,testcase_file,min_sup);
        //cout<<"hash table generated for itemset of size "<<i<<endl;
        map_to_set(freq_table1,Lk_1);
    }
    printfun(freq_itemset,output_file);
    //cout<<"test case = "<<total_transaction<<endl;
    //cout<<"min*case ="<<min_sup*total_transaction;

}