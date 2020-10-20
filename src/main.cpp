#include <iostream>

#include "../include/Interpreter.h"

using namespace std;

int main()
{
    Interpreter it(cout);

    vector<string> tokens = it.tokenize("print ( 3 + 3 ) * 3");

    for (auto t : tokens)
        std::cout << t << std::endl;
}
