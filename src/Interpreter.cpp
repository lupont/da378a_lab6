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
    if (value[0] != '-' && !isdigit(value[0]))
        return false;

    for (int i = 1; i < value.length(); i++)
        if (!isdigit(value[i]))
            return false;

    return true;
}

bool Interpreter::is_variable(const string value) const
{
    if (!isalpha(value[0]))
        return false;

    if (value.size() > 1)
        for (int i = 1; i < value.size(); i++)
            if (!isalpha(value[i]) && !isdigit(value[i]))
                return false;
    return true;
}

void Interpreter::evaluate(const vector<string>& tokens)
{
    this->tokens = tokens;
    position = 0;

    string primary = peek();

    if (primary == "config")
    {
        consume(primary);
        parse_config_stmt();
    }
    else if (primary == "print")
    {
        consume(primary);
        parse_print_stmt();
    }
    else
    {
        parse_assg_stmt();
    }
}

void Interpreter::parse_config_stmt()
{
    if (tokens.size() != 2)
        throw std::runtime_error("Wrong number of arguments for config statement.");

    string value = peek();

    if (value != DEC && value != HEX && value != BIN)
        throw std::runtime_error(value + " is not a permitted value for config statement.");

    current_base = value;
    consume(value);
}

void Interpreter::parse_print_stmt()
{
    int value = parse_math_exp();

    if (current_base == "bin")
        out_stream << bitset<32>(value).to_string() << endl;
    else if (current_base == "hex")
        out_stream << hex << "0x" << value << endl;
    else 
        out_stream << dec << value << endl;
}

void Interpreter::parse_assg_stmt()
{
    string var_name = peek();

    if (!is_variable(var_name))
        throw runtime_error("Variable must begin with a letter and contain letters and digits only.");

    consume(var_name);
    consume("=");

    int value = parse_math_exp();
    symbol_table[var_name] = value;
}

int Interpreter::parse_math_exp()
{
    return parse_sum_exp();
}

int Interpreter::parse_sum_exp()
{
    int result = parse_product_exp();
    string next_token = peek();

    while (1)
    {
        if (next_token == "+")
        {
            consume("+");
            result += parse_product_exp();
        }
        else if (next_token == "-")
        {
            consume("-");
            result -= parse_product_exp();
        }
        else break;

        next_token = peek();
    }

    return result;
}

int Interpreter::parse_product_exp()
{
    int result = parse_primary_exp();
    string next_token = peek();

    while (1)
    {
        if (next_token == "*")
        {
            consume("*");
            result *= parse_primary_exp();
        }
        else if (next_token == "/")
        {
            consume("/");
            int value = parse_primary_exp();

            if (value == 0)
                throw runtime_error("Attempted division by zero.");

            result /= value;
        }
        else break;

        next_token = peek();
    }

    return result;
}

int Interpreter::parse_primary_exp()
{
    string next_token = peek();

    if (next_token == "(")
    {
        consume(next_token);
        int value = parse_math_exp();

        next_token = peek();

        if (next_token != ")")
            throw runtime_error("Parentheses must be closed.");

        consume(next_token);
        return value;
    }
    else if (is_int(next_token))
    {
        consume(next_token);
        return stoi(next_token);
    }
    else if (is_variable(next_token))
    {
        if (symbol_table.find(next_token) == symbol_table.end())
            throw runtime_error("Variable not set.");

        consume(next_token);
        return symbol_table[next_token];
    }

    throw runtime_error("Primary must be a math expression in parentheses, an integer, or a variable.");
}

vector<string> Interpreter::tokenize(const string& line)
{
    vector<string> tokens;
    stringstream stream(line);
    string token;

    while (getline(stream, token, ' '))
        tokens.push_back(token);

    return tokens;
}

string Interpreter::peek() const
{
    if (position >= tokens.size()) 
        return ETX;
    return tokens[position];
}

void Interpreter::consume(const string& token)
{
    string next_token = peek();

    if (next_token == ETX)
        throw std::runtime_error("Consumed past last token\n");

    if (next_token != token)
        throw std::runtime_error("Could not consume token " + token + "\n");

    ++position;
}

