#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <iostream>
#include <cstring>

#define MAX_CLUSTER 10;

using namespace std;


class HashTable{
public:


  //default size is 101 (prime)
  HashTable(int n=101) ;

  //deletes array
  ~HashTable();

  //copy Constructor
  HashTable (HashTable& other);

  //overloaded assignment OPERATOR
  const HashTable& operator= (HashTable& rhs) ;

  //code to has strings
  unsigned int hashCode(const char *str);

  //finds the next open index
  //this function returns -1 when it needs to use the next array
  int probe(unsigned int hash, const char *str, bool isInsert);

  void clean(int arrayTo);

  void transfer(int index);

  void checkRehash(); //checks load factor

  void insert(const char *str) ;

  bool find(const char *str) ;

  char * remove(const char *str) ;

  //GRADING FUNCTIONS
  bool isRehashing(){
    if(use2){
      return true;
    }else{
      return false;
    }
  }
  int tableSize(int table=0) {
    if(table == 0){
      return m_arrayCapacity;
    }else{
      return m_array2Capacity;
    }
  }
  int size(int table=0) {
    if(table == 0){
      return m_arraySize;
    }else{
      return m_array2Size;
    }
  }

  const char * at(int index, int table=0) {
    if(table == 0){
      if(array[index] != NULL){
        return array[index];
      }else{
        return NULL;
      }
    }else{
      if(array2[index] != NULL){
        return array2[index];
      }else{
        return NULL;
      }
    }

  }

  void dump() ;

private:

  int m_arraySize = 0; //size of array start at 0
  int m_array2Size = 0;
  int m_arrayCapacity = 0; //capacity will be initialize
  int m_array2Capacity = 0;
  char ** array = NULL; //dynamic allocated array
  char ** array2 = NULL;  //second array for when more space is needed
  //once all of array1 is copied, array 1 will be deleted
  //then the pointer for array 1 will be set to array 2.
  //then array 2 will be set to null
  //NULL is used to see whether the second array is being used.
  bool use2 = false; //tells whether to insert into the second array

  float load = 0.0; //the current load factor, changes with insert and delete
  float load2 = 0.0;

  char ** backupArray = NULL; //the fubar third array top copy everything at once
  bool useBackup = false;

  //sentinel for removed item
  static char * const DELETED ;

  //to avoid extra messes with insert while cleaning
  bool isMoving = false;

};


#endif
