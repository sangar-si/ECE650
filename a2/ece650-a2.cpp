// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
/*
Tasks:
1. Do the program (done)
2. Use istringstream instead of stringstream (done)
3. Error handling : Input edge out of range (done)
4. Error handling : input edge has odd number of number entries (done)
5. Error when there exists no path between two given nodes (done)
6. Check for negative edges (done)
7. Program should accept V E and the be able to accept multiple s commands. (done)
8. Ctrl + D gives an Error before exiting (done)
9. Try to get all the input first before processing (done)(parsing now done with each line)
10. End of file to be fine tuned. it still throws an error.(done)
11. Negative values are being taken as positive values(done)
12. Blank lines to be ignored(done)
13. Try passing pointer instead of value (try)(too risky since its made and working)
14. 0 is not a valid input for a vertex.(done)
15. Cannot input E without V (done)
16. Program should forget previous values properly(done)
17. Clear cmake warnings (done)
*/

/*REFERENCES
[1] BFS Algorithm implemention based from the lectures of Dr. Deepak Khemani
    Link : https://www.youtube.com/watch?v=QqdN7LH3jeA
[2] http://www.cplusplus.com/reference/sstream/stringstream/stringstream/
[3] https://www.geeksforgeeks.org/stringstream-c-applications/
[4] https://www.youtube.com/watch?v=bADtYBxrM8I
[5] http://www.cplusplus.com/reference/sstream/stringstream/?kw=stringstream

*/


#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string.h>
using namespace std;

struct node{
    int val;
    int parent;
    //bool seen;

};

struct edge_pair{
    int left;
    int right;
};

int node_check(int start_node, int end_node, vector<edge_pair> eps){
    /*Checks if both entered values are present in the given edge input. Throw error if it doesn't exist*/
    bool st_node_flag = false;
    bool end_node_flag = false;
    unsigned int i;
    for(i=0;i<eps.size();i++){
        if(((start_node == eps[i].left)||(start_node == eps[i].right))&&(!st_node_flag)){
            st_node_flag = true;
        }
        if(((end_node==eps[i].left)||(end_node==eps[i].right))&&(!end_node_flag)){
            end_node_flag = true;
        }
        if (st_node_flag && end_node_flag){
            break;
        }
    }
            //Break if both of them are not present
    if (!(st_node_flag && end_node_flag)){
        std::cerr<<"Error: Invalid command. Vertex out of range"<<endl;
        return -1;
    }
    return 0;
}

int print_vector(vector<node> obj){
    /*Prints a node in suitable form. Used for testing*/
    unsigned int i;
    for(i=0;i<obj.size();i++){
        cout<<endl;
        cout<<"Value --> "<<obj[i].val<<endl;
        cout<<"Parent--> "<<obj[i].parent<<endl;
        cout<<endl;
    }
    return 0;
}

