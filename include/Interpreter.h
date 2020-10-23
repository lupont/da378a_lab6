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
    std::ostream& out_stream;
    std::unordered_map<std::string, int> symbol_table;
    std::vector<std::string> tokens;
    std::string current_base;

    bool is_variable(std::string);
    bool is_number(std::string);

    void print();
    void config();
    void assign();

    int parse_MathExp();
    int parse_SumExp();
    int parse_ProductExp();
    int parse_PrimaryExp();
    
public:
    Interpreter(std::ostream& out_stream);

    void evaluate(const std::vector<std::string>& tokens);

    std::vector<std::string> tokenize(const std::string& line);

    std::string peek();
    std::string peek(int);

    void consume(const std::string& token);

    std::string to_string();
};

#endif

