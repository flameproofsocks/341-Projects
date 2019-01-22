
// File: Sally.cpp
//
// CMSC 341 Fall 2018 Project 2
//
// Implementation of member functions of Sally Forth interpreter
//
//
#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <stdexcept>
#include <cstdlib>
using namespace std ;

#include "Sally.h"


// Basic Token constructor. Just assigns values.
//
Token::Token(TokenKind kind, int val, string txt) {
   m_kind = kind ;
   m_value = val ;
   m_text = txt ;
}


// Basic SymTabEntry constructor. Just assigns values.
//
SymTabEntry::SymTabEntry(TokenKind kind, int val, operation_t fptr) {
   m_kind = kind ;
   m_value = val ;
   m_dothis = fptr ;
}


// Constructor for Sally Forth interpreter.
// Adds built-in functions to the symbol table.
//
Sally::Sally(istream& input_stream) :
   istrm(input_stream)  // use member initializer to bind reference
{

   symtab["DUMP"]    =  SymTabEntry(KEYWORD,0,&doDUMP) ;

   symtab["+"]    =  SymTabEntry(KEYWORD,0,&doPlus) ;
   symtab["-"]    =  SymTabEntry(KEYWORD,0,&doMinus) ;
   symtab["*"]    =  SymTabEntry(KEYWORD,0,&doTimes) ;
   symtab["/"]    =  SymTabEntry(KEYWORD,0,&doDivide) ;
   symtab["%"]    =  SymTabEntry(KEYWORD,0,&doMod) ;
   symtab["NEG"]  =  SymTabEntry(KEYWORD,0,&doNEG) ;

   symtab["."]    =  SymTabEntry(KEYWORD,0,&doDot) ;
   symtab["SP"]   =  SymTabEntry(KEYWORD,0,&doSP) ;
   symtab["CR"]   =  SymTabEntry(KEYWORD,0,&doCR) ;

   //Stack Operators:
   symtab["DUP"]   =  SymTabEntry(KEYWORD,0,&doDup) ;
   symtab["DROP"]   =  SymTabEntry(KEYWORD,0,&doDrop) ;
   symtab["SWAP"]   =  SymTabEntry(KEYWORD,0,&doSwap) ;
   symtab["ROT"]    =  SymTabEntry(KEYWORD,0,&doRot) ;

   //Variable OPerators:
   //since a number and string will be added to the stack(in order),
   //only the keyword is needed to retrieve from Stack
   symtab["SET"]    =  SymTabEntry(KEYWORD,0,&doSet) ;
   symtab["@"]   =  SymTabEntry(KEYWORD,0,&doAt) ;
   //storing a value to a variable.
   symtab["!"]   =  SymTabEntry(KEYWORD,0,&doStore) ;

   //comparison operators:
   //< <= == != >= >
   symtab["<"]    =  SymTabEntry(KEYWORD,0,&doLess) ;
   symtab["<="]   =  SymTabEntry(KEYWORD,0,&doLessEqual) ;
   symtab["=="]   =  SymTabEntry(KEYWORD,0,&doEqual) ;
   symtab["!="]   =  SymTabEntry(KEYWORD,0,&doNotEqual) ;
   symtab[">="]   =  SymTabEntry(KEYWORD,0,&doGreaterEqual) ;
   symtab[">"]    =  SymTabEntry(KEYWORD,0,&doGreater) ;

   //Boolean Operators, and/or/not
   //work witht the output of comparison operators.
   symtab["OR"]    =  SymTabEntry(KEYWORD,0,&doOr) ;
   symtab["AND"]   =  SymTabEntry(KEYWORD,0,&doAnd) ;
   symtab["NOT"]   =  SymTabEntry(KEYWORD,0,&doNot) ;

   //if/else/endif operators
   symtab["IFTHEN"]    =  SymTabEntry(KEYWORD,0,&doIf) ;
   symtab["ELSE"]   =  SymTabEntry(KEYWORD,0,&doElse) ;
   symtab["ENDIF"]   =  SymTabEntry(KEYWORD,0,&doEndif) ;

   //do/while loops
   symtab["DO"]   =  SymTabEntry(KEYWORD,0,&doDo) ;
   symtab["UNTIL"]   =  SymTabEntry(KEYWORD,0,&doUntil) ;
}


