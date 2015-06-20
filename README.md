# sudokuSolver
A fast sudoku solver in C++

# Instructions

## Build

```
$ git clone git@github.com:voltrevo/sudokuSolver.git
$ cd sudokuSolver
$ clang++ sudokuSolver.cpp -O3 -std=c++11 -o sudokuSolver
```

## Run

I've included a large collection of 17-clue sudoku puzzles as `sudoku17.txt`, which can also be found [here](http://staffhome.ecm.uwa.edu.au/~00013890/sudoku17). Each line of this file describes separate puzzle, with zeros for empty cells and numbers for provided clues. I usually use the `head` command to pull out individual puzzles:

```
$ head -n1 sudoku17.txt
000000010400000000020000000000050407008000300001090000300400200050100000000806000
```

The compiled binary simply reads this format from `stdin` and writes solutions in this format to `stdout`:

```
$ head -n1 sudoku17.txt | ./sudokuSolver
693784512487512936125963874932651487568247391741398625319475268856129743274836159
```

On my machine (13" Macbook Pro 2014), it solves the first 100 of these puzzles in an average of 3.8 milliseconds each:

```
$ time head -n100 sudoku17.txt | ./sudokuSolver >/dev/null
real  0m0.379s
user  0m0.371s
sys   0m0.007s
```
