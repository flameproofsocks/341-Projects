#include "HashTable.h"
#include "primes.cpp"
#include <iostream>
#include <cstring>
#include <string>
#include <cmath> //for abs


using namespace std;
char * const HashTable::DELETED  = (char *) &DELETED; //define keyword

HashTable::HashTable(int n){
  array = new char*[n]; //only declare the first array
  m_arrayCapacity = roundUpPrime(n);

}

//deletes array
HashTable::~HashTable(){

  //free, then delete
  for(int i = 0; i < m_arrayCapacity; i++){
    if(array[i] == DELETED){
      delete array[i];
    }else if(array[i] != NULL){
      free(array[i]);
      array[i] = NULL;
    }
  }
  delete [] array;
  array = NULL;

  if(use2){
    for(int i = 0; i < m_array2Capacity; i++){
      if(array2[i] == DELETED){
      }else if(array2[i] != NULL){
        free(array2[i]);
        array2[i] = NULL;
      }
    }
    delete [] array2;
    array2 = NULL;
  }
}

//copy Constructor
HashTable::HashTable (HashTable& other){
  //manually copy the arrays and copy varibles.
  //doesn't matter about re-hashing as the arrays are same.

  //initialize array 1:
  array = new char*[other.m_arrayCapacity];
  //copy array 1:
  for(int i = 0; i < other.m_arrayCapacity; i++){
    if(other.array[i] == other.DELETED){ //copy deleted aswell
      array[i] = DELETED;
    }else if(other.array[i] != NULL){ //no need to copy null.
      array[i] = strdup(other.array[i]);
    }
  }
  //copy data values
  m_arraySize = other.m_arraySize;
  m_arrayCapacity = other.m_arrayCapacity;
  load = other.load;
  load2 = other.load2;

  if(other.use2){
    use2 = true;
    array2 = new char*[other.m_array2Capacity];
    for(int i = 0; i < other.m_array2Capacity; i++){
      if(other.array2[i] == other.DELETED){ //copy deleted
        array2[i] = DELETED;
      }else if(other.array2[i] != NULL){
        array2[i] = strdup(other.array2[i])
        ;
      }
    }
    m_array2Size = other.m_array2Size;
    m_array2Capacity = other.m_array2Capacity;

  }
  dump();

}

//overloaded assignment OPERATOR
const HashTable& HashTable::operator=(HashTable& rhs){
  if(this == &rhs){
    return *this; //gaurd against self assignment
  }

  //*It also works to just call the destructor (But that is wrong)
  //delete current array
  //(To prevent memory leaks delete the array before re-assigning it)
  //free, then delete
  for(int i = 0; i < m_arrayCapacity; i++){
    if(array[i] == DELETED){
      delete array[i];
    }else if(array[i] != NULL){
      free(array[i]);
      array[i] = NULL;
    }
  }
  delete [] array;
  array = NULL;

  if(use2){
    for(int i = 0; i < m_array2Capacity; i++){
      if(array2[i] == DELETED){
      }else if(array2[i] != NULL){
        free(array2[i]);
        array2[i] = NULL;
      }
    }
    delete [] array2;
    array2 = NULL;
  }


  //pretty much same as copy constructor
  //initialize array 1:
  array = new char* [rhs.m_arrayCapacity];
  //copy array 1:
  for(int i = 0; i < rhs.m_arrayCapacity; i++){
    if(rhs.array[i] == rhs.DELETED){ //copy deleteees
      array[i] = DELETED;
    }else if(rhs.array[i] != NULL){ //no need to copy null.
      array[i] = strdup(rhs.array[i]);
    }
  }
  //copy data values
  m_arraySize = rhs.m_arraySize;
  m_arrayCapacity = rhs.m_arrayCapacity;
  load = rhs.load;
  load2 = rhs.load2;

  if(rhs.use2){
    use2 = true;
    array2 = new char*[rhs.m_array2Capacity];
    for(int i = 0; i < rhs.m_array2Capacity; i++){
      if(rhs.array2[i] == rhs.DELETED){
        array2[i] = DELETED;
      }else if(rhs.array2[i] != NULL){
        array2[i] = strdup(rhs.array2[i]);
      }
    }
    m_array2Size = rhs.m_array2Size;
    m_array2Capacity = rhs.m_array2Capacity;

  }
  return *this;

}


