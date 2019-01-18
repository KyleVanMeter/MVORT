#include <iostream>

// Names (be sure to include your last initial!):
//

// 1) Write a function that tests whether or not ints are pass-by-value or pass-by-reference
// (do not use pointers or c++ references in this function)
// Answer:
void testOne(int x, int y) {
  int temp;

  temp = x;
  x = y;
  y = temp;

  std::cout << "In scope testOne.  x = " << x << ", y = " << y;
}

// 2) Write a function that tests whether strings (std::string) are pass-by-value or pass-by-reference
// (do not use pointers or c++ references in this function)
// Answer:
void testTwo(std::string x, std::string y) {
  std::string temp;

  temp = x;
  x = y;
  y = temp;

  std::cout << "In scope testTwo.  x = " << x << ", y = " << y;
}


// 3) Write a function that tests whether arrays are pass-by-value or pass-by-reference
// recall: to initialize an array-- type name[] = {comma separated elements};
// (do not use pointers or c++ references in this function)
// Answer:
void testThree(int x[4], int y[4]) {
  //since int x[4], and int y[4] are of type int* then by default it passes by reference
  int* temp;
  int* a = x;
  int* b = y;

  temp = a;
  a = b;
  b = temp;

  std::cout << "In scope testThree.  x[0] = " << x[0] << ", y[0] = " << y[0];
  std::cout << "\nIn scope testThree.  a[0] = " << a[0] << ", b[0] = " << b[0];
}

int main() {
    // code to test your functions goes here.
  int x = 10;
  int y = 15;
  testOne(x, y);
  std::cout << "\nIn scope main.  x = " << x << ", y = " << y << "\n\n";

  std::string a = "abc";
  std::string b = "def";
  testTwo(a, b);
  std::cout << "\nIn scope main.  a = " << a << ", b = " << b << "\n\n";

  int c[4] = {1,2,3,4};
  int d[4] = {5,6,7,8};
  testThree(c, d);
  std::cout << "\nIn scope main.  c[0] = " << c[0] << ", d[0] = " << d[0] << "\n\n";
}
