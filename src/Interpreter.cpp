#include <sstream>

#include "../include/Interpreter.h"

using namespace std;

Interpreter::Interpreter(ostream& out_stream)
    : out_stream(out_stream)
{
    position = 0;
    current_base = DEC;
}

bool Interpreter::is_int(const string value) const
{
    // If the first character is neither a negation sign
    // nor a digit, or if the value is just a negation sign,
    // the value is not an integer.
    if (value[0] != '-' && !isdigit(value[0]) || value == "-")
        return false;

    // Loop through the rest of the characters...
    for (int i = 1; i < value.length(); i++)
        // ...if the character at the current position
        // is not a digit, the value is not an integer.
        if (!isdigit(value[i]))
            return false;

    // If the value contains only digits and/or
    // starts with a negation sign, it is a 
    // valid integer.
    return true;
}

bool Interpreter::is_variable(const string value) const
{
    // If the first character is not a letter in the alphabet,
    // it is not a valid variable name.
    if (!isalpha(value[0]))
        return false;

    if (value.size() > 1)
        // Iterate over the rest of the characters...
        for (int i = 1; i < value.size(); i++)
            // ...if the character is neither a letter nor
            // a digit, it is not a valid variable name.
            if (!isalpha(value[i]) && !isdigit(value[i]))
                return false;

    // If the value starst with a letter and contains only 
    // letters and numbers, it is a valid variable name.
    return true;
}

void Interpreter::evaluate(const vector<string>& tokens)
{
    // Save the supplied line of tokenized C@ code.
    this->tokens = tokens;

    // Reset the position to 0.
    position = 0;

    // Get the first token.
    string primary = peek();

    // If it is 'config'...
    if (primary == "config")
    {
        // ...consume and parse the config statement.
        consume(primary);
        parse_config_stmt();
    }

    // If it is 'print'...
    else if (primary == "print")
    {
        // ...consume and parse the print statement.
        consume(primary);
        parse_print_stmt();
    }

    // Otherwise...
    else
    {
        // ...parse the assignment statement.
        parse_assg_stmt();
    }
}

void Interpreter::parse_config_stmt()
{
    // If there are more than 2 tokens, it is an
    // invalid config statement.
    if (tokens.size() != 2)
        throw std::runtime_error("Wrong number of arguments for config statement.");

    string value = peek();

    // If the value of the config statement is
    // neither dec, hex, nor bin - it is invalid.
    if (value != DEC && value != HEX && value != BIN)
        throw std::runtime_error(value + " is not a permitted value for config statement.");

    // Otherwise, set the current base to the value
    // and consume.
    current_base = value;
    consume(value);
}

void Interpreter::parse_print_stmt()
{
    // Parse the expression.
    int value = parse_math_exp();

    // Print according to the current base,
    // as set in a config statement.
    if (current_base == "bin")
        out_stream << bitset<32>(value).to_string() << endl;
    else if (current_base == "hex")
        out_stream << hex << "0x" << value << endl;
    else 
        out_stream << dec << value << endl;
}

void Interpreter::parse_assg_stmt()
{
    // The first token contains the variable name.
    string var_name = peek();

    // If it does not conform to the valid C@ 
    // variable naming rules, throw an error.
    if (!is_variable(var_name))
        throw runtime_error("Variable must begin with a letter and contain letters and digits only.");

    // Consume the variable name token.
    consume(var_name);

    // If the next token is not an equals sign,
    // it is not a valid assignment statement.
    if (peek() != "=")
        throw runtime_error("Assignment must follow a variable with an equals sign.");

    // Consume the equals sign.
    consume("=");

    // Parse the value that should be assigned
    // to the variable.
    int value = parse_math_exp();

    // Assign the value to the variable.
    symbol_table[var_name] = value;
}

int Interpreter::parse_math_exp()
{
    // A math expression is just a sum
    // expression.
    return parse_sum_exp();
}

