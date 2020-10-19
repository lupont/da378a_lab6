#include <iostream>

class Interpreter
{
    
public:
    Interpreter(std::ostream& out_stream);

    void evaluate(const std::vector<std::string>& tokens);
};