// This function should be called when tkBuffer is empty.
// It adds tokens to tkBuffer.
//
// This function returns when an empty line was entered
// or if the end-of-file has been reached.
//
// This function returns false when the end-of-file was encountered.
//
// Processing done by fillBuffer()
//   - detects and ignores comments.
//   - detects string literals and combines as 1 token
//   - detects base 10 numbers
//
//
bool Sally::fillBuffer() {
   string line ;     // single line of input
   int pos ;         // current position in the line
   int len ;         // # of char in current token
   long int n ;      // int value of token
   char *endPtr ;    // used with strtol()


   while(true) {    // keep reading until empty line read or eof

      // get one line from standard in
      //
      getline(istrm, line) ;

      // if "normal" empty line encountered, return to mainLoop
      //
      if ( line.empty() && !istrm.eof() ) {
         return true ;
      }

      // if eof encountered, return to mainLoop, but say no more
      // input available
      //
      if ( istrm.eof() )  {
         return false ;
      }


      // Process line read

      pos = 0 ;                      // start from the beginning

      // skip over initial spaces & tabs
      //
      while( line[pos] != '\0' && (line[pos] == ' ' || line[pos] == '\t') ) {
         pos++ ;
      }

      // Keep going until end of line
      //
      while (line[pos] != '\0') {

         // is it a comment?? skip rest of line.
         //
         if (line[pos] == '/' && line[pos+1] == '/') break ;

         // is it a string literal?
         //
         if (line[pos] == '.' && line[pos+1] == '"') {

            pos += 2 ;  // skip over the ."
            len = 0 ;   // track length of literal

            // look for matching quote or end of line
            //
            while(line[pos+len] != '\0' && line[pos+len] != '"') {
               len++ ;
            }

            // make new string with characters from
            // line[pos] to line[pos+len-1]
            string literal(line,pos,len) ;  // copy from pos for len chars

            // Add to token list
            //
            tkBuffer.push_back( Token(STRING,0,literal) ) ;

            // Different update if end reached or " found
            //
            if (line[pos+len] == '\0') {
               pos = pos + len ;
            } else {
               pos = pos + len + 1 ;
            }

         } else {  // otherwise "normal" token

            len = 0 ;  // track length of token

            // line[pos] should be an non-white space character
            // look for end of line or space or tab
            //
            while(line[pos+len] != '\0' && line[pos+len] != ' ' && line[pos+len] != '\t') {
               len++ ;
            }

            string literal(line,pos,len) ;   // copy form pos for len chars
            pos = pos + len ;

            // Try to convert to a number
            //
            n = strtol(literal.c_str(), &endPtr, 10) ;

            if (*endPtr == '\0') {
               tkBuffer.push_back( Token(INTEGER,n,literal) ) ;
            } else {
               tkBuffer.push_back( Token(UNKNOWN,0,literal) ) ;
            }
         }

         // skip over trailing spaces & tabs
         //
         while( line[pos] != '\0' && (line[pos] == ' ' || line[pos] == '\t') ) {
            pos++ ;
         }

      }
   }
}



// Return next token from tkBuffer.
// Call fillBuffer() if needed.
// Checks for end-of-file and throws exception
//
Token Sally::nextToken() {
      Token tk ;
      bool more = true ;

      while(more && tkBuffer.empty() ) {
         more = fillBuffer() ;
      }

      if ( !more && tkBuffer.empty() ) {
         throw EOProgram("End of Program") ;
      }

      tk = tkBuffer.front() ;
      tkBuffer.pop_front() ;
      return tk ;
}