int Interpreter::parse_sum_exp()
{
    // Parse the left hand side, which is
    // a product expression.
    int result = parse_product_exp();

    // Get the next token.
    string next_token = peek();

    // One or more times...
    while (1)
    {
        // ...if the next token is plus...
        if (next_token == "+")
        {
            // ...consume the plus sign and 
            // add the right hand side 
            // expression to the result.
            consume(next_token);
            result += parse_product_exp();
        }

        // ...if it is a minus...
        else if (next_token == "-")
        {
            // ...consume the minus sign and
            // subtract the right hand side
            // expression from the result.
            consume(next_token);
            result -= parse_product_exp();
        }

        // ...otherwise we are done.
        else break;

        // Get the next token in preparation
        // for the next expression.
        next_token = peek();
    }

    return result;
}

int Interpreter::parse_product_exp()
{
    // Parse the left hand side, which is
    // a product expression.
    int result = parse_primary_exp();

    // Get the next token.
    string next_token = peek();

    // One or more times...
    while (1)
    {
        // ...if the next token is times...
        if (next_token == "*")
        {
            // ...consume the multiplication 
            // sign and multiply the right hand
            // side expression to the result.
            consume(next_token);
            result *= parse_primary_exp();
        }

        // ...if it is a slash...
        else if (next_token == "/")
        {
            // ...consume the division 
            // sign and calculate the right
            // hand side expression.
            consume(next_token);
            int value = parse_primary_exp();

            // If the value is zero, throw an error
            // for attemping to divide by zero.
            if (value == 0)
                throw runtime_error("Attempted division by zero.");

            // Divide the result by the value.
            result /= value;
        }

        // ...otherwise, we are done.
        else break;

        // Get the next token in preparation
        // for the next expression.
        next_token = peek();
    }

    return result;
}

int Interpreter::parse_primary_exp()
{
    // Get the first token of the expession.
    string next_token = peek();

    // If it is an opening parentheses...
    if (next_token == "(")
    {
        // ...consume it.
        consume(next_token);

        // Parse the expression between the parentheses.
        int value = parse_math_exp();

        next_token = peek();

        // If the following token is not a closing
        // parentheses, it is invalid syntax.
        if (next_token != ")")
            throw runtime_error("Parentheses must be closed.");

        // Consume the closing parentheses.
        consume(next_token);
        return value;
    }

    // If it is an integer...
    else if (is_int(next_token))
    {
        // ...consume and return it.
        consume(next_token);
        return stoi(next_token);
    }

    // If it is a variable name...
    else if (is_variable(next_token))
    {
        // ...throw if the variable is not set.
        if (symbol_table.find(next_token) == symbol_table.end())
            throw runtime_error("Variable not set.");

        // Otherwise consume the token
        // and return the variable's value.
        consume(next_token);
        return symbol_table[next_token];
    }

    // If we get this far without matching,
    // something has gone wrong.
    throw runtime_error("Primary must be a math expression in parentheses, an integer, or a variable.");
}

vector<string> Interpreter::tokenize(const string& line)
{
    // The vector containing the tokenized C@ code.
    vector<string> tokens;

    // A stream over the line, to be able to
    // use the 'getline' function.
    stringstream stream(line);

    // The current token in the line.
    string token;

    // While there are tokens left to be
    // parsed, save it to the 'token' variable.
    // Split the string on spaces.
    while (getline(stream, token, ' '))
        // Add the current token to the vector.
        tokens.push_back(token);

    return tokens;
}

string Interpreter::peek() const
{
    // If the position has reached the end,
    // no tokens are left.
    if (position >= tokens.size()) 
        return ETX;

    // Otherwise, return the current token.
    return tokens[position];
}

void Interpreter::consume(const string& token)
{
    // Get the current token.
    string next_token = peek();

    // If it is at the end, throw because
    // there are no tokens to consume.
    if (next_token == ETX)
        throw std::runtime_error("Consumed past last token\n");

    // If the next token in the vector does
    // not match the supplied one, throw 
    // because something has gone wrong.
    if (next_token != token)
        throw std::runtime_error("Could not consume token " + token + "\n");

    // Advance the current position.
    ++position;
}

