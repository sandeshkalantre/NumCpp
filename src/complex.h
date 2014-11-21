/*
This file contains the implementation of the complex class
used in the calculation of the FFT
*/
#ifndef COMPLEX_H
#define COMPLEX_H

#include "parser.hpp"

//Number class from parser.hpp
class Number;
//ndArray class from parser.hpp
//class ndArray;

/*
//class of complex numbers
class Complex
{
    private:
        //internal representation i_
        Number i_real;
        Number i_img;
    public:
        //const j:sqrt(-1)
        //static const Complex j;

        //constructors
        Complex(): i_real(0.0),i_img(0.0){};
        Complex(Number _real,Number _img): i_real(_real),i_img(_img){};
        //real number
        Complex(Number _real):i_real(_real),i_img(0.0){};

        //assignment for a real number
        Complex operator=(const Number _real)
        {
            i_real = _real;
            i_img = 0.0;
            return *this;
        }

        //return the real and imaginary parts respectively
        Number real();
        Number img();

        //conjugate
        //does not change the original complex number
        Complex conjugate();

        //the norm of the complex number
        Number norm();

        //the operators overloaded for members of the Complex class
        Complex operator+(Complex c_num2);
        Complex operator-(Complex c_num2);
        Complex operator*(Complex c_num2);
        Complex operator/(Complex c_num2);
        Complex operator+=(Complex c_num2);
        Complex operator-=(Complex c_num2);
        Complex operator*=(Complex c_num2);
        Complex operator/=(Complex c_num2);
        bool operator==(Complex c_num2);
        bool operator!=(Complex c_num2);
        Complex operator-();
};

class Complex_array
{
    private:
        unsigned long i_size;
        Complex* c_array;// = new Complex[i_size];
    public:
        unsigned long size();
        void store_value(const Complex c_num,unsigned long index);
        Complex return_value(unsigned long index);

        //constructors
        Complex_array(ndArray array);
        //arrays of complex numbers with zeros
        Complex_array(unsigned long _size);

        //subscripting operator
        Complex operator[](unsigned long index);

        //scale the array by a real number
        void scale(Number scale);

        //the fft function
        void FFT(Complex_array c_array_fft);
};

class Complex_FFT
{
    public:
        Complex_array forward_fft(const Complex_array c_array);
        Complex_array inverse_fft(const Complex_array c_array);
    private:
        void rearrange(Complex_array c_array);
        void fft(Complex_array c_array,bool inverse = false);
};
*/

#endif
