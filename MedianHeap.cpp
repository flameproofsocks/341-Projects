#ifndef _MEDIANHEAP_CPP_
#define _MEDIANHEAP_CPP_

#include "MedianHeap.h"
#include <iostream>

using namespace std;

template <typename T>
Heap<T>::Heap(){

}


//2*i  (left side )  2*i + 1 (right side)
//the insert function for a maxheap
template <typename T>
void Heap<T>::insert(T value){

  // cerr << "Pre-Insert:  ";
  // for(int i = 1; i <= arraySize; i++){
  //   cout << array[i] << " ";
  // }
  // cout << endl;


  if(arraySize < arrayCapacity){
    int i = arraySize + 1; //this should be the next open spot
    arraySize++; // adds to the size

    int j = 0; // holder location
    T holder;

    //add the value
    array[i] = value;

    //trickle the value up for proper order
    while(i >= 1){

      //cerr << "[  ]";
      //this determines parent and child to swap
      //left child
      if(i%2 == 0){
        j = i;
        i = (i)/2;
      }else{ //right child
        j = i;
        i = (i-1)/2;
      }

      //determine if swap is needed
      if( swapNeeded(array[i] , array[j] )  ){
        //cerr << "swaperation!!: " << i << " - " << j << "\n";
        //perform the swap, then quit when no swap needed
        holder = array[i]; //hold i (parent)
        array[i] = array[j]; //put child -> parent
        array[j] = holder; //put parent -> child

      }else{
        return; //swaperation complete !
      }

    }
  }else{ //throw an error, too many items
    //cerr << "SIZE: " << arraySize << " capacity: " << arrayCapacity;
    throw out_of_range("Too many items!");
  }

}


template <typename T>
T Heap<T>::getTop(){
  if(arraySize > 0){
    return array [1];
    //does not change size
  }
}

//this is used to remove specific positions
template <typename T>
void Heap<T>::remove(int index){

  //check if the item can be removed
  if(index <= arraySize + 1){
    //remove the item, then replace as needed
    //I only replace the item, removing the item (setting to null)
    //needs every item type to have the same null

    //take the last item, and trickle it down
    array[index] = array[arraySize]; //replace with last
    //arraySize--; // update the size
    //by decreasing the size the last item is "deleted"
    //tthe last item is out of range, and thus "deleted"

    //i is lower down than j (deeper in the tree, bigger index)
    int i = index;
    int j = 0; // holder location
    T holder;

    //trickle down the value up for proper order
    //very similar to a regular insert, with different ordering
    while(i < arraySize){

      //all that changes between min and max is < or >
      //determine if swap is needed (it should be most times)
      if( swapNeeded(array[j], array[i]) ){
        //perform the swap, then quit
        holder = array[i]; //hold i (parent)
        array[i] = array[j]; //put child -> parent
        array[j] = holder; //put parent -> child

      }else{
        //return; //swaperation complete !
      }

      //indexing is second to have proper while loop
      //prefer to pull from/ the right,
      //maxheap pulls from the greater child
      //swapNeeded is ordered as (highervalue, deepervalue)
      //for this case you want the bigger value in a maxheap
      if( swapNeeded (array[2*i + 1] , array[2*i] ) ){
        //pull left (left side of tree, lower in array)
        j = i;
        i = 2*i;
      }else{ //pull from the right
        j = i;
        i = 2*i + 1;
      }

    }
    arraySize--; // update the size

  }else{ //throw an error, too many items
    throw out_of_range("Too many items!");
  }

}


template <typename T>
bool Heap<T>::searchAndDestroy(T& givenItem, bool (*equalTo) (const T&, const T&) ){
  //loops through the array and calls delete if it's mon or max
  for(int i = 1; i < arraySize + 1; i++){
    if(equalTo(givenItem, array[i])){
      //this means the item was found
      remove(i); //remove and return true
      return true;
    }
  }
  return false; //item not found

}

