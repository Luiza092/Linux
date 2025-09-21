#include "complex.h"
#include "sort.h"
#include <iostream>
#include <vector>

int main(){
   std::vector<Complex> numbers = {
      Complex(5, 12),
      Complex(3, 4),
      Complex(0, 5),
      Complex(1, 8),
   };

   std::cout << "Before: " << std::endl;
   for(size_t i = 0; i < numbers.size(); ++i){
      numbers[i].print();
      std::cout << " | abs = " << numbers[i].modul() << std::endl;
   }

   sortComplex(numbers);
   std::cout << "After: " << std::endl;
   for(size_t i = 0; i < numbers.size(); ++i){ 
      numbers[i].print();
      std::cout << " | abs = " << numbers[i].modul() << std::endl;
   }

   return 0;
}
