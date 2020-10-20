#include <sstream>

#include "../include/Interpreter.h"

using namespace std;

Interpreter::Interpreter(ostream& out_stream)
{
    position = 0;
}

void Interpreter::evaluate(const vector<string>& tokens)
{
    
}

void Interpreter::tokenize(const string& line)
{
    stringstream stream(line);
    string token;

    while (getline(stream, token, ' ')) {
        tokens.push_back(token);
    }
}

std::string Interpreter::peek()
{
    return peek(0);
}

std::string Interpreter::peek(int steps)
{
    if (position + steps >= tokens.size()) 
        return ETX;
    return tokens[position + steps];
}

void Interpreter::consume(const std::string& token)
{
    string next_token = peek();

    if (next_token == ETX)
        throw std::runtime_error("Consumed past last token\n");

    if (next_token != token)
        throw std::runtime_error("Could not consume token " + token + "\n");

    ++position;
}


