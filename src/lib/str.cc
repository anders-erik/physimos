#include <cstring>
#include <iostream>

#include "str_std.hh"

#include <stdexcept>
#include <vector>

#include <cstdio>  // snprintf - float constructor

#include "str.hh"
#include "str_util.hh"
#include "alloc.hh"
#include "print.hh"
// #include "arr.hh"



Str::
Str(const char *c_str) 
{
    size_str = std::strlen(c_str);
    allocate(size());
    memcpy(mem, c_str, size());
}

Str::
Str(unsigned int size, char set_char)
{
    size_str = size;
    allocate(size_str);
    fill_alloc(set_char);
}


Str::
Str(const Str& other) 
{
    if(this != & other)
    {
        allocate(other.capacity());
        memcpy(mem, other.data(), other.size());
        size_str = other.size();
    }
};


Str::
Str(Str&& other) 
{
    if(this != & other)
    {
        mem = other.data();
        size_alloc = other.capacity();
        size_str = other.size();
        other.rob();
    }
}


Str::
~Str()
{
    release_mem();
}




Str& Str::
operator=(const Str& other)
{
    if(this != &other)
    {
        allocate(other.capacity());
        size_str = other.size();
        memcpy(mem, other.data(), size_str);
    }

    return *this;
}


Str& Str::
operator=(Str&& other)
{
    if (this != &other)
    {
        mem = other.mem;
        size_str = other.size_str;
        size_alloc = other.size_alloc;
        other.rob();
    }

    return *this;
}


bool Str::
operator==(const Str & rhs) const
{
    if(this->size() != rhs.size())
        return false;

    for(size_t i = 0; i < this->size(); i++)
    {
        if(this->operator[](i) != rhs[i])
            return false;
    }

    return true;
}

bool Str::operator<(const Str & other) const 
{
    uint min_size = size() < other.size() ? size() : other.size();

    // compare chars for least shared size
    for(uint i=0; i<min_size; i++)
    {
        if((*this)[i] < other[i])
            return true;
        if((*this)[i] > other[i])
            return false;
    }

    if(size() < other.size())
        return true;

    return false;
}

bool Str::operator>=(const Str & other) const
{
    return !(*this < other);
}





Str& Str::
operator+=(Str&& rhs)
{
    this->append(rhs);
    return *this;
}


Str& Str::
operator+=(const Str & rhs)
{
    this->append(rhs);
    return *this;
}

// Str& Str::operator+=(char ch)
// {
//     return *this += Str::CH(ch);
// }


Str& Str::
operator+=(const char * c_str)
{
    Str rhs {c_str};
    this->append(rhs);
    return *this;
}



Str Str::
operator+(const Str& rhs)
{
    Str new_str = *this;
    new_str.append(rhs);
    return new_str;
}


Str Str::
operator+(const char * c_str)
{
    Str new_str = *this;
    new_str.append(Str(c_str));
    return new_str;
}




char& Str::
operator[](size_t index)
{
    return *(mem+index);
}

const char Str::
operator[](size_t index) const
{
    if(index >= size_str)
        throw std::runtime_error("Str: index access out of bounds.");

    return *(mem+index);
}



void Str::allocate(unsigned int size_to_alloc)
{
    mem = Alloc::allocate_bytes(size_to_alloc);
    size_alloc = size_to_alloc;
}


void Str::reallocate(unsigned int new_size_alloc)
{
    mem = Alloc::reallocate_bytes(mem, size_alloc, new_size_alloc);
    size_alloc = new_size_alloc;
}


void Str::release_mem()
{
    if(has_capacity())
        Alloc::deallocate_bytes(mem);

    mem = nullptr;
    size_alloc = 0;
    size_str = 0;
}

void Str::
rob()
{
    mem = nullptr;
    size_alloc = 0;
    size_str = 0;
}



char* Str::
data() const 
{
    return mem;
}

unsigned int Str::
size() const 
{
    return size_str;
}

unsigned int Str::
capacity() const 
{
    return size_alloc;
}

void Str::
pop_back()
{
    size_str--;
}

void Str::
reserve(unsigned int new_alloc_size)
{
    if(new_alloc_size <= size_alloc)
        return;
    
    if(mem == nullptr)
        allocate(new_alloc_size);
    else
        reallocate(new_alloc_size);
}


bool Str::
has_size() const 
{
    return size() > 0 ? true : false;
}


bool Str::
has_capacity() const 
{
    return capacity() > 0 ? true : false;
}

void Str::
fill_alloc(char fill_char)
{
    memset(mem, fill_char, size_alloc);
    size_str = size_alloc;
}


void Str::
clear()
{
    release_mem();
}

void Str::append(const Str& str_to_append){

    unsigned int combined_str_size = size_str + str_to_append.size();

    if(size_alloc < combined_str_size )
        reallocate(combined_str_size);
    
    memcpy(mem+size_str, str_to_append.data(), str_to_append.size());
    
    size_str = combined_str_size;
}

