#ifndef INTERPRETER
#define INTERPRETER

#include <ostream>
#include <string>
#include <vector>
#include <unordered_map>

/**
 * An interpreter for the C@ language. Contains variables
 * with their values, as well as the current base to print
 * in. Also keeps track of the current, tokenized line of
 * C@ code.
 */
class Interpreter
{
private:
    // These constants are used in the implementation.
    const std::string ETX = "\u0003";
    const std::string DEC = "dec";
    const std::string HEX = "hex";
    const std::string BIN = "bin";

    // Keeps track of the current token.
    int position;

    // The output stream to write to.
    std::ostream& out_stream;

    // The map of variables, name to value.
    std::unordered_map<std::string, int> symbol_table;

    // The current line of tokenized C@ code.
    std::vector<std::string> tokens;

    // The base to print in.
    std::string current_base;


    /**
     * Decides whether a string contains an integer value or not.
     */
    bool is_int(const std::string) const;

    /**
     * Decides whether a string is a valid C@ variable name or not.
     */
    bool is_variable(const std::string) const;

    /**
     * Parses a config statement.
     */
    void parse_config_stmt();

    /**
     * Parses a print statement.
     */
    void parse_print_stmt();

    /**
     * Parses a assignment statement.
     */
    void parse_assg_stmt();


    /**
     * Parses a math expression.
     */
    int parse_math_exp();

    /**
     * Parses a sum expression.
     */
    int parse_sum_exp();

    /**
     * Parses a product expression.
     */
    int parse_product_exp();

    /**
     * Parses a primary expression.
     */
    int parse_primary_exp();
    
public:
    /**
     * Constructs a new Interpreter instance
     * which will write to the given output
     * stream.
     */
    Interpreter(std::ostream& out_stream);

    // Delete copy constructor, there exists
    // no reason for it to exist within the
    // scope of this program.
    Interpreter(const Interpreter& rhs) = delete;

    /**
     * Evaluates a line of tokenized C@ code.
     */
    void evaluate(const std::vector<std::string>& tokens);

    /**
     * Tokenizes a line of C@ code.
     */
    std::vector<std::string> tokenize(const std::string& line);

    /**
     * Gets the token at the current position.
     */
    std::string peek() const;

    /**
     * Consumes the current token, and advances
     * to the next.
     */
    void consume(const std::string& token);
};

#endif

