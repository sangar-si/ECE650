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

using namespace std;

struct edge_pair{
    int left;
    int right;
};

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



int main(void) {
    // -- allocate on the heap so that we can reset later if needed
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    std::vector<std::vector<Minisat::Lit>> lits;
    unsigned int k=1, k_max = 5, i, j, i1, j1, left, right;
    std::vector<edge_pair> E;
    edge_pair e_temp;
    bool res;   
    vector<Minisat::Lit> lit;
    Minisat::vec<Minisat::Lit> temp_lit, temp_lit1;

    std::cout<<"Start entering stuff..."<<std::endl;
    std::cin>>k_max;
    for(i=0;i<6;i++){
        e_temp.left=1;
        e_temp.right=1;
        E.push_back(e_temp);
    }
    cout<<"Done1"<<endl;
    k=2;
    while(k<k_max){
        //Assign variables
        for(i=0;i<k-1;i++){
            cout<<"3"<<endl;
            for(j=0;j<k_max;j++){
                cout<<"1"<<endl;
                lit.push_back(Minisat::mkLit(solver->newVar()));
            }
            cout<<"2"<<endl;
            lits.push_back(lit);
            cout<<"2"<<endl;
        }
        cout<<"Done";
        //Create the first set of clauses
        for(i=0; i<k; i++){
            for(j=0; j<k_max; j++){
                temp_lit.push(lits[i][j]);
            }
            solver->addClause(temp_lit);
            temp_lit.clear();
        }

        //Create the second set of clauses
        if(k>1){
            for(j=0;i<k_max;j++){
                for(i=0;i<k-1;i++){
                    for(i1=i+1;i1<k;i1++){
                        solver->addClause(~lits.at(i)[j],~lits.at(i1)[j]);
                    }
                }
            }
        }

        //Create the thrid set of clauses
        for(i=0;i<k;i++){
            for(j=0;j<k_max-1;j++){
                for(j1=j+1;j1<k_max;j1++){
                    solver->addClause(~lits.at(i)[j],~lits.at(i)[j1]);
                }
            }
        }

        //Encoding the edges as clauses
        
        temp_lit.clear();
        for(j=0;j<E.size();j++){
                left = E.at(j).left;
                right = E.at(j).right;
            for(i=0;i<k;i++){
                temp_lit.push(lits.at(i)[left+1]);
                temp_lit.push(lits.at(i)[right+1]);
            }
            solver->addClause(temp_lit);
            temp_lit.clear();
        }

        res = solver->solve();
        if (res){
            break;
        }
        k++;
    }
    std::cout<<"The result is :"<<res<<std::endl;
    for(i=0;i<k;i++){
        for(j=0;j<k_max;j++){
            cout<<Minisat::toInt(solver->modelValue(lits.at(i)[j]))<<std::endl;
        }
    }

    return 0;
}
