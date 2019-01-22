// File: Driver.cpp
//
// BAsed off test3, test 2, test 5
// tests inserts removes, resizing, giving up on rehashing.

#include <iostream>
#include <cstdlib>
using namespace std ;

#include "HashTable.h"


int main() {
   HashTable T(107) ;

   T.insert("undulation") ;     //  9
   T.insert("impertinent") ;    // 10
   T.insert("maladies") ;       // 10 -> 11
   T.insert("dominions") ;	// 12

   T.insert("waspish") ;        // 52
   T.insert("wildebeest") ;     // 52 -> 53
   T.insert("reaction") ;       // 52 -> 54

   T.insert("pawns") ;          // 43
   T.insert("vacuously") ;      // 43 -> 44
   T.insert("firth") ;          // 43 -> 45
   T.insert("manipulator") ;    // 43 -> 46
   T.insert("dreariness") ;     // 43 -> 47

   T.insert("insanity") ;       // 105
   T.insert("enthronement") ;   // 105 -> 106
   T.insert("falsifiers") ;     // 105 -> 0
   T.insert("ignominious") ;    // 105 -> 1
   T.insert("mummified") ;      // 105 -> 2

   T.insert("tributes") ;	// 21
   T.insert("skulduggery") ;	// 22
   T.insert("convulse") ;	// 23
   T.insert("frothed") ;	// 24
   T.insert("horrify") ;	// 25
   T.insert("blackmailers") ;	// 26
   T.insert("defenestrated") ;	// 27
   T.insert("garrison") ;	// 23 -> 28
   T.insert("lidless") ;	// 22 -> 29
   T.insert("eye") ;            // 21 -> 30, should trigger inc. rehash


   cout << "----------------------------------------------------\n" ;
   cout << "Hash table after rehash triggered\n" ;
   cout << "----------------------------------------------------\n" ;
   T.dump() ;
   cout << "----------------------------------------------------\n" ;

   cout << "\n\nDo some insertions to make long linear probe sequence in the second table.\n\n" ;

   T.insert("wildcat") ;        // 18 (new table)
   T.insert("weightlessness") ; // 69 (new table)
   T.insert("sorceress") ;      // 33 (new table)
   T.insert("enchantress") ;    // 33 (new table) really.
   T.insert("witchery") ;       // 67 -> 68 (new table)
   T.insert("heliosphere") ;    // 67 -> 72 (new table)
   T.insert("obstruct") ;       // 67 -> 73 (new table)

   cout << "----------------------------------------------------\n" ;
   cout << "Hash table insertions.\n" ;
   cout << "----------------------------------------------------\n" ;
   T.dump() ;
   cout << "----------------------------------------------------\n" ;

   cout << "\n\nOne more insertion in slot 67 should make us give up on rehashing.\n\n" ;
   T.insert("peripatetic") ;    // 67 -> 77 (new table)

   cout << "----------------------------------------------------\n" ;
   cout << "Hash table giving up on rehashing.\n" ;
   cout << "----------------------------------------------------\n" ;
   T.dump() ;
   cout << "----------------------------------------------------\n" ;


   //after giving up on rehashing, we establish a second table and add a few things.
   HashTable N(107) ;

   N.insert("undulation") ;     //  9
   N.insert("impertinent") ;    // 10
   N.insert("maladies") ;       // 10 -> 11
   N.insert("dominions") ;	// 12

   N.insert("waspish") ;        // 52
   N.insert("wildebeest") ;     // 52 -> 53
   N.insert("reaction") ;       // 52 -> 54

   N.insert("pawns") ;          // 43
   N.insert("vacuously") ;      // 43 -> 44
   N.insert("firth") ;          // 43 -> 45
   N.insert("manipulator") ;    // 43 -> 46
   N.insert("dreariness") ;     // 43 -> 47

   N.insert("insanity") ;       // 105
   N.insert("enthronement") ;   // 105 -> 106
   N.insert("falsifiers") ;     // 105 -> 0
   N.insert("ignominious") ;    // 105 -> 1
   N.insert("mummified") ;      // 105 -> 2

   N.insert("tributes") ;	// 21
   N.insert("skulduggery") ;	// 22
   N.insert("convulse") ;	// 23
   N.insert("frothed") ;	// 24
   N.insert("horrify") ;	// 25
   N.insert("blackmailers") ;	// 26
   N.insert("defenestrated") ;	// 27
   N.insert("garrison") ;	// 23 -> 28
   N.insert("lidless") ;	// 22 -> 29

   cout << "----------------------------------------------------\n" ;
   cout << "Original hash table\n" ;
   cout << "----------------------------------------------------\n" ;
   N.dump() ;
   cout << "----------------------------------------------------\n" ;

   cout << "\n\nInserting \"eye\" should trigger rehash\n\n" ;
   N.insert("eye") ;            // 21 -> 30, should trigger inc. rehash


   cout << "----------------------------------------------------\n" ;
   cout << "Hash table after rehash triggered\n" ;
   cout << "----------------------------------------------------\n" ;
   N.dump() ;
   cout << "----------------------------------------------------\n" ;

   cout << "\n\nSearch for \"manipulator\" should move cluster in slots 43-47.\n\n" ;
   N.find("manipulator") ;            // 21 -> 30, should trigger inc. rehash

   cout << "----------------------------------------------------\n" ;
   cout << "Hash table after cluster 43-47 moved.\n" ;
   cout << "----------------------------------------------------\n" ;
   N.dump() ;
   cout << "----------------------------------------------------\n" ;

   cout << "\n\nDo some finds, inserts and removes\n\n" ;

   const char *str ;
   char *ptr ;

   if( N.find(str="zip") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }

   if( N.find(str="spaceflight") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }

   if( N.find(str="frothed") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }

   N.insert("wildcat") ;        // 18 (new table)
   N.insert("weightlessness") ; // 69 (new table)
   N.insert("sorceress") ;      // 33 (new table)
   N.insert("enchantress") ;    // 33 (new table) really.


   cout << "----------------------------------------------------\n" ;
   cout << "Hash table after more insertions.\n" ;
   cout << "----------------------------------------------------\n" ;
   N.dump() ;
   cout << "----------------------------------------------------\n" ;

   cout << "\n\nA find on \"ignominious\" + 1 more operation should cause the tables to consolidate down to one table. \n\n" ;

   if( N.find(str="ignominious") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }

   if( N.find(str="reaction") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }


   cout << "----------------------------------------------------\n" ;
   cout << "Hash table after wrap up.\n" ;
   cout << "----------------------------------------------------\n" ;
   N.dump() ;
   cout << "----------------------------------------------------\n" ;


   cout << "----------------------------------------------------\n" ;
   cout << "Hash tables have been set to each other.\n" ;
   cout << "----------------------------------------------------\n" ;
   
   //finally set the two tables equal to each other, (in this case assignment op)
   //and test for equivilance
   N = T;
   if(N.tableSize() == T.tableSize()){
     cout << "Table Sizes are correct \n";
   }else{
     cout << "Table Sizes are off.\n";
   }

   if(N.size() == T.size() ){
     cout << "Sizes are correct\n";
   }else{
     cout << "Sizes are Incorrect\n";
   }


}