Str& Str::cut_to_substr(unsigned int pos, unsigned int new_size){

    if(pos > size_str)
        throw std::out_of_range("Str::cut_to_substr: pos > size_str.");
    

    unsigned int new_size_bounded = new_size;
    if(pos + new_size > size_str)
        new_size_bounded = size_str - pos;


    if(new_size_bounded == size_str)
        return *this;


    // ok substring here


    // not memcpy as src and dest overlap
    memmove(mem, mem+pos, new_size_bounded);

    // shrink allocation size and null-terminate
    reallocate(new_size_bounded+1);
    *(mem+new_size_bounded) = 0x0; 

    size_str = new_size_bounded;

    return *this;
}

Str Str::
substr(unsigned int pos, unsigned int new_size)
{
    Str copy = *this;
    return copy.cut_to_substr(pos, new_size);
}

Str Str::indent_space(unsigned int indent_count)
{
    if(indent_count == 0)
        return *this;
    
    Str ret_str;
    Str indent {indent_count, ' '};

    Arr<Str> split = StrUtil::split(*this, '\n');
    if(split.last() == "")
        split.pop_back();

    for(size_t i = 0; i < split.count(); i++)
        split[i] = indent + split[i] + "\n";

    for(size_t i = 0; i < split.count(); i++)
        ret_str += split[i];

    return ret_str;
}



const char* Str::to_c_str()
{
    // make sure null-termination fits
    if( size() == capacity() )
    {
        reallocate(size() + 1);
    }

    *(mem+size_str) = 0x0;
    return data();
}



void Str::print() const
{
    Print::ln(*this);
}

void Str::print_in_quotes()
{
    Print::buf("\"");
    Print::buf(*this);
    Print::buf("\"\n");
}




Str Str::
CH(char ch)
{
    Str str = " ";
    str[0] = ch;
    return str;
}


Str Str::
SI(long long s_int)
{
    char buff[30];
    memset(buff, 0x0, 30);
    snprintf(buff, sizeof(buff), "%lli", s_int);
    return Str{buff};

    // Old tmp code to hold me over

    if(s_int == 0)
        return "0";

    size_t max_chars_int = 10;

    // std::vector<char> chars_;
    char chars[max_chars_int + 1];

    int divisor = 1000000000;

    for(size_t i = 0; i < max_chars_int; i++){
        // Extract currently largest digit
        int res =  s_int / divisor;
        // remove that digit
        s_int -= divisor * res;
        // append as ascii
        chars[i] = res + 0x30;
        // Prepare for next digit
        divisor /= 10;
    }
    chars[max_chars_int] = 0x0;

    Str new_str = Str((const char*)chars);

    size_t leading_zero_count = 0;
    for (size_t i = 0; i < max_chars_int; i++)
    {
        if(chars[i] != '0')
            break;
        leading_zero_count++;
    }

    new_str.cut_to_substr(leading_zero_count, new_str.size());

    return new_str;
}

Str Str::UI(unsigned long long u_int)
{
    char buff[30];
    memset(buff, 0x0, 30);
    snprintf(buff, sizeof(buff), "%lli", u_int);
    return Str{buff};


    if(u_int == 0)
        return "0";

    size_t max_chars_int = 10;

    // std::vector<char> chars_;
    char chars[max_chars_int + 1];

    int divisor = 1000000000;

    for(size_t i = 0; i < max_chars_int; i++){
        // Extract currently largest digit
        int res =  u_int / divisor;
        // remove that digit
        u_int -= divisor * res;
        // append as ascii
        chars[i] = res + 0x30;
        // Prepare for next digit
        divisor /= 10;
    }
    chars[max_chars_int] = 0x0;

    Str new_str = Str((const char*)chars);

    size_t leading_zero_count = 0;
    for (size_t i = 0; i < max_chars_int; i++)
    {
        if(chars[i] != '0')
            break;
        leading_zero_count++;
    }

    new_str.cut_to_substr(leading_zero_count, new_str.size());

    return new_str;
}



Str Str::
FL(float fl, uchar decimals, Str::FloatRep float_representation)
{
    // Get actual number of decimal points
    // max 2^3
    unsigned char decimal_max8 = decimals << 5;
    decimal_max8 = decimal_max8 >> 5;
    char decimals_char = decimal_max8 + 0x30;
    
    char format[5];
    format[0] = '%';
    format[1] = '.';
    format[2] = decimals_char;
    if(float_representation == Str::FloatRep::Fixed)
        format[3] = 'f';
    if(float_representation == Str::FloatRep::Scientific)
        format[3] = 'e';
    format[4] = 0x0;

    char buffer[32];
    memset(buffer, 0x0, 32);
    snprintf(buffer, sizeof(buffer), format, fl);  // 6 digits after decimal

    return Str{buffer};
}

