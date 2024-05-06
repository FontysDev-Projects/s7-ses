#include "Complex.h"

Complex::Complex(double real, double imag)
    : real(real), imag(imag)
{
}

Complex Complex::operator*(Complex const &other) const
{
    return Complex(real * other.real - imag * other.imag, real * other.imag + imag * other.real);
}
