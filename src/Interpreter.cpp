#include <sstream>

#include "../include/Interpreter.h"

using namespace std;

Interpreter::Interpreter(ostream& out_stream)
    : out_stream(out_stream)
{
    position = 0;
    current_base = "dec";
}

void Interpreter::evaluate(const vector<string>& tokens)
{
    this->tokens = tokens;
    position = 0;

    string primary = peek();

    if (primary == "config")
    {
        consume("config");
        config();
    }
    else if (primary == "print")
    {
        consume("print");
        print();
    }
    else
    {
        assign();
    }
}

void Interpreter::config()
{
    if (tokens.size() != 2)
        throw std::runtime_error("Wrong number of arguments for config statement.");

    string value = peek();

    if (value != "dec" && value != "hex" && value != "bin")
        throw std::runtime_error(value + " is not a permitted value for config statement.");

    current_base = value;
    consume(value);
}

void Interpreter::print()
{
    int value = parse_MathExp();

    if (current_base == "bin")
        out_stream << bitset<32>(value).to_string() << endl;
    else if (current_base == "hex")
        out_stream << hex << "0x" << value << endl;
    else 
        out_stream << dec << value << endl;
}

void Interpreter::assign()
{
    string var_name = peek();

    if (!is_variable(var_name))
        throw runtime_error("Variable must begin with a letter and contain letters and digits only.");

    consume(var_name);
    consume("=");

    int value = parse_MathExp();
    symbol_table[var_name] = value;
}

int Interpreter::parse_MathExp()
{
    return parse_SumExp();
}

int Interpreter::parse_SumExp()
{
    int result = parse_ProductExp();
    string next_token = peek();

    while (1)
    {
        if (next_token == "+")
        {
            consume("+");
            result += parse_ProductExp();
        }
        else if (next_token == "-")
        {
            consume("-");
            result -= parse_ProductExp();
        }
        else break;

        next_token = peek();
    }

    return result;
}

int Interpreter::parse_ProductExp()
{
    int result = parse_PrimaryExp();
    string next_token = peek();

    while (1)
    {
        if (next_token == "*")
        {
            consume("*");
            result *= parse_PrimaryExp();
        }
        else if (next_token == "/")
        {
            consume("/");
            int value = parse_PrimaryExp();

            if (value == 0)
                throw runtime_error("Attempted division by zero.");

            result /= value;
        }
        else break;

        next_token = peek();
    }

    return result;
}

int Interpreter::parse_PrimaryExp()
{
    string next_token = peek();

    if (next_token == "(")
    {
        consume(next_token);
        int value = parse_MathExp();

        next_token = peek();

        if (next_token != ")")
            throw runtime_error("Parentheses must be closed.");

        consume(next_token);
        return value;
    }
    else if (is_number(next_token))
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

bool Interpreter::is_number(string value)
{
    if (value[0] != '-' && !isdigit(value[0]))
        return false;

    for (int i = 1; i < value.length(); i++)
        if (!isdigit(value[i]))
            return false;

    return true;
}

bool Interpreter::is_variable(string value)
{
    if (!isalpha(value[0]))
        return false;

    if (value.size() > 1)
        for (int i = 1; i < value.size(); i++)
            if (!isalpha(value[i]) && !isdigit(value[i]))
                return false;
    return true;
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

string Interpreter::peek()
{
    return peek(0);
}

string Interpreter::peek(int steps)
{
    if (position + steps >= tokens.size()) 
        return ETX;
    return tokens[position + steps];
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

