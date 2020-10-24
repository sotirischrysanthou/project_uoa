#pragma once


#include <string>


using namespace std;



// Pointer = void*
typedef void* Pointer;


// Pointer to function that compares 2 elements a and b and return:
// < 0  if a < b
//   0  if a and b are equal
// > 0  and a > b
typedef int (*CompareFunc)(Pointer a, Pointer b);

// Pointer to function that distroy an element value
typedef void (*DestroyFunc)(Pointer value);



