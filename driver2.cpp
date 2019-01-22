
// File: driver2cpp
//
// CMSC 341 Fall 2018 Project 2
//
// Simple driver program to call the Sally Forth interpreter
// This version accepts user input for filename of Sally Forth
// source code.
//
// g++ driver2.cpp Sally.cpp Sally.h -o d2.out
// /afs/umbc.edu/users/p/a/park/pub/cs341/bro11/proj2


#include <iostream>
#include <fstream>
using namespace std ;

#include "Sally.h"

int main() {
   string fname ;

   cout << "Enter file name: " ;
   cin >> fname ;
   ifstream ifile(fname.c_str()) ;

   Sally S(ifile) ;

   S.mainLoop() ;

   ifile.close() ;
   return 0 ;
}
