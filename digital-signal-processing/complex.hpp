#ifndef COMPLEX_H
#define COMPLEX_H

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
};

#endif // COMPLEX_H