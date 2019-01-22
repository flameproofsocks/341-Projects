// File: Sally.h
//
// CMSC 341 Fall 2018 Project 2
//
// Class declarations for Sally Forth interpreter
//

#ifndef _SALLY_H_
#define _SALLY_H_

#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <map>
#include <stdexcept>
#include <queue> //used for loops
using namespace std ;


// thrown by lexical parser when end of file reached
//
class EOProgram : public runtime_error {
public:
   EOProgram(const string& what) : runtime_error(what) { }
} ;


enum TokenKind { UNKNOWN, KEYWORD, INTEGER, VARIABLE, STRING } ;


// lexical parser returns a token
// programs are lists of tokens
//
class Token {

public:

   Token(TokenKind kind=UNKNOWN, int val=0, string txt="" ) ;
   TokenKind m_kind ;
   int m_value ;      // if it's a known numeric value
   string m_text ;    // original text that created this token

} ;



// type of a C++ function that does the work
// of a Sally Forth operation.
//
class Sally ;
typedef void (* operation_t)(Sally *Sptr) ;



// entry in the symbol table has this type
//
class SymTabEntry {
public:
   SymTabEntry(TokenKind kind=UNKNOWN, int val=0, operation_t fptr=NULL) ;
   TokenKind m_kind ;
   int m_value ;            // variables' values are stored here
   operation_t m_dothis ;   // pointer to a function that does the work
} ;



// Main Sally Forth class
//
class Sally {

public:

   Sally(istream& input_stream=cin) ;  // make a Sally Forth interpreter

   void mainLoop() ;  // do the main interpreter loop

protected:


   // Sally Forth symbol table
   // keywords and variables are store here
   //
   map<string,SymTabEntry> symtab ;

private:

   // Where to read the input
   //
   istream& istrm ;


   // Sally Forth operations to be interpreted
   //
   list<Token> tkBuffer ;


   // Sally Forth parameter stack
   //
   stack<Token> params;


   // add tokens from input to tkBuffer
   //
   bool fillBuffer() ;


   // give me one more token.
   // calls fillBuffer() for you if needed.
   //
   Token nextToken() ;


   // static member functions that do what has
   // to be done for each Sally Forth operation
   //
   // pointers to these functions are stored
   // in the symbol table
   //
   static void doDUMP(Sally *Sptr) ;    // for debugging

   static void doPlus(Sally *Sptr) ;
   static void doMinus(Sally *Sptr) ;
   static void doTimes(Sally *Sptr) ;
   static void doDivide(Sally *Sptr) ;
   static void doMod(Sally *Sptr) ;
   static void doNEG(Sally *Sptr) ;

   //formating and printing.
   static void doDot(Sally *Sptr) ;
   static void doSP(Sally *Sptr) ;
   static void doCR(Sally *Sptr) ;

   //stack operators:
   static void doDup(Sally *Sptr) ;
   static void doDrop(Sally *Sptr) ;
   static void doSwap(Sally *Sptr) ;
   static void doRot(Sally *Sptr) ;


   //Variable Operators
   static void doSet(Sally *Sptr) ;
   static void doAt(Sally *Sptr) ;
   static void doStore(Sally *Sptr) ;


   // void doSet(Sally *Sptr) ;
   // void doAt(Sally *Sptr) ;
   // void doStore(Sally *Sptr) ;

   //compapison operators:
   static void doLess(Sally *Sptr) ;
   static void doLessEqual(Sally *Sptr) ;
   static void doEqual(Sally *Sptr) ;
   static void doNotEqual(Sally *Sptr) ;
   static void doGreater(Sally *Sptr) ;
   static void doGreaterEqual(Sally *Sptr) ;

   //Boolean Operators:
   static void doOr(Sally *Sptr) ;
   static void doAnd(Sally *Sptr) ;
   static void doNot(Sally *Sptr) ;

   //if/else operators
   static void doIf(Sally *Sptr) ;
   static void doElse(Sally *Sptr) ;
   static void doEndif(Sally *Sptr) ;

   stack<bool> doExecute;
   //this determines whether stuff should execute, based on if/else statemnts.

   //Looping operators
   static void doDo(Sally *Sptr) ;
   static void doUntil(Sally *Sptr) ;

   //this will store values that are repeated when looping.
   queue<Token> loopQueue;
   int inLoop = 0;//tells main loop whether to read from the text input
   // or to read from the loop stack
   // 0 - nothing // 1 - recording data // 2 - reading from queue

} ;

#endif
