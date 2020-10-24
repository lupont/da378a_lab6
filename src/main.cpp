#include <iostream>
#include <fstream>

#include "../include/Interpreter.h"

using namespace std;

/**
 * Creates a REPL (Read, Evaluate, Print, Loop) in the C@ language.
 * The user may input any line of valid C@ code and see it evaluated
 * in an interactive environment. If the input is invalid, the
 * REPL will catch the error and prind the error message.
 * To exit the REPL, ^D may be used.
 */
void repl()
{
    // Print some information about the REPL environment.
    cout << "Welcome to the C@ REPL!" << endl;
    cout << "Simply input a line and have it evaluate and print." << endl;
    cout << "When you're done, ^D exits the REPL." << endl << endl;

    // Create an Interpreter that writes to standard out.
    Interpreter it(cout);
    string line;

    // Prompt
    cout << ">> ";

    // As long as the program is not interrupted...
    while (getline(cin, line))
    {
        // ...get the input line and attempt to interpret it.
        try
        {
            it.evaluate(it.tokenize(line));
        }

        // In case of an error, write the error message.
        catch (runtime_error e) 
        {
            cout << e.what() << endl;
        }

        // Prompt
        cout << ">> ";
    }
}

/**
 * Reads a file of C@ code and interprets it. Writes the
 * output to the given stream.
 * @param file_path The path to the file to read.
 * @param out_stream The stream to write the output to.
 */
void read_file(const char* file_path, ostream& out_stream)
{
    // Create an Interpreter that writes to the given stream.
    Interpreter it(out_stream);

    // Create an input stream over the given file path.
    ifstream file(file_path);
    string line;

    // If the input stream could not be opened, exit.
    if (!file.is_open())
        return;
    
    // Read each line in order and interpret it.
    while (getline(file, line))
        it.evaluate(it.tokenize(line));

    // Close the input stream.
    if (file.is_open())
        file.close();
}

/**
 * Runs the Interpreter in one of two main ways:
 * 1. In REPL mode. This is launched if no
 *    arguments were supplied.
 * 2. In read mode. Reads a file of valid
 *    C@ code and writes either to standard
 *    out, or a given file path.
 */
int main(int argc, char** argv)
{
    // If 2 arguments were supplied, assume they are
    // the paths to the input file and output file,
    // respectively.
    if (argc == 3)
    {
        // Create an output stream to the second argument.
        ofstream output_file(argv[2]);

        // Send the first argument (input file path)
        // and output stream to be read.
        read_file(argv[1], output_file);

        // Close the output stream if it is opened.
        if (output_file.is_open())
            output_file.close();
    }

    // If one argument was supplied, assume it is
    // the path to the input file.
    else if (argc == 2)
        // In which case output will be written
        // to standard out.
        read_file(argv[1], cout);

    // If no arguments were supplied, launch the REPL.
    else
        repl();
}

