#include <iostream>
#include <cstdlib>

int main(int count, char* arg[]){
   if(count > 4){
      std::cout << "ERROR" << std::endl; //programm + 3 arguments
      return 1;
   }

   int a = std::atoi(arg[1]); //string -> int
   int b = std::atoi(arg[2]);
   int c = std::atoi(arg[3]);

   int min = a;
   int max = a;

   if(b < min){
      min = b;
   }
   if(c < min){
      min = c;
   }
   if(b > max){
      max = b;
   }
   if(c > max){
      max = c;
   }

   std::cout << "min-" << min << ", max-" << max << std::endl;
   return 0;
}
