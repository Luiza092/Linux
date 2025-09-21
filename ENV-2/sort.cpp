#include "sort.h"
#include <algorithm>

void sortComplex(std::vector<Complex>& arr)  {
   for(size_t i = 0; i < arr.size(); ++i){
      for(size_t j = 0; j < arr.size() - 1; ++j){
         if(arr[j].modul() > arr[j + 1].modul()){
            std::swap(arr[j], arr[j + 1]);
         }
      }
   }
}