// The main interpreter loop of the Sally Forth interpreter.
// It gets a token and either push the token onto the parameter
// stack or looks for it in the symbol table.
//
//
void Sally::mainLoop() {

   Token tk ;
   map<string,SymTabEntry>::iterator it ;

   // a default item isadded so that an empty stack is never attemted at being acessed
   doExecute.push(true);

   try {
      while( 1 ) {

        //determines whether to get from the tokens or the copy stack(loop stack)
       if(inLoop == 2){
         tk = loopQueue.front();
         loopQueue.pop();
         loopQueue.push(tk);
       }else if (inLoop == 1){
         //case for adding to loopQueue
         //"writing" to the queue
        tk = nextToken() ;
        loopQueue.push(tk);
      }else{
        //default
          tk = nextToken() ;
      }

        //everything should be executed the same.

         while(!doExecute.top()){

           if (it->second.m_kind == KEYWORD){
             if(tk.m_text == "ELSE" || tk.m_text == "ENDIF"){
               it = symtab.find(tk.m_text) ;
                it->second.m_dothis(this) ;
              }
           }
           if(inLoop == 2){
             tk = loopQueue.front();
             loopQueue.pop();
             loopQueue.push(tk);
           }else if (inLoop == 1){
             //case for adding to loopQueue
             //"writing" to the queue
            tk = nextToken() ;
            loopQueue.push(tk);
          }else{
            //default
              tk = nextToken() ;
          }

         }


         if ( tk.m_kind == INTEGER || tk.m_kind == STRING ) {

            // if INTEGER or STRING just push onto stack
            params.push(tk) ;

         } else {
              it = symtab.find(tk.m_text) ;

              if ( it == symtab.end() )  {   // not in symtab

                 params.push(tk) ;

            } else if (it->second.m_kind == KEYWORD)  {

               // invoke the function for this operation
               //
               it->second.m_dothis(this) ;

            } else if (it->second.m_kind == VARIABLE) {

               // variables are pushed as tokens
               //
               tk.m_kind = VARIABLE ;
               params.push(tk) ;

            } else {

               // default action
               //
               params.push(tk) ;

            }
         }
      }

   } catch (EOProgram& e) {

      cerr << "End of Program\n" ;
      if ( params.size() == 0 ) {
         cerr << "Parameter stack empty.\n" ;
      } else {
         cerr << "Parameter stack has " << params.size() << " token(s).\n" ;
      }

   } catch (out_of_range& e) {

      cerr << "Parameter stack underflow??\n" ;

   } catch (...) {

      cerr << "Unexpected exception caught\n" ;

   }
}

// -------------------------------------------------------


