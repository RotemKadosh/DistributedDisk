#ifndef ILRD_94_COMPLEX_IMPL_HPP
#define ILRD_94_COMPLEX_IMPL_HPP

#include <iostream> // std :: ostream, istream
#include "cmplx_num.hpp"

namespace ilrd
{

const int buff_size = 20; 

Complex::Complex(const double real_,const double imaginary_):
 m_real(real_),
 m_imaginary(imaginary_)
{}
Complex::Complex(const Complex& other_):
 m_real(other_.m_real),
 m_imaginary(other_.m_imaginary)
{}

inline double Complex::GetReal() const
{
    return this->m_real;
}

inline double Complex::GetImaginary() const
{
    return this->m_imaginary;
}

inline void Complex::SetReal(double value)
{
    this->m_real = value; 
}

inline void Complex::SetImaginary(double value)
{
    this->m_imaginary = value; 
}

inline Complex& Complex::operator+=(const Complex& other_)
{
    m_real += other_.GetReal();
    m_imaginary += other_.GetImaginary();
    return *this;
}

inline Complex& Complex::operator-=(const Complex& other_)
{
     m_real -= other_.GetReal();
    m_imaginary -= other_.GetImaginary();
    return *this;
}

inline Complex& Complex::operator*=(const Complex& other_)
{
    double real = m_real;
    double imaginary = m_imaginary;
    double o_real = other_.GetReal();
    double o_imaginary = other_.GetImaginary();
    m_real = (real * o_real) - (imaginary * o_imaginary);
    m_imaginary = ((real * o_imaginary) + (o_real * imaginary));
    
    return *this;
}
inline Complex& Complex::operator/=(const Complex& other_)
{
    double a = m_real;
    double b = m_imaginary;
    double c = other_.GetReal();
    double d = other_.GetImaginary();
    double mult = (1 / ((c * c) + (d * d)));
    m_real = mult*((a * c) + (d * b));
    m_imaginary = mult * ((b * c) - (a * d));
    return *this;
}


inline std::ostream& operator<<(std::ostream& os_, const Complex& cmplx_)
{
    return os_ << cmplx_.GetReal() << " + i*("<< cmplx_.GetImaginary()<< ")\n";
}

inline std::istream& operator>>(std::istream& is, Complex& cmplx_)
{
    double real = 0.0, imaginary = 0.0;
    std::cout<<"enter number to real\n";
    is >> real;
    std::cout<<"enter number to imaginary\n";
    is >> imaginary;

    cmplx_.SetReal(real);
    cmplx_.SetImaginary(imaginary);
    return is;
}

inline bool operator==(const Complex& cmplx_, const Complex& cmplx_to_compare_)
{
    return(cmplx_.GetReal() == cmplx_to_compare_.GetReal() &&
            cmplx_.GetImaginary() == cmplx_to_compare_.GetImaginary());
}

inline bool operator!=(const Complex& cmplx_, const Complex& cmplx_to_compare_)
{
    return !(cmplx_ == cmplx_to_compare_);
}

inline Complex operator+(const Complex& cmplx_, const Complex& other_)
{
    return Complex(cmplx_.GetReal() + other_.GetReal(),
                    cmplx_.GetImaginary() + other_.GetImaginary());
}

inline Complex operator-(const Complex& cmplx_, const Complex& other_)
{
    Complex result(cmplx_.GetReal(), cmplx_.GetImaginary());
    result -= other_;
       
       
    return result;
}

inline Complex operator*(const Complex& cmplx_, const Complex& other_)
{
    Complex result(cmplx_.GetReal(), cmplx_.GetImaginary());
    result *= other_;
    
    return result;

}

inline Complex operator/(const Complex& cmplx_, const Complex& other_)
{
    Complex result(cmplx_.GetReal(), cmplx_.GetImaginary());
    result /= other_;
       
    return result;
}

}
#endif //ILRD_94_COMPLEX_IMPL_HPP;






