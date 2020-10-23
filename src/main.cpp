#include <iostream>
#include <fstream>

#include "../include/Interpreter.h"

using namespace std;

void repl()
{
    cout << "Welcome to the C@ REPL!" << endl;
    cout << "Simply input a line and have it evaluate and print." << endl;
    cout << "When you're done, ^D exits the REPL." << endl << endl;

    Interpreter it(cout);
    string line;

    cout << ">> ";
    while (getline(cin, line))
    {
        try                     { it.evaluate(it.tokenize(line)); }
        catch (runtime_error e) { cout << e.what() << endl;       }
        cout << ">> ";
    }
}

void read_file(const char* file_path)
{
    Interpreter it(cout);
    ifstream file(file_path);
    string line;

    if (!file.is_open())
        return;
    
    while (getline(file, line))
    {
        it.evaluate(it.tokenize(line));
    }
}

int main(int argc, char** argv)
{
    if (argc == 2)
        read_file(argv[1]);
    else
        repl();
}

