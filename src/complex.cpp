/*
This file contains the implementation of the Complex and Complex_array classes
which are used for FFT implementation
*/

#include "parser.hpp"
//IMPLEMENTATION OF THE COMPLEX CLASS
const Complex Complex::j(0.0,1.0);

//assignment for a real number
Complex Complex::operator=(Number _real)
{
    i_real = _real;
    i_img = 0.0;
    return *this;
}

Number Complex::real()
{
    Number temp_r = i_real;
    return temp_r;
}

Number Complex::img()
{
    Number temp_img = i_img;
    return temp_img;
}

Complex Complex::conjugate()
{
    return Complex(real(),-img());
}

Number Complex::norm()
{
    return std_functions::hypot(real(),img());
}

Complex Complex::operator+(Complex c_num2)
{
    return Complex(real() + c_num2.real(),img() + c_num2.img());
}

Complex Complex::operator-(Complex c_num2)
{
    return Complex(real() - c_num2.real(),img() - c_num2.img());
}

Complex Complex::operator*(Complex c_num2)
{
    Number temp_real = (real() * c_num2.real()) - (img() * c_num2.img());
    Number temp_img = (real() * c_num2.img()) + (img() * c_num2.real());
    return Complex(temp_real,temp_img);
}

Complex Complex::operator/(Complex c_num2)
{
    Complex temp = *this * c_num2.conjugate();
    temp.real() = temp.real()/c_num2.norm();
    temp.img() = temp.img()/c_num2.norm();
    return temp;
}

Complex Complex::operator+=(Complex c_num2)
{
    *this = *this + c_num2;
    return *this;
}

Complex Complex::operator-=(Complex c_num2)
{
    *this = *this - c_num2;
    return *this;
}

Complex Complex::operator*=(Complex c_num2)
{
    *this = *this * c_num2;
    return *this;
}

Complex Complex::operator/=(Complex c_num2)
{
    *this = *this / c_num2;
    return *this;
}

bool Complex::operator==(Complex c_num2)
{
    if(real() != c_num2.real())
    {
        return false;
    }
    else if(img() != c_num2.img())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Complex::operator!=(Complex c_num2)
{
    return !(*this == c_num2);
}

Complex Complex::operator-()
{
    this->real() = -real();
    this->img() = -img();
    return *this;
}

//IMPLEMENTATION OF THE COMPLEX_ARRAY CLASS
Complex_array::Complex_array()
{
     Complex* c_array = new Complex [64];
     if(c_array == NULL)
     {
        throw MALLOC_ERROR;
     }
}
Complex_array::Complex_array(unsigned long _size)
{
    i_size = _size;
    Complex* c_array = new Complex [size()];
    if(c_array == NULL)
    {
        throw MALLOC_ERROR;
    }
    for(unsigned long i = 0;i < i_size;i++)
    {
        c_array[i] = 0.0;
    }
}

Complex_array::Complex_array(ndArray array)
{
    if(array.dim != 2)
    {
        throw DIM_MISMATCH;
    }
    if(array.dim_size[1] != 2)
    {
        throw DIM_MISMATCH;
    }
    i_size = array.dim_size[0];
    Complex* c_array = new Complex [size()];
    if(c_array == NULL)
    {
        throw MALLOC_ERROR;
    }
    unsigned long index = 0;
    for(std::map<std::vector<int>,Number>::iterator it = array.array.begin();
        it != array.array.end();
        it++)
        {
            c_array[index] = Complex(it-> second,(++it) -> second);
        }
}

unsigned long Complex_array::size()
{
    return i_size;
}

void Complex_array::store_value(const Complex c_num,unsigned long index)
{
    c_array[index] = c_num;
    return;
}

Complex Complex_array::return_value(unsigned long index)
{
    return c_array[index];
}

Complex Complex_array::operator[](unsigned long index)
{
    return c_array[index];
}

void Complex_array::scale(Number scale)
{
    try
    {
        if(scale == Number(0.0))
        {
            throw DIVISION_BY_ZERO;
        }
        for(unsigned long i = 0;i < size();i++)
        {
            c_array[i] /= Complex(scale);
        }
        return;
    }
    catch(const char* str)
    {
        std::cout<<str;
        return;
    }
}


void Complex_array::forward_fft(Complex_array c_array_fft)
{

    return;
}





//rearranges the array for implementation of the fft
void Complex_array::rearrange()
{
    return;

}
