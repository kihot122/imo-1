#include "parser.hpp"
#include "alg1.hpp"
#include "alg2.hpp"
#include "alg3.hpp"

#include <iostream>

int main(int argc, char** argv)
{  
    std::vector<std::vector<int>> testV {
        {0,2,3,4}, 
        {2,0,3,5}, 
        {3,3,0,6},
        {4,5,6,0}};

    
    std::vector<int> res = Alg1(testV, 0, 1);
    for (int e : res)
    {
        std::cout << e << " ";
    }


}