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

void read_file(const char* file_path, ostream& out_stream)
{
    Interpreter it(out_stream);
    ifstream file(file_path);
    string line;

    if (!file.is_open())
        return;
    
    while (getline(file, line))
        it.evaluate(it.tokenize(line));

    if (file.is_open())
        file.close();
}

int main(int argc, char** argv)
{
    if (argc == 3)
    {
        ofstream output_file(argv[2]);
        read_file(argv[1], output_file);
        if (output_file.is_open())
            output_file.close();
    }
    else if (argc == 2)
        read_file(argv[1], cout);
    else
        repl();
}

