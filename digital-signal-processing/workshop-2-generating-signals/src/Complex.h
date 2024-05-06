#ifndef COMPLEX_H
#define COMPLEX_H

#include <cmath>

class Complex
{
private:
    double real;
    double imag;

public:
    Complex(double real, double imag);
    ~Complex() = default;
    Complex operator*(Complex const &a) const;

    double getReal() const { return real; }
    double getImag() const { return imag; }
    double magnitude() const { return std::sqrt(real * real + imag * imag); }
    double phase() const { return std::atan2(imag, real); }
};

#endif // COMPLEX_H