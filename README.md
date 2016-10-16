# SatPi

Satisfiability programming interface written in C++.

* Generates random K-SAT formulas in DIMACS format.

### Usage

For example, to generate a random 3-SAT formula with 5000 variables with a clause-to-variable ratio of 4.24 to the file myformula.cnf, run the following:
```
satpi.exe generate -numVariables 5000 -literalsPerClause 3 -clausesPerVariableRatio 4.24 -outputFilepath myformula.cnf
```

All work Copyright (c) 2016 Richard Russell. (https://rars.github.io).
