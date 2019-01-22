#ifndef _MEDIANHEAP_H_
#define _MEDIANHEAP_H_

#include <iostream>
using namespace std;

//heap class, manages the max/min heap
//max and min heap are individual heap

template <typename T>
class Heap{
public:

  //for whatever reason this constructor is needed
  Heap();

  //destructor (deletes array)
  ~Heap(){
    delete [] array;
    array = NULL;
  }

  //assignment operator
  //takes work away from MedianHeap;
  //const Heap<T>& operator=(const Heap<T>& rhs);

  //constructor, allocates array
  Heap( bool (*swapN) (const T& leftL , const T& rightL), int size){
    swapNeeded = swapN;
    array = new T[size]; //delete in destructor
    arrayCapacity = size;
    arraySize = 0;

  }
  //getter functions for size/capacity
  int getSize(){
    return arraySize;
  }

  int getCapacity(){
    return arrayCapacity;
  }

  //modifying functions
  T getTop(); //this is the min or max value
  //since it changes nothing and does not rebalance, min/max doesnt matter

  void insert(T value);

  void remove(int index); //this deletes the item

  //this goes along with removeMin/Max
  //ismax determines which one to call
  bool searchAndDestroy(T& givenItem, bool (*equalTo) (const T&, const T&));

  void dump(); //no inputs/outputs, just a print function

  int arraySize = 0;
  int arrayCapacity = 0;

  T* array;
//private: //EVERYTHING IS PUBLIC AND IT IS BAD, I can't get friend to work...

//  T* array; //I can't get friends to work, so the array is public

  //this manages the differences in operations between min and max heaps
  //for a maxheap a swap is needed when the upper value is smaller
  //for a minheap it is when the upper value is bigger
  //this stores a different operation for the min/max heap
  //this way only 1 variable is needed
  //swap syntax should always be: (higher in tree, deeper in tree)
  bool (*swapNeeded) (const T&, const T&); //this is public for copy constructor

};

template <typename T>
class MedianHeap{

public:

  MedianHeap();
  //constructor  (FROM PROJECT DESCRIPTION)
  // First two parameters are function pointers that compare items of type T.
  // The first function returns true if the first parameter is less than
  // the second parameter. The second function returns true if the first
  // parameter is greater than the second. The constructor must create a
  // MedianHeap object capable of holding cap items. The object created by
  // the constructor must be useable immediately.
  MedianHeap( bool (*lt) (const T& , const T& ),
  bool (*gt) (const T&, const T& ), int cap = 100) ;

  //copy constructor
  MedianHeap(const MedianHeap<T>& otherH) ;

  //destructor (this may be done automatically depeneding on design)
  ~MedianHeap();

  //overloaded assignment operator
  const MedianHeap<T>& operator=(const MedianHeap<T>& rhs)  ;


  //MUST BE CONSTANT TIME (stored by variable)
  int size() {
    return m_size;
  }
  //MUST BE CONSTANT TIME (stored by variable)
  int capacity(){
    return m_capacity;
  }

  //runs in O( log(n) )
  //runs along the height of a tree of size n
  void insert(const T& item);

  //returning a copy of the selected item
  T getMedian(){
    return m_median;
  }

  T getMin() {
    return m_min;
  }

  T getMax() {
    return m_max;
  }


  //runtime should be O(n)
  //this delets an item equal to the item judged by the equalTo function points
  bool deleteItem(T& givenItem, bool (*equalTo) (const T&, const T&) ) ;

  //print the data structure, does not delete
  void dump() ;


  /////GRADING FUNCTIONS
  //template <typename T>
  int maxHeapSize(){
    return maxHeap->arraySize;

  }

  //template <typename T>
  int minHeapSize() {
    return minHeap->arraySize;
  }

  //template <typename T>
  T locateInMaxHeap(int pos){
    T item = maxHeap->array[pos];
    return item;
  }

  //template <typename T>
  T locateInMinHeap(int pos){
    T item = minHeap->array[pos];
    return item;
  }

private:

  //pointer to min and maxheap, use ->
  Heap<T> * maxHeap;
  Heap<T> * minHeap;

  T m_max;
  T m_min;
  T m_median;

  int m_size;
  int m_capacity;

  bool (*less) (const T&, const T&);
  bool (*greater) (const T&, const T&);

  friend Heap<T>;
};


#include "MedianHeap.cpp"
#endif
