#include <iostream>

using namespace std;
#define STOP_WIDTH 800


void add(int i, int term, int * list ){

  //base case
  if( i >= STOP_WIDTH*STOP_WIDTH){
    return;
  }else{
    if(i != term)
      list[i] = list[i] + 1;
    add(i + term, term, list );

  }
}

int main(){

  int * theList;
  cout << "Recursion is easy once its not hard\n";
  theList = new int[STOP_WIDTH*STOP_WIDTH];
  for(int i = 0; i < STOP_WIDTH*STOP_WIDTH; i++){
    theList[i] = 0; //initialize list
  }

  for(int i = 2; i < STOP_WIDTH; i ++){
    if(theList[i] == 0){
      cout << "Trying: " << i << endl;
        add(i, i, theList); //prime
    }

  }

  cout << "\nLIST IS: " << endl << endl;
  for(int i = 0; i < STOP_WIDTH*STOP_WIDTH; i++){

    cout << theList[i] << " ";
    if(i%STOP_WIDTH == 0){
      cout << endl;
    }
  }
  cout << endl;
  cout << "Primes are:\n\n";
  for(int i = 0; i < STOP_WIDTH*STOP_WIDTH; i++){
    if(theList[i] == 0){
      cout << i << ", ";
    }

  }
  cout << endl << endl;

}
