#include <iostream>

int main(int count, char* arg[]){
   for(int i = count - 1; i > 0;  --i){
      std::cout << arg[i] << " ";
   }

   std::cout << std::endl;
   return 0;
}
