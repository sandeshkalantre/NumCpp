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

Complex_array::Complex_array(ndArray& array)
{
    //std::vector<int> temp_dim_size = array.dim_size;
    //std::reverse(array.dim_size.begin(),array.dim_size.end());
    if(array.dim != 2)
    {
        throw DIM_MISMATCH;
    }
    if(array.dim_size[1] != 2)
    {
        throw DIM_MISMATCH;
    }
    i_size = array.dim_size[0];

    c_array = new Complex [i_size];
    if(c_array == NULL)
    {
        throw MALLOC_ERROR;
    }
    //unsigned long index = 0;
    /*
    for(std::map<std::vector<int>,Number>::iterator it = array.array.begin();
        it != array.array.end();
        it++)
        {
            Number num1 = it -> second;
            it++;
            Number num2 = it -> second;
            c_array[index] = Complex(num1,num2);
        }
    */
    std::vector<int> index(2);
    //array.show();
    for(int i = 0;i < array.dim_size[0];i++)
    {
        index[0] = i;
        index[1] = 0;
        Number num1 = array.array[index];
        index[1] = 1;
        Number num2 = array.array[index];
        //mpfr_printf("%.15RF %.15RF \n",num1.value,num2.value);
        store_value(Complex(num1,num2),i);

    }

    //array.dim_size = temp_dim_size;
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
    return return_value(index);
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


void Complex_array::forward_fft()
{
    rearrange();
    Number pi(-3.1415926535897932);
    //step is the level of the array we are working
    for(unsigned long step = 1; step < size(); step <<= 1)
    {
        const unsigned long jump = step << 1;

        Number n_step((int)step);
        Number delta = pi / n_step;

        Number sine = std_functions::sin(delta / Number(2));

        Complex multiplier(-Number(2.0) * sine * sine,std_functions::sin(delta));

        Complex factor(1.0);

        for(unsigned long group = 0;group < step;group++)
        {
            for(unsigned long pair = group;pair < size();pair+=jump)
            {
                unsigned long match = pair + step;
                Complex product = factor * c_array[match];
                c_array[match] = c_array[pair] - product;
                c_array[pair] += product;
            }
            factor = multiplier * factor + factor;
        }
    }
    return;
}

void Complex_array::inverse_fft()
{
    rearrange();
    Number pi(3.1415926535897932);
    //step is the level of the array we are working
    //the cooley-tuckey algorithm
    for(unsigned long step = 1; step < size(); step <<= 1)
    {
        const unsigned long jump = step << 1;

        Number n_step((int)step);
        Number delta = pi / n_step;

        Number sine = std_functions::sin(delta / Number(2));

        Complex multiplier(-Number(2.0) * sine * sine,std_functions::sin(delta));

        Complex factor(1.0);

        for(unsigned long group = 0;group < step;group++)
        {
            for(unsigned long pair = group;pair < size();pair+=jump)
            {
                unsigned long match = pair + step;
                Complex product = factor * c_array[match];
                c_array[match] = c_array[pair] - product;
                c_array[pair] += product;
            }
            factor = multiplier * factor + factor;
        }
    }
    scale(Number((int)size()));
    return;
}
//rearranges the array for implementation of the fft
void Complex_array::rearrange()
{
    //Swap position
   unsigned int Target = 0;
   //Process all positions of input signal
   for (unsigned int Position = 0; Position < size(); ++Position)
   {
      //   Only for not yet swapped entries
      if (Target > Position)
      {
         //   Swap entries
         Complex Temp(c_array[Target]);
         c_array[Target] = c_array[Position];
         c_array[Position] = Temp;
      }
      //   Bit mask
      unsigned int Mask = size();
      //   While bit is set
      while (Target & (Mask >>= 1))
         //   Drop bit
         Target &= ~Mask;
      //   The current bit is 0 - set it
      Target |= Mask;
   }
   return;

}
