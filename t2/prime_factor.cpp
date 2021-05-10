#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

vector<int> sort_vector(vector<int> vec){
  int min, min_index=0;
  vector<int> sorted_vec;
  unsigned int i=0;
  min = vec[0];
  while(vec.size()>0){
    min = vec[0];
    min_index = 0;
    for(i=0;i<vec.size();i++){
      if(vec[i]<min){
        min=vec[i];
        min_index = i;
      }
    }
    sorted_vec.push_back(min);
    vec.erase(vec.begin()+min_index);
  }
  return sorted_vec;
}

int print_vector(vector<int> obj){
    /*Prints a node in suitable form. Used for testing*/
    unsigned int i;
    for(i=0;i<obj.size();i++){
        cout<<obj[i]<<" ";
    }
    return 0;
}

bool isprime(int val){
  unsigned int i=2;
  int max;
  max = (val/2)+1; //it is not necessary to search all the way. It is sufficient to search until half of the number +1 just in case
  for(;i<max;i++){
    if((val%i)==0){
      return false;
    }
  }
  return true;
}

std::string prime_factor(unsigned x) {
  std::stringstream out;
  vector<int> factors,factors_unsorted;
  unsigned int i, count=1;
  int c_val;
  for(i=2;i<=x;i++){
    if(isprime(i)){
      if((x%i)==0){
        factors_unsorted.push_back(i);
        x=x/i;
        i=1;
      }
    }
  }
  //sort(factors.begin(),factors.end());
  factors = sort_vector(factors_unsorted);
  for(i=0;i<factors.size();i++){
    c_val = factors.at(i);
    for(i=i+1;i<factors.size();i++){
      if(factors.at(i)==c_val){
        count++;
      }
      else{break;}
    }
    i--;
    out<<c_val;
    if(count>1){
      out<<"^"<<count;
      count=1;
    }
    if((i+1)<factors.size()){
      out<<" x ";
    }
  }

  // YOUR CODE GOES HERE
  return out.str();
}


int main() {

  auto input = 6;
  auto output = prime_factor(input);

  std::cout << "Prime factorization of " << input << " is " << output
            << std::endl;
  // expected output: 3^2

  input = 8320;
  output = prime_factor(input);

  std::cout << "Prime factorization of " << input << " is " << output
            << std::endl;
  // expected output: 2^4 x 3 x 5
  return 0;
}
