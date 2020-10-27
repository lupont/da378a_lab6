# da378a_lab6
Interpreter for the C@ language, laboration #6 for DA378A at MAU.

## Building
Execute `make dirs && make` to build an executable.

## Running
Running the generated `bin/lab6` without arguments puts you in a REPL mode. In this mode, you can write one line of valid C@ code at a time.

Running it with a path to a text file will interpret the entire file and write its evaluated results to standard out.

Running it with two paths will interpret the first file, and write its evaluated results to the second file.

### Example
`bin/lab6` puts you in the REPL mode.
`bin/lab6 example/example.cat` runs the provided example program.
`bin/lab6 example/example.cat output.txt` runs the provided example program and writes the output to a file called `output.txt`.

