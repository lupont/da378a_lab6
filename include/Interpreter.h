#ifndef INTERPRETER
#define INTERPRETER

#include <ostream>
#include <string>
#include <vector>
#include <unordered_map>

class Interpreter
{
private:
    const std::string ETX = "\u0003";

    int position;
    std::unordered_map<std::string, int> symbol_table;
    std::vector<std::string> tokens;
    
public:
    Interpreter(std::ostream& out_stream);

    void evaluate(const std::vector<std::string>& tokens);

    void tokenize(const std::string& line);

    std::string peek();
    std::string peek(int);

    void consume(const std::string& token);
};

#endif

