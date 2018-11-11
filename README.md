# SatPi

Satisfiability programming interface written in C++.

- Generates random K-SAT formulas in DIMACS format.

### Usage

For example, to generate a random 3-SAT formula with 5000 variables with a clause-to-variable ratio of 4.24 to the file myformula.cnf, run the following:

```
satpi.exe generate -numVariables 5000 -literalsPerClause 3 -clausesPerVariableRatio 4.24 -outputFilepath myformula.cnf
```

All work Copyright (c) 2016 Richard Russell. (https://rars.github.io).

## Build

### Linux

This has been tested with CMake-3.12.4. Run the following commands from a bash shell:

```
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install
make
make install
cd ..
install/bin/satpi generate -numVariables 5000 -literalsPerClause 3 -clausesPerVariableRatio 4.24 -outputFilepath myformula.cnf
```

The binary should be output at `install/bin/satpi`.
