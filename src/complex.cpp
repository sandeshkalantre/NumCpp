#include "complex.h"
//IMPLEMENTATION OF THE COMPLEX CLASS
Complex Complex::j(0.0,1.0);
Number Complex::real()
{
    return real;
}

Number Complex::img()
{
    return img;
}

Complex conjugate()
{
    return Complex(real,-img);
}

Number norm()
{
    return (real^2 + img^2)^0.5;
}

Complex Complex::operator+(const Complex c_num2)
{
    return Complex(real + c_num2.real,img + c_num2.img);
}

Complex Complex::operator-(const Complex c_num2)
{
    return Complex(real - c_num2.real,img - c_num2.img);
}

Complex Complex::operator*(const Complex c_num2)
{
    Number temp_real = (real * c_num2.real) - (img * c_num2.img);
    Number temp_img = (real * c_num2.img) + (img * c_num2.real);
    return Complex(temp_real,temp_img);
}

Complex Complex::operator/(const Complex c_num2)
{
    Complex temp = *this * c_num2.conjuagte();
    temp.real() = temp.real()/c_num2.norm();
    temp.img() = temp.img()/c_num2.norm();
    return temp;
}

Complex Complex::operator+=(const Complex c_num2)
{
    *this = *this + c_num2;
    return *this;
}

Complex Complex::operator-=(const Complex c_num2)
{
    *this = *this - c_num2;
    return *this;
}

Complex Complex::operator*=(const Complex c_num2)
{
    *this = *this * c_num2;
    return *this;
}

Complex Complex::operator+=(const Complex c_num2)
{
    *this = *this / c_num2;
    return *this;
}

bool Complex::operator==(const Complex c_num2)
{
    if(real != c_num2.real())
    {
        return false;
    }
    else if(img != c_num2.img())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Complex::operator!=(cont Complex c_num2)
{
    return !(*this == c_num2);
}

Complex Complex::operator-()
{
    *this.real = -real;
    *this.img = -img;
    return *this;
}

//IMPLEMENTATION OF THE COMPLEX_ARRAY CLASS
Complex_array::Complex_array(unsigned long _size)
{
    size = _size;
    for(int i = 0;i < size;i++)
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

    unsigned long index = 0;
    for(std::map<std::string,Number>::iterator it = array.begin();
        it != array.end();
        it++)
        {
            c_array[index] = Complex(it-> second,(it + 1) -> second);
            it++;
        }
}

unsigned long Complex_array::size()
{
    return size;
}

void Complex_array::store_value(const Complex c_num,unsigned long index)
{
    c_array[index] = c_num;
    return;
}

Complex return_value(unsigned long index)
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
            throw DIVIDE_BY_ZERO;
        }    
        for(int i = 0;i < size;i++)
        {
            c_array[i] /= Complex(scale);
        }
        return;
    }
    catch(const char* str)
    {
        std::cout<<"Domain Error : "<<str;
        return;
    }
}

void Complex_array::FFT(Complex_array c_array_fft,bool inverse = false)
{
    if(!inverse)
    {    
        Complex_FFT complex_fft;
        c_array_fft = complex_fft.forward_fft(*this);
        return;
    }
    else
    {
        Complex_FFT complex_fft;
        c_array_fft = complex_fft.inverse_fft(*this);
        return;
    }
    
}

//rearranges the array for implementation of the fft
void Complex_FFT::rearrange(Complex_array c_array)
{
    

