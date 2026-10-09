/*
Program: EECS 348 Assignment 4
Description: Solves Sudoku puzzles using recursion and backtracking
Inputs: puzzle1.txt, puzzle2.txt, puzzle3.txt, puzzle4.txt, puzzle5.txt
Outputs: Original puzzles, solutions, and number of solutions found
Collaborators: None
Other Sources: Microsoft Copilot and Grok and Chatgpt for the validation part
Author: Raika Zolfagharimanesh
Creation Date: October 8, 2026
*/

#include <iostream> // lets us use cout and endl
#include <fstream> // lets us read the puzzle files
#include <string> // lets us use strings

using namespace std; // so we don't have to write std every time

// class that stores and solves the sudoku puzzle
// based on the Copilot code with changes made by me
class Sudoku {

private:

    char board[9][9]; // store the 9 by 9 sudoku board
    int solutionCount; // store how many solutions we found

    // check if a number can go in this spot
    // based on Copilot code with changes made by me
    bool isValid(int row, int col, char num) {

        // go through the row
        for (int c = 0; c < 9; c++) {

            // check if the number is already in the row
            if (board[row][c] == num) {
                return false; // number can't go here
            }
        }

        // go through the column
        for (int r = 0; r < 9; r++) {

            // check if the number is already in the column
            if (board[r][col] == num) {
                return false; // number can't go here
            }
        }

        // find where the 3 by 3 block starts
        int startRow = (row / 3) * 3;
        int startCol = (col / 3) * 3;

        // go through the 3 by 3 block
        for (int r = startRow; r < startRow + 3; r++) {

            for (int c = startCol; c < startCol + 3; c++) {

                // check if the number is already in the block
                if (board[r][c] == num) {
                    return false; // number can't go here
                }
            }
        }

        return true; // number is good to use
    }

    // find an empty spot that has the least possible choices
    // this is one of the changes I made to make the search faster
    bool findBestEmptyCell(int &bestRow, int &bestCol) {

        int smallestChoices = 10; // start above the max possible choices
        bool foundEmpty = false; // keep track if we find an empty spot

        // go through all the rows
        for (int row = 0; row < 9; row++) {

            // go through all the columns
            for (int col = 0; col < 9; col++) {

                // only do this if the spot is empty
                if (board[row][col] == '_') {

                    foundEmpty = true; // we found an empty spot
                    int choices = 0; // count how many numbers can go here

                    // try numbers 1 through 9
                    for (char num = '1'; num <= '9'; num++) {

                        // count it if the number works here
                        if (isValid(row, col, num)) {
                            choices++;
                        }
                    }

                    // save this spot if it has less choices
                    if (choices < smallestChoices) {
                        smallestChoices = choices;
                        bestRow = row;
                        bestCol = col;
                    }

                    // if nothing can go here this path won't work
                    if (smallestChoices == 0) {
                        return true;
                    }
                }
            }
        }

        return foundEmpty; // tell if an empty spot was found
    }

    // solve the puzzle using recursion and backtracking
    // based on Copilot code with changes made by me
    void solveRecursive() {

        int row = -1; // store the row we want to work on
        int col = -1; // store the column we want to work on

        // find the best empty spot
        bool emptyFound = findBestEmptyCell(row, col);

        // if there are no empty spots the puzzle is solved
        if (!emptyFound) {

            solutionCount++; // add one to number of solutions

            // print which solution this is
            cout << "\nSolution " << solutionCount << ":" << endl;

            printBoard(); // print the solved board

            return; // go back to look for other solutions
        }

        // try every number from 1 to 9
        for (char num = '1'; num <= '9'; num++) {

            // only use the number if it works in this spot
            if (isValid(row, col, num)) {

                board[row][col] = num; // put the number in the spot

                solveRecursive(); // keep solving from here

                board[row][col] = '_'; // undo it so we can try another way
            }
        }
    }

public:

    // constructor for a new sudoku object
    Sudoku() {

        solutionCount = 0; // start with no solutions

        // go through the whole board
        for (int row = 0; row < 9; row++) {

            for (int col = 0; col < 9; col++) {

                board[row][col] = '_'; // start every spot as empty
            }
        }
    }

    // read a sudoku puzzle from a file
    // based on Copilot code with extra checks added by me
    bool loadPuzzle(const string &filename) {

        ifstream file(filename); // open the puzzle file

        // check if the file actually opened
        if (!file.is_open()) {

            cout << "Could not open " << filename << endl; // show the error

            return false; // file did not work
        }

        char value; // store each value we read

        // go through all 9 rows
        for (int row = 0; row < 9; row++) {

            // go through all 9 columns
            for (int col = 0; col < 9; col++) {

                // try to read the next value
                if (!(file >> value)) {

                    cout << "Invalid puzzle file." << endl; // not enough values

                    return false;
                }

                // make sure the value is 1-9 or an underscore
                if ((value < '1' || value > '9') && value != '_') {

                    cout << "Invalid character in puzzle file." << endl;

                    return false; // bad input
                }

                board[row][col] = value; // put the value into the board
            }
        }

        // check if there is extra stuff after the 81 values
        if (file >> value) {

            cout << "Too many values in puzzle file." << endl;

            return false;
        }

        file.close(); // close the file

        return true; // puzzle loaded correctly
    }

    // print the sudoku board
    // based on Copilot code
    void printBoard() const {

        // go through every row
        for (int row = 0; row < 9; row++) {

            // go through every column
            for (int col = 0; col < 9; col++) {

                cout << board[row][col]; // print the value

                // add a space unless this is the last value
                if (col < 8) {

                    cout << " ";
                }
            }

            cout << endl; // move to the next row
        }
    }

    // start solving the puzzle
    void solve() {

        solutionCount = 0; // reset number of solutions

        solveRecursive(); // start the recursive search

        // check if we didn't find any solutions
        if (solutionCount == 0) {

            cout << "\nNo solution found" << endl;
        }

        // show the total number of solutions
        cout << "\nSolutions found: " << solutionCount << endl;
    }
};

// main part of the program
// goes through all five puzzle files
int main() {

    // store the names of all the puzzle files
    string files[5] = {
        "puzzle1.txt",
        "puzzle2.txt",
        "puzzle3.txt",
        "puzzle4.txt",
        "puzzle5.txt"
    };

    // go through all five puzzles
    for (int i = 0; i < 5; i++) {

        Sudoku puzzle; // make a sudoku object for this puzzle

        // show which puzzle we are working on
        cout << "\n====================================" << endl;
        cout << "Puzzle File: " << files[i] << endl;
        cout << "====================================" << endl;

        // try to load the puzzle
        if (!puzzle.loadPuzzle(files[i])) {

            cout << "Could not load puzzle." << endl;

            continue; // skip this one if the file didn't work
        }

        // show the puzzle before solving it
        cout << "\nOriginal Puzzle:" << endl;

        puzzle.printBoard(); // print original puzzle

        puzzle.solve(); // solve it and print all the solutions
    }

    return 0; // program is done
}
