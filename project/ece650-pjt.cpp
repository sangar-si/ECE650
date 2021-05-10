// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/minisat/core/Solver.h"
#include "minisat/minisat/mtl/Vec.h"
// defined std::cout
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string.h>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/wait.h> 


using namespace std;

struct edge_pair{
    int left;
    int right;
};

char* intToStr(int data) {
    std::string strData = std::to_string(data);

    char* temp = new char[strData.length() + 1];
    strcpy(temp, strData.c_str());

   return temp;
}

int graph_out(int n){
    char *args[2];
    args[0] = (char *)"/graphGen";
    args[1] = intToStr(n);
    execv("/home/agurfink/ece650/graphGen/graphGen",args);
    return 0;
}

int print_vector(vector<edge_pair> obj){
    /*Prints a node in suitable form. Used for testing*/
    unsigned int i;
    for(i=0;i<obj.size();i++){
        cout<<"Left --> "<<obj[i].left;
        cout<<" Right--> "<<obj[i].right<<endl;
    }
    cout<<endl;
    return 0;
}

int parse_input_edges(int max_vertex, std::string edges, std::vector<edge_pair> *eps){
    /*Takes max vertex, edges and edge pair vector as input and parses the edges string and creates a node pair vector*/
    edge_pair one_edge_pair;
    unsigned int i=0,ctr=1;
    //Replacing non numerical characters excluding '-' with spaces so that the same can be parsed into an intereger variable
    for(i=0;i<edges.size();i++){
        if ((edges[i]=='0')||(edges[i]=='1')||(edges[i]=='2')||(edges[i]=='3')||(edges[i]=='4')||(edges[i]=='5')||(edges[i]=='6')||(edges[i]=='7')||(edges[i]=='8')||(edges[i]=='9')||(edges[i]=='-')){
            edges[i]=edges[i];
        }
        else{
            edges[i]=' ';
        }
    }

    std::stringstream ed(edges);
    std::stringstream ed_check(edges);
    int edge;
    ctr=1;
    while(ed>>edge){
        if ((edge<=max_vertex)&&(edge> 0)){
            if(ctr%2 == 1){
                one_edge_pair.left = edge;
                ctr++;
            }
            else{
                one_edge_pair.right = edge;
                eps->push_back(one_edge_pair);
                ctr++;
            }

        }
        else{
            std::cerr<<"Error: Invalid edge entry"<<std::endl;
            return -1;
        }
    }

    //Error checking for the number of edges
    ctr=0;
    while(ed_check>>edge){
        ctr++;
        if (edge>max_vertex){
            std::cerr<<"Error: Invalid edge entry"<<endl;
            return -1;
        }
    }
    if (ctr%2==1){
        std::cerr<<"Error: Invalid number of edges"<<endl;
        return -1;
    }
    return 0;
}

int cnf_sat(unsigned int k_max, std::vector<edge_pair> *E){
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    unsigned int i, j, i1, j1, left, right;
    unsigned int k=1;
    bool res;   
    Minisat::vec<Minisat::Lit> temp_lit, lits;
    vector<int> cnf;

    while(k<k_max){
        //Assign variables
        unsigned int var_no;
        var_no = k*k_max;
        lits.clear();
        for(i=0;i<var_no;i++){
            lits.push(Minisat::mkLit(solver->newVar()));
        }
        //Create the first set of clauses
        for(i=0; i<k; i++){
            for(j=0; j<k_max; j++){
                temp_lit.push(lits[(i*k_max)+j]);
            }
            solver->addClause(temp_lit);
            temp_lit.clear();
        }
        //Create the second set of clauses
        if(k>1){
            for(j=0;j<k_max;j++){
                for(i=0;i<k-1;i++){
                    for(i1=i+1;i1<k;i1++){
                        solver->addClause(~lits[(i*k_max)+j],~lits[(i1*k_max)+j]);
                    }
                }
            }
        }
        //Create the thrid set of clauses
        for(i=0;i<k;i++){
            for(j=0;j<k_max-1;j++){
                for(j1=j+1;j1<k_max;j1++){
                    solver->addClause(~lits[(i*k_max)+j],~lits[(i*k_max)+j1]);
                }
            }
        }
        //Encoding the edges as clauses
        temp_lit.clear();
        for(j=0;j<E->size();j++){
                left = E->at(j).left;
                right = E->at(j).right;
            for(i=0;i<k;i++){
                temp_lit.push(lits[(i*k_max)+(left-1)]);
                temp_lit.push(lits[(i*k_max)+(right-1)]);
            }
            solver->addClause(temp_lit);
            temp_lit.clear();
        }
        res = solver->solve();
        if (res){
            for(i=0;i<k;i++){
                for(j=0;j<k_max;j++){
                    if(Minisat::toInt(solver->modelValue(lits[(i*k_max)+j]))==0){
                        cnf.push_back(j+1);
                    }
                }
            }
            sort(cnf.begin(), cnf.end());
            cout<<"CNF-SAT-VC: ";
            for(i=0;i<cnf.size();i++){
                if(i==cnf.size()-1){cout<<cnf.at(i)<<endl;}
                else{cout<<cnf.at(i)<<" ";}
            }
            return 0;  
        }
        solver.reset (new Minisat::Solver());
        k++;  
    }
    cout<<k;
    return -1;
}

