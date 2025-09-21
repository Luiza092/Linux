#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <cmath>

class Complex {
private:
   double R;
   double I;

public:
   Complex(double r = 0, double i = 0);

   Complex operator+(const Complex& other) const;
   Complex operator-(const Complex& other) const;
   Complex operator*(double k) const;
   Complex operator*(const Complex& other) const;

   double modul() const;
   void print() const;
};

#endif