void Sally::doPlus(Sally *Sptr) {
   Token p1, p2 ;

   if ( Sptr->params.size() < 2 ) {
      throw out_of_range("Need two parameters for +.") ;
   }
   p1 = Sptr->params.top() ;
   Sptr->params.pop() ;
   p2 = Sptr->params.top() ;
   Sptr->params.pop() ;
   int answer = p2.m_value + p1.m_value ;
   Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doMinus(Sally *Sptr) {
   Token p1, p2 ;

   if ( Sptr->params.size() < 2 ) {
      throw out_of_range("Need two parameters for -.") ;
   }
   p1 = Sptr->params.top() ;
   Sptr->params.pop() ;
   p2 = Sptr->params.top() ;
   Sptr->params.pop() ;
   int answer = p2.m_value - p1.m_value ;
   Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doTimes(Sally *Sptr) {
   Token p1, p2 ;

   if ( Sptr->params.size() < 2 ) {
      throw out_of_range("Need two parameters for *.") ;
   }
   p1 = Sptr->params.top() ;
   Sptr->params.pop() ;
   p2 = Sptr->params.top() ;
   Sptr->params.pop() ;
   int answer = p2.m_value * p1.m_value ;
   Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doDivide(Sally *Sptr) {
   Token p1, p2 ;

   if ( Sptr->params.size() < 2 ) {
      throw out_of_range("Need two parameters for /.") ;
   }
   p1 = Sptr->params.top() ;
   Sptr->params.pop() ;
   p2 = Sptr->params.top() ;
   Sptr->params.pop() ;
   int answer = p2.m_value / p1.m_value ;
   Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doMod(Sally *Sptr) {
   Token p1, p2 ;

   if ( Sptr->params.size() < 2 ) {
      throw out_of_range("Need two parameters for %.") ;
   }
   p1 = Sptr->params.top() ;
   Sptr->params.pop() ;
   p2 = Sptr->params.top() ;
   Sptr->params.pop() ;
   int answer = p2.m_value % p1.m_value ;
   Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doNEG(Sally *Sptr) {
   Token p ;

   if ( Sptr->params.size() < 1 ) {
      throw out_of_range("Need one parameter for NEG.") ;
   }
   p = Sptr->params.top() ;
   Sptr->params.pop() ;
   Sptr->params.push( Token(INTEGER, -p.m_value, "") ) ;
}


void Sally::doDot(Sally *Sptr) {
   Token p ;
   if ( Sptr->params.size() < 1 ) {
      throw out_of_range("Need one parameter for .") ;
   }

   p = Sptr->params.top() ;
   Sptr->params.pop() ;

   if (p.m_kind == INTEGER) {
      cout << p.m_value ;
   } else {
      cout << p.m_text ;
   }
}


void Sally::doSP(Sally *Sptr) {
   cout << " " ;
}


void Sally::doCR(Sally *Sptr) {
   cout << endl ;
}

void Sally::doDUMP(Sally *Sptr) {
   // do whatever for debugging
}

//comparison operators:
void Sally::doLess(Sally *Sptr) {
   Token p1, p2 ;
   int answer = 0;

   if ( Sptr->params.size() < 2 ) {
      throw out_of_range("Need two parameters for <.") ;
   }
   p1 = Sptr->params.top() ;
   Sptr->params.pop() ;
   p2 = Sptr->params.top() ;
   Sptr->params.pop() ;
   if(p2.m_value < p1.m_value){
     answer = 1;
     Sptr->params.push( Token(INTEGER, answer, "true") ) ;
   }else{
     answer = 0;
    Sptr->params.push( Token(INTEGER, answer, "false") ) ;
   }
}

void Sally::doLessEqual(Sally *Sptr) {
  Token p1, p2 ;
  int answer = 0;

  if ( Sptr->params.size() < 2 ) {
     throw out_of_range("Need two parameters for <=.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if(p2.m_value <= p1.m_value){
    answer = 1;
    Sptr->params.push( Token(INTEGER, answer, "true") ) ;
  }else{
    answer = 0;
   Sptr->params.push( Token(INTEGER, answer, "false") ) ;
  }
}

void Sally::doEqual(Sally *Sptr) {
  Token p1, p2 ;
  int answer = 0;

  if ( Sptr->params.size() < 2 ) {
     throw out_of_range("Need two parameters for ==.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if(p2.m_value == p1.m_value){
    answer = 1;
    Sptr->params.push( Token(INTEGER, answer, "true") ) ;
  }else{
    answer = 0;
   Sptr->params.push( Token(INTEGER, answer, "false") ) ;
  }
}

void Sally::doNotEqual(Sally *Sptr) {
  Token p1, p2 ;
  int answer = 0;

  if ( Sptr->params.size() < 2 ) {
     throw out_of_range("Need two parameters for !=.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if(p2.m_value != p1.m_value){
    answer = 1;
    Sptr->params.push( Token(INTEGER, answer, "true") ) ;
  }else{
    answer = 0;
   Sptr->params.push( Token(INTEGER, answer, "false") ) ;
  }
}

void Sally::doGreaterEqual(Sally *Sptr) {
  Token p1, p2 ;
  int answer = 0;

  if ( Sptr->params.size() < 2 ) {
     throw out_of_range("Need two parameters for >=.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if(p2.m_value >= p1.m_value){
    answer = 1;
    Sptr->params.push( Token(INTEGER, answer, "true") ) ;
  }else{
    answer = 0;
   Sptr->params.push( Token(INTEGER, answer, "false") ) ;
  }
}

void Sally::doGreater(Sally *Sptr) {
  Token p1, p2 ;
  int answer = 0;

  if ( Sptr->params.size() < 2 ) {
     throw out_of_range("Need two parameters for >.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if(p2.m_value > p1.m_value){
    answer = 1;
    Sptr->params.push( Token(INTEGER, answer, "true") ) ;
  }else{
    answer = 0;
   Sptr->params.push( Token(INTEGER, answer, "false") ) ;
  }
}

void Sally::doDup(Sally *Sptr) {
  Token p1;
  TokenKind t;
  string message = "";
  int value = 0;

  if ( Sptr->params.size() < 1 ) {
     throw out_of_range("Need one parameter for DUP.") ;
  }
  p1 = Sptr->params.top() ;
  t = p1.m_kind;
  message = p1.m_text;
  value = p1.m_value;

  Sptr->params.push( Token(t, value, message) ) ;

}

void Sally::doDrop(Sally *Sptr) {
  Token p1;
  if ( Sptr->params.size() < 1 ) {
     throw out_of_range("Need one parameter for DROP.") ;
  }else{
    Sptr->params.pop() ;
  }
}


void Sally::doSwap(Sally *Sptr) {
  //the p1 is at the top of the stack, and p2 is one lower
  Token p1, p2 ;

  if ( Sptr->params.size() < 2 ) {
     throw out_of_range("Need two parameters for SWAP.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;

  //taking the information to copy to the refreshed variables.
  TokenKind t1 = p1.m_kind;
  string message1 = p1.m_text;
  int value1 = p1.m_value;

  TokenKind t2 = p2.m_kind;
  string message2 = p2.m_text;
  int value2 = p2.m_value;

  //p1 shoudl be pushed first, putting it below p2.
  Sptr->params.push( Token(t1, value1, message1) ) ;
  Sptr->params.push( Token(t2, value2, message2) ) ;


}

void Sally::doRot(Sally *Sptr) {
  //ideally this may be done with loops and an array
  //but for just 3 items, that may be excessive.
  Token p1, p2, p3;

  if ( Sptr->params.size() < 3 ) {
     throw out_of_range("Need two parameters for ROTATION.") ;
  }
  //get the infromation for the top 3.
  //1 will be top of stack, and should go to bottom.
  //items will be replaced 1, 3, 2
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p3 = Sptr->params.top() ;
  Sptr->params.pop() ;

  //done very siimilar to swap.
  //taking the information to copy to the refreshed variables.
  TokenKind t1 = p1.m_kind;
  string message1 = p1.m_text;
  int value1 = p1.m_value;

  TokenKind t2 = p2.m_kind;
  string message2 = p2.m_text;
  int value2 = p2.m_value;

  TokenKind t3 = p3.m_kind;
  string message3 = p3.m_text;
  int value3 = p3.m_value;
  //this can be done in a for loop for an expandable, >3 rotation.

  //if you wanted to replicate the stack without rotating,
  //the order should be 3,2,1.
  //however the rotation causes this to be 2 1 3
  Sptr->params.push( Token(t2, value2, message2) ) ;
  Sptr->params.push( Token(t1, value1, message1) ) ;
  Sptr->params.push( Token(t3, value3, message3) ) ;

}

//and, or , not,   values for
void Sally::doOr(Sally *Sptr) {
  Token p1, p2 ;

  if ( Sptr->params.size() < 2 ) {
     throw out_of_range("Need two parameters for OR.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if(p1.m_text == "true" || p2.m_text == "true"){
    Sptr->params.push( Token(INTEGER, 1, "true") ) ;
  }else{
    Sptr->params.push( Token(INTEGER, 0, "false") ) ;
  }
}


void Sally::doAnd(Sally *Sptr) {
  Token p1, p2 ;

  if ( Sptr->params.size() < 2 ) {
     throw out_of_range("Need two parameters for AND.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if(p1.m_text == "true" && p2.m_text == "true"){
    Sptr->params.push( Token(INTEGER, 1, "true") ) ;
  }else{
   Sptr->params.push( Token(INTEGER, 0, "false") ) ;
  }
}

void Sally::doNot(Sally *Sptr) {
  Token p1, p2 ;

  if ( Sptr->params.size() < 1 ) {
     throw out_of_range("Need one parameter for NOT.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  if(p1.m_text == "true" ){
   Sptr->params.push( Token(INTEGER, 0, "false") ) ;
 }else{
  Sptr->params.push( Token(INTEGER, 1, "true") ) ;
 }
}

void Sally::doSet(Sally *Sptr) {
  Token p1, p2 ;
  //p1 is the name, and p2 is the value

  if ( Sptr->params.size() < 2 ) {
     throw out_of_range("Need two parameters for SET.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;

  //since find function only returns the end if the value is not found,
  //this will only add to the table when the value is not found.
  if(Sptr->symtab.find(p1.m_text) == Sptr->symtab.end() ){
    //this is added to symbol table not stack
    //create the table enttry, adds to std::map<key, value> map;
    Sptr->symtab[p1.m_text] = SymTabEntry(VARIABLE, p2.m_value) ;
  }else{
    cout << "Error, variable already defined";
  }

}


void Sally::doAt(Sally *Sptr) {
  Token p1;
  //p1 is the name, and p2 is the value


  if ( Sptr->params.size() < 1 ) {
     throw out_of_range("Need one parameter for @.") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;

  int val;
  map<string, SymTabEntry>::iterator it; //declare the iterator
  it = Sptr->symtab.find(p1.m_text);
  if(it != Sptr->symtab.end()){
    //using seconf gets the value in the map
    Sptr->params.push( Token(INTEGER, it->second.m_value)  ) ;
    //Token(VARIABLE, it->second.m_value)
  }else{
    cout << "Variable not found";
  }

}

void Sally::doStore(Sally *Sptr){
  //this is very similar to the SET.
  //basically set after it has been set.
  Token p1, p2 ;
  //p1 is the name, and p2 is the value

  if ( Sptr->params.size() < 2 ) {
     throw out_of_range("Need two parameters for !(Store).") ;
  }
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;
  p2 = Sptr->params.top() ;
  Sptr->params.pop() ;

  //done in opposite way as set. if the variable doesnt exist then you have Error
  if(Sptr->symtab.find(p1.m_text) != Sptr->symtab.end() ){
    //this is added to symbol table not stack
    //create the table enttry, adds to std::map<key, value> map;
    Sptr->symtab[p1.m_text] = SymTabEntry(VARIABLE,p2.m_value) ;
  }else{
    cout << "Error, variable not yet defined";
  }

}


void Sally::doIf(Sally *Sptr){
  //this should add to the stack.

  //code for if statement.
  Token p1;
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;

  if(p1.m_text == "true"){
    Sptr->doExecute.push(true);
  }else{
    Sptr->doExecute.push(false);
  }

}

void Sally::doElse(Sally *Sptr){

  //code for else statement.
  if(Sptr->doExecute.top() == true){
    Sptr->doExecute.pop();
    Sptr->doExecute.push(false);
  }else{
    Sptr->doExecute.pop();
    Sptr->doExecute.push(true);
  }

}

void Sally::doEndif(Sally *Sptr){
  //this should take from the stack.
  //because previous statements can still be underneath,
  // thus funtion shouldn't change anything.
  Sptr->doExecute.pop();

}

void Sally::doDo(Sally *Sptr){
  //for the case of loops in series this will also clear the loop
  for(int i = 0; i < Sptr->loopQueue.size(); i++){
    Sptr->loopQueue.pop();
  }

  //the do starts the loop, this will tell when things should be added to
  // the list of repeating items.
  Sptr->inLoop = 1; //starts recording data
  //recording data may not mean the conditions for the loop are even met
  //but it will still be done

}

void Sally::doUntil(Sally *Sptr){
  //this function should evaluate whether or not it is time to stop
  // and either loop again or signal out of the loop
  Token p1;
  p1 = Sptr->params.top() ;
  Sptr->params.pop() ;

  if(p1.m_text != "true"){
    //coindition for cointinuing looping
    Sptr->inLoop = 2; //set to read from the stored data

  }else{
    //exit the loop
    Sptr->inLoop = 0;
  }

}