//prints the entire array (max or min)
template <typename T> //dont think i need to template this one
void Heap<T>::dump(){

  //meant to look like output files
  cout << "size = " << arraySize << ", capacity = " << arrayCapacity << endl;
  for(int i = 1; i <= arraySize; i++){
    cout << "Heap[" << i << "] = (" << array[i] << ") \n";
    //formatted like :   Heap[2] = (91)
  }
}


/////////////////////////////////////////////////////////////////////////
//constructor  (FROM PROJECT DESCRIPTION)
template <typename T>
MedianHeap<T>::MedianHeap(){
  m_capacity = 100;
}
// First two parameters are function pointers that compare items of type T.
// The first function returns true if the first parameter is less than
// the second parameter. The second function returns true if the first
// parameter is greater than the second. The constructor must create a
// MedianHeap object capable of holding cap items. The object created by
// the constructor must be useable immediately.
template <typename T>
MedianHeap<T>::MedianHeap( bool (*lt) (const T& leftL , const T& rightL), bool (*gt) (const T& leftG, const T& rightG), int cap) {

//MedianHeap<T>::MedianHeap( bool (*lt) (const T&, const T&), bool (*gt) (const T&, const T&), int cap){
  //because rebalancing needs an imbalance of 1 or more between min and max heap
  //the size of the min/max heap should be: n/2 + 2.

  //the difference between a minHeap and a maxHeap is the comparison that
  //takes place, switching the operator will make a maxheap a maxheap and
  //a minheap a minheap
  maxHeap = new Heap<T>(lt, cap/2 + 2);
  minHeap = new Heap<T>(gt, cap/2 + 2);

  m_capacity = cap;
  m_size = 0;

  //set the less thans and greater thans
  less = lt;
  greater = gt;

}

//destructor (this may be done automatically depeneding on design)
template <typename T>
MedianHeap<T>::~MedianHeap() {
  //delete everything made by new
  //this will call delete on the min and max heap
  //minHeap->~Heap();
  delete minHeap;
  minHeap = NULL;
  //maxHeap->~Heap();
  delete maxHeap;
  maxHeap = NULL;

}

//overloaded assignment operator
template <typename T>
const MedianHeap<T>& MedianHeap<T>::operator=(const MedianHeap<T>& rhs){
  //copy heap and copy variable values
  //gaurd against self assignment
  if(this == &rhs){
    return *this;
  }

  //set all the variables equal.
  //set the x, y, leaf equal
  m_max = rhs.m_max;
  m_min = rhs.m_min;
  m_median = rhs.m_median;

  m_size = rhs.m_size;
  m_capacity = rhs.m_capacity;

  less = rhs.less;
  greater = rhs.greater;

  //make min and maxheap
  maxHeap = new Heap<T>(less, m_capacity/2 + 2);
  minHeap = new Heap<T>(greater, m_capacity/2 + 2);

  //copy Heap;
  //size and capacity
  maxHeap->arraySize = rhs.maxHeap->arraySize;
  minHeap->arraySize = rhs.minHeap->arraySize;
  maxHeap->arrayCapacity = rhs.maxHeap->arrayCapacity;
  minHeap->arrayCapacity = rhs.minHeap->arrayCapacity;

  //copy array
  for(int i = 1; i < minHeap->arraySize + 1; i++){
    minHeap->array[i] = rhs.minHeap->array[i];
  }
  for(int i = 1; i < maxHeap->arraySize + 1; i++){
    maxHeap->array[i] = rhs.maxHeap->array[i];
  }

  return *this;
}