unsigned int HashTable::hashCode(const char *str) {

   unsigned int val = 0 ;
   const unsigned int thirtyThree = 33 ;  // magic number from textbook

   int i = 0 ;
   while (str[i] != '\0') {
      val = val * thirtyThree + str[i] ;
      i++ ;
   }
   return val ;
}


//str is NULL when finding, and a c_str when finding/deletin
//used to find where to insert
//uses hash index and finds open index in array
int HashTable::probe(unsigned int hash, const char *str, bool isInsert){
  //TO DO make probe move ENTIRE cluster (move beginning and end)
  // transfer(hash%m_arrayCapacity); //transfer beginning
  // transfer(index+1); //transfer end

  int index = hash%m_arrayCapacity;
  //the first array is always searched, as something for find or delete could be here,
  //and insert must also check the first array to avoid dupliactes.
  //perhaps duplicates can be allowed across arrays, but not in the same array
  if(!isMoving){ // you always search the first array.

    for(int i = 0; i < 9; i++){ //lass than the maximum size (10) (MAX_CLUSTER)

      if(array[index] != NULL && array[index] != DELETED && 0 == strcmp( array[index] ,str)  ){
        if(isInsert && !use2){//this means its a DUPLICATE!!!!
          return -3; //special number!!!! for a duplicate!!!
        }else{
          return index; //find and remove should also call trasnfer
        }
      }
      if(array[index] == NULL){ //return -1 (find) or index(insert)

        if(use2 && i > 0){
          transfer(index); //transfer
        }

        if(isInsert){ //return index
          if(!use2){
            return index;
          }else{
            i = 21; //this is to exit out of the loop.
          }
        }else{ //(find), return -1
          return -1;
        } //still attempts to copy when the second array is in use.

      }
      if(array[index] == DELETED && isInsert){ //insert into DELETED
        if(!use2){
          return index;
        }else{ //this is when insert is called when you use the second array
          i = 21; //this is to exit out of the loop.
          transfer(index); //transfer end
        }
      }

      index = index+1;
      if(index >= m_arrayCapacity){
        index = 0;
      }
    }
  }
  if (use2){ //using second array
    index = hash%m_array2Capacity;
    for(int i = 0; i < 9; i++){ //less than the maximum size (10) (MAX_CLUSTER)

      //either a duplicate or the item was found
      if(array2[index] != NULL && array2[index] != DELETED && 0 == strcmp( array2[index] ,str)  ){
        if(isInsert){//this means its a DUPLICATE!!!!
          return -3; //special number!!!! for a duplicate!!!
        }else{
          return index;
        }
      }
      if(array2[index] == NULL){ //return -1 (find) or index(insert)
        if(isInsert){ //return index
          return index;
        }else { //(find), return -1
          return -1;
        } //still attempts to copy when the second array is in use.
      }

      if(array2[index] == DELETED && isInsert){ //insert into DELETED
        return index;
      }

      index = index+1; //update index
      if(index >= m_arrayCapacity){
        index = 0;
      }

    }
  }

  //this only gets to this point after probing for a long time.
  //this means resizing will have to occur
  if(!use2){
    //initialize the second array
    use2 = true;
    int newCap = roundUpPrime(m_arrayCapacity+1); //round up to enarest prime
    array2 = new char*[newCap]; //only declare the first array
    m_array2Capacity = newCap;

    //this is the result of a large cluster, then that cluster must be copied.
    transfer(index);

    return -1;

  }else if(!useBackup){ //MAke a third array

    clean(3); //clean array 1 and 2 to array 3
    return -2;


  }else{//i assume this means even the third array is horribly made
    //I'm just throwing an exception here.
    throw out_of_range("ERROR IN THIRD ARRAY");
  }

}

