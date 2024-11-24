# Sudoku Solver

Puzzle Sudoku solver implemented in C using a backtracking algorithm.

## Features

- Automatically solves Sudoku puzzles.
- Supports visualization of the solving process.
- Allows manual input or generates puzzles programmatically.

## How to Run

1. Clone the repository:
```
bash
git clone https://github.com/andrew52522/sudoku_solver_2
cd sudoku_solver
```
2. Compile the programm:
```
    gcc main.c -o sudoku_solver
```
3. Run the programm:
```
 ./sudoku_solver
```

## Input Format
```
The program supports two modes:

Manual Mode: User solves the puzzle manually.
Automatic Mode: Program solves the puzzle using a backtracking algorithm.
Specify the number of empty cells (0–81) and the mode (0 for manual, 1 for automatic).
```

## Project Structure
- `main.c`: Core implementation of the Sudoku solver.
- `README.md`: Documentation.

## Check how works automatic solving

[[Download and watch demo.webm](https://github.com/andrew52522/sudoku_solver_2/blob/main/gif_sudoku.webm)](demo.webm)