//Copy Constructor
template <typename T>
MedianHeap<T>::MedianHeap(const MedianHeap<T>& otherH) {
  //set all the variables equal.
  //set the x, y, leaf equal
  m_max = otherH.m_max;
  m_min = otherH.m_min;
  m_median = otherH.m_median;

  m_size = otherH.m_size;
  m_capacity = otherH.m_capacity;

  less = otherH.less;
  greater = otherH.greater;

  maxHeap = new Heap<T>(less, m_capacity/2 + 2);
  minHeap = new Heap<T>(greater, m_capacity/2 + 2);

  //copy Heap;
  //size and capacity
  maxHeap->arraySize = otherH.maxHeap->arraySize;
  minHeap->arraySize = otherH.minHeap->arraySize;
  maxHeap->arrayCapacity = otherH.maxHeap->arrayCapacity;
  minHeap->arrayCapacity = otherH.minHeap->arrayCapacity;

  //copy array (for loop)
  for(int i = 1; i < minHeap->arraySize + 1; i++){
    minHeap->array[i] = otherH.minHeap->array[i];
  }
  for(int i = 1; i < maxHeap->arraySize + 1; i++){
    maxHeap->array[i] = otherH.maxHeap->array[i];
  }

}

//runs in O( log(n) )
//runs along the height of a tree of size n
template <typename T>
void MedianHeap<T>::insert(const T& item){


  //define the median if needed //only done once
  //this saves from comparisons against uninitialized values
  if(m_size == 0){
    m_median = item;
    m_max = item;
    m_min = item;
    maxHeap->insert(item); //insert into max by default
    m_size++;
    return;
  }
  //call to insert in max or min heap //add + 1???
  if( less(item , m_median) && maxHeap->arraySize <= minHeap->arraySize){

    //redefeine max if its bigger or there is no max
    if( less(item , m_min) ){
      m_min = item;
    }

    //cout << "MAX-";
    maxHeap->insert(item);  //insert maxheap

  }else if( greater(item , m_median) && minHeap->arraySize <= maxHeap->arraySize){

    //redefine minimum if its smaller, or if there is no min
    if( greater(item , m_max) ){
      m_max = item;
    }
    //cout << "MIN-";
    minHeap->insert(item);  //insert minheap

  }else{ //assuming there aren't duplicates this means to rebalance

    //redefine the median
    //1: add the median to the smaller heap
    //2: redefine median to be top of bigger heap
    //3: delete from bigger heap
    //4: add new value to bigger heap

    if(maxHeap->arraySize > minHeap->arraySize){ //take from max

      minHeap->insert(m_median);
      m_median = maxHeap->getTop();
      maxHeap->remove(1); //removes top item
      maxHeap->insert(item);

    }else{ //take from min

      maxHeap->insert(m_median);
      m_median = minHeap->getTop();
      minHeap->remove(1); //removes top item
      minHeap->insert(item);

    }

  }
  m_size++;

}


//runtime should be O(n)
//to find the equal item, this traverses the list (min and max)
//with in order search (linear time) then deletes the item and sorts in log(n)
// n + log(n) = n  // O(n) runtime
//this delets an item equal to the item judged by the equalTo function points546541

template <typename T>
bool MedianHeap<T>::deleteItem(T& givenItem, bool (*equalTo) (const T&, const T&) ) {

  if(m_size > 0){
    m_size--;
    //check the median
    if(equalTo(m_median, givenItem) ){

      //decide which heap to take from
      if(maxHeap->arraySize > minHeap->arraySize){ //take from max
        m_median = maxHeap->getTop();
        maxHeap->remove(1); //removes top item

      }else { //take from min
        m_median = minHeap->getTop();
        minHeap->remove(1); //removes top item
      }
      return true; //return as the item was deleted
    }
    //this is only evaluated if the item was not the median
    return (minHeap->searchAndDestroy(givenItem, equalTo) || maxHeap->searchAndDestroy(givenItem, equalTo) );
    //this short circuit code should only run one if it returns true
  }
}


//print the data structure, does not delete
template <typename T>
void MedianHeap<T>::dump(){

  cout << "Size: " << m_size << endl;
  //this is to look nice
  cout << "... MedianHeap::dump() ...\n\n";
  cout << "------------Max Heap------------";
  maxHeap->dump();

  cout << "\n------------Min Heap------------";
  minHeap->dump();

  cout << "--------------------------------\n\n";
  cout << "min\t= " << m_min;
  cout << "median\t= " << m_median;
  cout << "max\t= " << m_max << endl;

}


#endif
