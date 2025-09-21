#include "complex.h"

Complex::Complex(double r, double i) : R(r), I(i) {}

Complex Complex::operator+(const Complex& other) const{
   return Complex(R + other.R, I + other.I);
}
Complex Complex::operator-(const Complex& other) const{
   return Complex(R - other.R, I - other.I);
}
Complex Complex::operator*(double k) const{
   return Complex(R * k, I * k);
}
Complex Complex::operator*(const Complex& other) const{
   double a = R * other.R - I * other.I;
   double b = R * other.R + I * other.I;
   return Complex(a, b);
}

double Complex::modul() const{
   return std::sqrt(R * R + I * I);
}

void Complex::print() const{
   std::cout << R << " + " << I << "i";
}