int approx_vc_1(std::vector<edge_pair> *E){
    vector<int> Edges, V_cover, del;
    vector<edge_pair> Eps;
    unsigned int i,j,max=0, reps=0, reps_max=0, curr=0;
    for(i=0;i<E->size();i++){Eps.push_back(E->at(i));}
    while(Eps.size()>0){
        max = 0;
        reps = 0;
        reps_max = 0;
        for(i=0;i<Eps.size();i++){
            del.push_back(Eps[i].left);
            del.push_back(Eps[i].right);
        }
        sort(del.begin(),del.end());
        curr = del[0];
        for(i=0;i<del.size();i++){
            if(curr==del[i]){
                reps++;
                if(reps>reps_max){
                    reps_max=reps;
                    max = curr;
                    }
            }
            else{
                reps=1;
                curr=del[i];
            }
        }
        del.clear();
        V_cover.push_back(max);
        for(i=0;i<Eps.size();){
            if((Eps[i].left == max)||(Eps[i].right == max)){
                Eps.erase(Eps.begin()+i);
                continue;
            }
            i++;
        }

    }
    sort(V_cover.begin(), V_cover.end());
    cout<<"APPROX-VC-1: ";
    for(i=0;i<V_cover.size();i++){
        if(i==V_cover.size()-1){cout<<V_cover.at(i)<<endl;}
        else{cout<<V_cover.at(i)<<" ";}
    }
    return 0;
}


int approx_vc_2(std::vector<edge_pair> *E){
    vector<int> Edges, V_cover, del;
    vector<edge_pair> Eps;
    unsigned int i,j,ind=0, left, right;
    for(i=0;i<E->size();i++){Eps.push_back(E->at(i));}
    while(Eps.size()>0){
        ind = 0;
        left = Eps[ind].left;
        right = Eps[ind].right;
        V_cover.push_back(Eps[ind].left);
        V_cover.push_back(Eps[ind].right);
        Eps.erase(Eps.begin()+ind);
        for(i=0;i<Eps.size();i++){
            if((Eps[i].left==left)||(Eps[i].left==right)||(Eps[i].right==left)||(Eps[i].right==right)){
                del.push_back(i);
            }

        }

        for(i=0;i<del.size();i++){
           Eps.erase(Eps.begin()+del[i]-i);
        }
        del.clear(); 

    }
    sort(V_cover.begin(), V_cover.end());
    cout<<"APPROX-VC-2: ";
    for(i=0;i<V_cover.size();i++){
        if(i==V_cover.size()-1){cout<<V_cover.at(i)<<endl;}
        else{cout<<V_cover.at(i)<<" ";}
    }
    return 0;
}
int main(void) {
    // -- allocate on the heap so that we can reset later if needed
        int GtoP[2];
        string vertices,edges,cmd,edge_stream,line;
        string e = "E";
        string v = "V";
        int vertices_max, new_line;
        std::vector<edge_pair> eps;
 //       vector<int> cnf;
        int ret;
        bool vertex_received = false;
        bool err_ = false;
        pid_t fork_p;

        int x=3;
        while (x<20) {
            //Getting the input Vertex. Checking for error and storing the maximum value and assigning the same number of memory
            fflush(stdout);
            x++;
            std::istringstream line_ip(line);
            char cmd_action;
            if(!vertex_received){
            pipe(GtoP);
            fork_p = fork();
            if(fork_p == 0){
                dup2(GtoP[1],STDOUT_FILENO);
                close(GtoP[0]);
                close(GtoP[1]);
                return graph_out(x);
            }
            dup2(GtoP[0],STDIN_FILENO);
            close(GtoP[0]);
            close(GtoP[1]);
    //user input part
            }
            getline(cin,line);

            line_ip>>cmd_action;
            switch(cmd_action){
                case 'V':
                    line_ip>>vertices_max;
                    if (vertices_max <= 1){
                        std::cerr<<"Error: Vertex cannot be less than or equal to 1"<<endl;
                        err_ = true;
                        break;
                    }
                    err_ = false;
                    vertex_received = true;
                    break;
                case 'E':
                    if((!vertex_received)&&(!err_)){
                        cerr<<"Error: Vertex not received"<<endl;
                        //err_ = true;
                        break;
                    }
                    line_ip>>edge_stream;
                    eps.clear();
                    ret = parse_input_edges((unsigned int) vertices_max, edge_stream, &eps);
                    if (ret!=0){
                        //err_ = true;
                        break;
                    }
                    vertex_received = false;
                    ret = cnf_sat(vertices_max,&eps);
                    ret = approx_vc_1(&eps);
                    ret = approx_vc_2(&eps);
                    if(ret!=0){
                        cout<<"Error: CNF SAT Unsuccessful"<<endl;
                    }
                    break;
                default:
                    if (line.size()>0){
                    cerr<<"Error: Unknown command"<<endl;
                    break;
                    }
            }
        } 
        return 0;
    
}
