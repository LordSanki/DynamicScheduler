DynamicScheduler
================
This project implements a simulator for dynamic instruction scheduling in an OOO pipeline using Tamasulo algorithm.

How to build
-------------
```bash
$> make
```

How to run unit tests
----------------
The script runs the binary with predefined test inputs and diffs the result with expected output files.
If test are successfull no output will be displayed else difference is printed to the screen.
```bash
$>./verify.sh
```

How to run simulator
--------------------
* Make  generates the project binary "sim"
```bash 
./sim
```
* Application will prompt with list of expected arguments
* Specify the arguments in correct order and run the applicatio again
* Results are printed to console hence you can redirect it to a file for future use
* Have a look at verify.sh for sample inputs
```bash
$> ./sim <arguments> > Result.txt
```

Specs
-------
proj3.pdf file is has the specifications of the algorithm