//"cleans up" and moves everything from first array to second, or
// everything to third array.
void HashTable::clean(int arrayTo){

  isMoving = true;
  //if array 2 is 2, this transfers everything to the second array.
  //if array is 3, this transfers everything (first and second array) to the third array.
  if(arrayTo == 2){

    use2 = true;
    //copy the items over
    for(int i = 0; i < m_arrayCapacity; i++){
      if(array[i] != NULL && array[i] != DELETED){
        char *hold = strdup(array[i]);
        free(array[i]); //free memory
        array[i] = NULL;
        insert(hold);
        free(hold);
      }
    }

    delete [] array;

    //basically set the pointer to the backup array
    array = array2;
    m_arraySize = m_array2Size;
    m_arrayCapacity = m_array2Capacity;

    m_array2Capacity = 0; //delete second array
    m_array2Size = 0;
    array2 = NULL;
    use2 = false;

  }else{ //transfer to third array (backup)

    //this adds (+1) to account for the insert not being performed yet.
    int newSize = (m_arraySize + m_array2Size + 1);
    int newCap = roundUpPrime( ( 4 * newSize ) ) ;
    backupArray = new char*[newCap]; //initialize array

    //copy the items over
    for(int i = 0; i < m_arrayCapacity; i++){
      if(array[i] != NULL){
        unsigned int index  = hashCode(array[i])%newCap;
        for(int j = 0; j < 9; j++){ //lass than the maximum size (10) (MAX_CLUSTER)
          if(backupArray[i] == NULL){
            j = 77; //exit from the loops
          }else{
            //find the next available spot
            index++;
            if(index == newCap){
              index = 0;
            }
          }
        }
        backupArray[index] = strdup(array[i]);
        free(array[i]);
        array[i] = NULL;
      }
    }
    //copy array 2
    for(int i = 0; i < m_array2Capacity; i++){
      if(array2[i] != NULL){
        unsigned int index  = hashCode(array2[i])%newCap;
        for(int j = 0; j < 9; j++){ //lass than the maximum size (10) (MAX_CLUSTER)
          if(backupArray[index] == NULL){
            j = 77; //exit from the loops
          }else{
            index++;
            if(index == newCap){
              index = 0;
            }
          }

        }
        backupArray[index] = strdup(array2[i]);
        free(array2[i]);
        array2[i] = NULL;
      }
    }

    delete [] array;
    delete [] array2;
    m_array2Capacity = 0;
    m_array2Size = 0;

    //basically set the pointer to the backup array
    array = backupArray;
    m_arraySize = newSize;
    m_arrayCapacity = newCap;
    backupArray = NULL;
    use2 = false;

  }

  isMoving = false;
}

//transfers everything around the cluster from array 1 to array 2.
void HashTable::transfer(int index){

  isMoving = true;
  //trasnfer the item
  if(array[index] != NULL ){     //copy the item over
    char *hold = strdup(array[index]);
    free(array[index]); //array[left] = DELETED; //
    array[index] = NULL;
    insert( hold); //insert( strdup( array[left] ) );
    free(hold);
    m_arraySize-- ;  //update size
  }

  int left = index - 1;
  int right = index + 1; //left and right should always be different
  if(left < 0){
    left = m_arrayCapacity - 1; }
  if(right > m_arrayCapacity){
    right = 0; }
  long int stop = 11;
  //when both left and right (2 and 3) have
  while(stop % 6 != 0){ //exits when cluster is copied end to end
    //dealing with indexies.
    //stop when there is empty place
    if(array[left] == NULL ){
      stop = stop * 2;
    }else if( strcmp(array[left], DELETED) != 0 ){     //copy the item over
      char *hold = strdup(array[left]);
      free(array[left]); //array[left] = DELETED; //
      array[left] = NULL;
      m_arraySize-- ;  //update size
      insert( hold); //insert( strdup( array[left] ) );
      free(hold);
    }
    if(!use2){ //return if it cleans up
      return;
    }

    if(array[right] == NULL ){
      stop = stop * 3;
    }else if( strcmp(array[right], DELETED) != 0 ){     //copy the item over
      char *hold = strdup(array[right]);
      free(array[right]); //
      array[right] = NULL;
      m_arraySize-- ;  //update size
      insert( hold); //insert( strdup( array[left] ) );
      free(hold);

    }
    if(!use2){ //return if it cleans up
      return;
    }
    load = (float)m_arraySize/(float)m_arrayCapacity; //update load factor


    //increment / decrement indexies
    //doesnt increment once a blank has been found
    if(stop % 2 != 0){
      if(left == 0){
        left = m_array2Capacity-1;
      }else{
        left = left - 1;
      }
    }
    if(stop % 3 != 0){
      right = m_array2Capacity%(left+1);
    }

  }
  isMoving = false;
}