int parse_input_edges(int max_vertex, string edges, vector<edge_pair> *eps){
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

    stringstream ed(edges);
    stringstream ed_check(edges);
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
            std::cerr<<"Error: Invalid edge entry"<<endl;
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

bool find_in_closed(int val, vector<node> closed){
    /*finds if a given child is already present in closed vector*/
    unsigned int i;
    for(i=0;i<closed.size();i++){
        if(val == closed[i].val){
            return true;
        }
    }
    return false;
}

bool find_in_open(int val, vector<node> open,int start_index){
    /*checks if a given child is already present in open vector*/
    unsigned int i;
    for(i=start_index;i<open.size();i++){
        if(val == open[i].val){
            return true;
        }
    }
    return false;
}

int print_path(vector<int> path){
    /*Takes the identified path vector and prints it in a suitable form as per requirement*/
    cout<<path[path.size()-1];
    path.pop_back();
    while(path.size()>0){
        cout<<"-";
        cout<<path[path.size()-1];
        path.pop_back();
    }
    cout<<endl;
    return 0;

}

int trace_path(vector<node> closed){
    /*Uses closed node vector to back track all the way from goal node to start node and find its way back*/
    vector<int> path;
    node temp;
    int search_node,i;
    temp = closed[closed.size()-1];
    closed.pop_back();
    path.push_back(temp.val);
    search_node = temp.parent;
    i=closed.size()-1;
    while(search_node != -1){
        if(search_node == closed[i].val){
            path.push_back(closed[i].val);
            search_node = closed[i].parent;
        }
        i--;
        if(i<0){
            i=closed.size();
        }
    }
    print_path(path);
    return 0;

}

int find_shortest_path(int start_node, int goal_node, vector<edge_pair> edge_pairs){

    /*BFS algorithm is followed to find the shortest path.
    open_front_index --> Acts like front pop as we would increment it and start considering from that point
    open --> queue containing all the nodes that are to be checked. Nodes are added at the back and removed from front
    closed --> contains all the nodes that have been checked
    Only nodes that are not present in both open and closed are added to open
    REFERENCE : BFS Algorithm implemention based from the lectures of Dr. Deepak Khemani
    Link : https://www.youtube.com/watch?v=QqdN7LH3jeA
    */

    node open_var; /*an intermediate variable to package data before pushing into a vector*/
    int open_front_index = 0;
    int current_node;
    unsigned int i;
    vector<node> open;
    vector<node> closed;
    open_var.val = start_node;
    open_var.parent = -1; /* Starting node has parent -1*/
    open.push_back(open_var);
    while((open.size()-open_front_index)>0){
        current_node = open[open_front_index].val;

        if (current_node == goal_node){
            closed.push_back(open[open_front_index]);
            trace_path(closed);   
            return 0;
        }
        else{
            closed.push_back(open[open_front_index]);
        }
        for(i=0;i<edge_pairs.size();i++){

            if((edge_pairs[i].left == current_node)&&(!find_in_open(edge_pairs[i].right,open,open_front_index))&&(!find_in_closed(edge_pairs[i].right,closed))){   
                open_var.val = edge_pairs[i].right;
                open_var.parent = current_node;       
                open.push_back(open_var);
            }
            if((edge_pairs[i].right == current_node)&&(!find_in_open(edge_pairs[i].left,open, open_front_index))&&(!find_in_closed(edge_pairs[i].left,closed))){
                open_var.val = edge_pairs[i].left;
                open_var.parent = current_node;       
                open.push_back(open_var);
            }
        }
        open_front_index++;
    }
    return 1;
}





int main(int argc, char** argv) {
//Variables declarations
        string vertices,edges,cmd,edge_stream,line;
        ifstream mfile(argv[0]);
        string e = "E";
        string v = "V";
        int vertices_max, start_node, end_node, new_line;
        vector<edge_pair> eps;
        vector<node> vec;
        int ret;
        bool vertex_received = false;
        bool edge_received = false;
       // bool err_ = false;
    //user input part
        getline(cin,line);
        while (!std::cin.eof()) {
            //Getting the input Vertex. Checking for error and storing the maximum value and assigning the same number of memory
            std::istringstream line_ip(line);
            char cmd_action;
            line_ip>>cmd_action;
            if(new_line != 1){
            switch(cmd_action){
                case 'V':
                    line_ip>>vertices_max;
                    if (vertices_max <= 1){
                        std::cerr<<"Error: Vertex cannot be less than or equal to 1"<<endl;
                        //err_ = true;
                        break;
                    }
                    vertex_received = true;
                    edge_received = false;
                    break;
                case 'E':
                    
                    if(!vertex_received){
                        cerr<<"Error: Vertex not received"<<endl;
                        //err_ = true;
                        break;
                    }
                    line_ip>>edge_stream;
                    eps.clear();
                    ret = parse_input_edges(vertices_max, edge_stream, &eps);
                    if (ret!=0){
                        //err_ = true;
                        break;
                    }
                    vertex_received = false;
                    edge_received = true;
                    break;
                case 's':
                    if(!edge_received){
                        //err_ = true;
                        std::cerr<<"Error: V or E input not found"<<endl;
                        break;
                    }
                    line_ip>>start_node;
                    line_ip>>end_node;
                    ret = node_check(start_node,end_node,eps);
                    if(ret!=0){
                        break;
                    }
                    ret = find_shortest_path(start_node,end_node,eps);
                    if (ret!=0){
                        std::cerr<<"Error: Path does not exist"<<endl;
                        break;
                    }
                    break;
                default:
                    if (line.size()>0){
                    cerr<<"Error: Unknown command"<<endl;
                    break;
                    }
            }
            }
            new_line = 0;
            getline(cin,line);
            if(line.size()<=1){
                new_line = 1;
            }
        } 
        return 0;
}
