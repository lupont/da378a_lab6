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
    const std::string DEC = "dec";
    const std::string HEX = "hex";
    const std::string BIN = "bin";

    int position;
    std::ostream& out_stream;
    std::unordered_map<std::string, int> symbol_table;
    std::vector<std::string> tokens;
    std::string current_base;

    bool is_int(const std::string) const;
    bool is_variable(const std::string) const;

    void parse_config_stmt();
    void parse_print_stmt();
    void parse_assg_stmt();

    int parse_math_exp();
    int parse_sum_exp();
    int parse_product_exp();
    int parse_primary_exp();
    
public:
    Interpreter(std::ostream& out_stream);

    void evaluate(const std::vector<std::string>& tokens);
    std::vector<std::string> tokenize(const std::string& line);

    std::string peek() const;
    void consume(const std::string& token);
};

#endif