//checks load factors and triggers re-hashing when needed.
void HashTable::checkRehash(){
  load = (float)m_arraySize/(float)m_arrayCapacity; //update load factor
  //create second array if array1 has >0.5 loadFactor
  if(load > 0.5 ){
    //initialize the second array
    use2 = true;
    int newCap = roundUpPrime(m_arrayCapacity*4); //round up to nearest prime
    array2 = new char*[newCap]; //only declare the first array
    m_array2Capacity = newCap;

    //this is not from large cluster, thus must transfer is not called.
  }
  if(load2 > 0.5){
    clean(3); //this is when the second array overflows while first is still being used
  }

  //cleanup array when loadFactor < 0.03
  if(load < 0.03 && use2){
    load = 0.1; //this is to avoid an infinite loop
    m_arraySize = (m_arrayCapacity - 1 )/ 2;
    clean(2); //clean up and transfer to array2
  }

}

void HashTable::insert(const char *str) {

  checkRehash(); //checks if rehashing needs to ocurr, or if cleanup needs to ocurr
  //hash the string to find the index
  if(str == NULL){
    return; //can't insert nothing.
  }
  unsigned int hash = hashCode(str);
  //find the next available spot
  int index = 0;
  index = probe(hash, str, true);

  if(index == -3){ //this means it's a dupliacte, and you exit.
    return;
  }

  if(index == -2){ //special case for third array
    //insert into third array (this just means re-hash)
    index = probe(hash, str, true);
    //Assume this doesn't fail, and just insert into that index
    array[index] = strdup(str);

    m_arraySize++;  //update size
    load = (float)m_arraySize/(float)m_arrayCapacity; //update load factor

  }else if(index == -1){ //insert into second array for first time.
    index = probe(hash, str, true); //hash for second array

    //Assume this doesn't fail, and just insert into that index
    array2[index] = strdup(str);

    m_array2Size++;  //update size
    load2 = (float)m_array2Size/(float)m_array2Capacity; //update load factor

  }else{ //insert normally
    if(!use2){ //insert into first array
      array[index] = strdup(str);

      m_arraySize++;  //update size
      load = (float)m_arraySize/(float)m_arrayCapacity; //update load factor
    }else{
      array2[index] = strdup(str);
      m_array2Size++;  //update size
      load2 = (float)m_array2Size/(float)m_array2Capacity; //update load factor
    }
  }
}

//checks whether something exists. (In both arrays)
bool HashTable::find(const char *str) {

  checkRehash();
  //first find the index.
  unsigned int hash = hashCode(str);

  //find the next available spot
  int index = probe(hash, str , false);

  if(index >= 0){
    if(use2){
      transfer(index); //transfer
    }
    return true;
  }else if(use2){
    index = probe(hash, str , false); //check second array
    //TO DO make properly check
    if(index >= 0){
      return true;
    }
  }
  return false;
}

char * HashTable::remove(const char *str) {
  //probe for the item, then remove the item,
  //removing means setting to "REMOVED" (DELTED MARKER)
  //TO DO remove
  checkRehash(); //checks if rehashing needs to ocurr, or if cleanup needs to ocurr

  unsigned int hash = hashCode(str) ;

  //find the next available spot
  int index = probe(hash, str, false); //overloaded probe

  char * returnVal;
  if(index >= 0){ //item was found
    returnVal = array[index];
    if(use2){
      transfer(index); //transfer
    }
    array[index] = DELETED; //free(array[index]);

    m_arraySize--;  //update size
    load = (float)m_arraySize/(float)m_arrayCapacity; //update load factor

    return returnVal;

  }else if(use2){//check other array
    index = probe(hash, str , false);
    if(index >= 0){
      returnVal = array2[index];
      array2[index] = DELETED;

      m_array2Size--;  //update size
      load2 = (float)m_array2Size/(float)m_array2Capacity; //update load factor

      return returnVal;
    }//else here means it was not found
  }

  return NULL; //return

  //returns NULL if item is not found
}


void HashTable::dump() {
  cout << "HashTable #1: size = " << m_arraySize << ", tableSize = " << m_arrayCapacity << endl;
  for(int i = 0; i < m_arrayCapacity; i++){
    cout << "H[ " << i << " ] = ";
    if(array[i] == DELETED){
      cout << "DELETED";
    }else if(array[i] != NULL){
      cout << array[i] ; //<< "Address: " << &array[i];
    }
    cout << endl;
  }

  if(use2){
    cout << "HashTable #2: size = " << m_array2Size << ", tableSize = " << m_array2Capacity << endl;
    for(int i = 0; i < m_array2Capacity; i++){
      cout << "H[ " << i << " ] = ";
      if(array2[i] == DELETED){
        cout << "DELETED";
      }else if(array2[i] != NULL){
        cout << array2[i];
      }
      cout << endl;
    }
  }


}
