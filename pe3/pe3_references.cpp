#include <iostream>

// Name: Kyle Van Meter
//

// Write any functions you need here!

void intPtr(int * param) {
}

void intRef(int & param) {
}

int main() {
  // Add as many prints to cout as you need to answer the questions.
  // Leave them in your code when you turn it in.
  // If you have lines of code that cause errors that test the questions,
  // leave this code in your file but comment it out.

  // 1) Declare an int
  int tmp;

  // 2) Declare a pointer to that int
  int *p = &tmp;

  // 3) Increment the int via the pointer
  *p += 1;

  // 4) Declare a reference to your int
  int &q = *p;

  // 5) Increment the int via the reference
  q = *p++;

  // 6) When you increment the int via the variable declared in #1, from which
  // variables can you see the changes?
  // Answer: int tmp;

  // 7) When you increment the int via the pointer declared in #2, from which
  // variables can you see the changes?
  // Answer: int *p, int tmp;

  // 8) When you increment the int via the reference declared in #4, from which
  // variables can you see the changes?
  // Answer: int *p, int tmp;

  // 9) Write a function that takes an int * parameter. How would you pass the
  // variable from #1 into this function? (write the function call below)
  intPtr(&tmp);

  // 10) Can you pass your reference from #4 to the function from #9 without accessing its address?
  // Answer: Yes?  Passing a reference means that the object q would have the memory address of p as its values

  // 11) Write a function that takes an int & parameter. How would you pass the
  // variable from #1 into this function? (write the function call below)
  intRef(tmp);

  // 12) Can you pass your pointer from #2 to the function from #11 without dereferencing it?
  // Answer: No

  // 13) Can you pass your reference from #4 to the function from #11?
  // Answer: Yes
}
