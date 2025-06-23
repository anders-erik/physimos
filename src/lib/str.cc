
#include <stdexcept>
#include <vector>

#include <cstdio>  // snprintf - float constructor

#include "str.hh"
#include "alloc.hh"
#include "print.hh"


// #define VERBOSE_STR // Prints string info throughout its lifetime




Str::Str() 
{
#ifdef VERBOSE_STR
    // println("Str default constructor"); // Can't use because member function clashing
    Print::line("Str default constructor");
    
    std::cout << "Str default constructor" << std::endl;
#endif
};


Str::
Str(int integer) 
{
#ifdef VERBOSE_STR
    std::cout << "Integer constructor" << std::endl;
#endif

    *this = std::move(Str::to_str_int(integer));
}


Str::
Str(float _float, unsigned char decimals)
{
    *this = std::move(Str::to_str_float(_float, decimals));
}


Str::
Str(unsigned int string_size, char initialization_value) 
{
#ifdef VERBOSE_STR
    std::cout << "Str initialization_value constructor" << std::endl;
#endif
    
    this->size_str = string_size;
    
    allocate(size());
    memset(mem, initialization_value, size());
};


Str::
Str(const char *c_str) 
{
#ifdef VERBOSE_STR
    std::cout << "Str c_str constructor" << std::endl;
#endif

    size_str = std::strlen(c_str);

    allocate(size());
    memcpy(mem, c_str, size());
}


Str::
Str(const Str& other) 
{
#ifdef VERBOSE_STR
    std::cout << "Str Copy Constructor" << std::endl;
#endif

    allocate(other.capacity());
    memcpy(mem, other.data(), other.size());
    size_str = other.size();
};


Str::
Str(Str&& other) 
{
#ifdef VERBOSE_STR
    std::cout << "Str move constructor" << std::endl;
#endif
    
    mem = other.data();
    size_alloc = other.capacity();
    size_str = other.size();
    other.rob();
}



Str::
~Str()
{
#ifdef VERBOSE_STR
    std::cout << "Str destructor with content = \"" << to_std_string() << "\"" << std::endl;
#endif
    release_mem();
}



char& Str::
operator[](size_t index)
{
    return *(mem+index);
}

const char Str::
operator[](size_t index) const
{
    if(index > size_str-1)
        throw std::runtime_error("Str: index access out of bounds.");

    return *(mem+index);
}



Str& Str::
operator+=(Str&& rhs)
{
    this->append(rhs);
    return *this;
}


Str& Str::
operator+=(Str & rhs)
{
    this->append(rhs);
    return *this;
}


Str& Str::
operator+=(const char * c_str)
{
    Str rhs {c_str};
    this->append(rhs);
    return *this;
}


Str Str::
operator+(Str&& rhs)
{
    Str new_str = *this;
    new_str.append(rhs);
    return new_str;
}


Str Str::
operator+(Str& rhs)
{
    Str new_str = *this;
    new_str.append(rhs);
    return new_str;
}


Str Str::
operator+(const char * c_str)
{
    Str new_str = *this;
    Str rhs = Str(c_str);
    new_str.append(rhs);
    return new_str;
}


Str& Str::
operator=(const Str& other)
{
#ifdef VERBOSE_STR
    std::cout << "Str copy assignment" << std::endl;
#endif

    allocate(other.capacity());
    memcpy(mem, other.data(), other.size());
    size_str = other.size();

    return *this;
}


Str& Str::
operator=(Str&& other)
{
#ifdef VERBOSE_STR
    std::cout << "Str move assignment op" << std::endl;
#endif

    if (this == &other)
        std::cout << "this != &other" << std::endl;
    
    mem = other.mem;
    size_str = other.size_str;
    size_alloc = other.size_alloc;
    // allocated = other.allocated;
    // initialized = other.initialized;

    other.rob();

    return *this;
}

bool Str::operator==(const Str & other) const
{
    if(this->size() != other.size())
        return false;

    for(size_t i = 0; i < this->size(); i++)
    {
        if(this->operator[](i) != other[i])
            return false;
    }

    return true;
}



void Str::allocate(unsigned int size_to_alloc)
{
    size_alloc = size_to_alloc;

    mem = Alloc::allocate_bytes(size_to_alloc);
}


void Str::reallocate(unsigned int new_size_alloc){

    // No need to reallocate
    if(size_alloc == new_size_alloc)
        return;
        // throw std::runtime_error("Tried to reallocate Str to current size.");

    mem = Alloc::reallocate_bytes(mem, size_alloc, new_size_alloc);

    size_alloc = new_size_alloc;

#ifdef VERBOSE_STR
    std::cout << "Realloc to size " << size_alloc << std::endl; 
#endif

}


void Str::release_mem()
{
    if(has_capacity())
    {
        Alloc::deallocate_bytes(mem);
    }

    mem = nullptr;
    size_alloc = 0;
    size_str = 0;
}

void Str::rob(){
    mem = nullptr;
    size_alloc = 0;
    size_str = 0;
    // allocated = false;
    // initialized = false;
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
Str Str::substr(unsigned int pos, unsigned int new_size){
    Str copy = *this;
    return copy.cut_to_substr(pos, new_size);
}



std::string Str::to_std_string(){


    if(!has_size()){
        std::cout << "WARN: returning std string with mem == nullptr" << std::endl;
        return std::string();
    }

    // specify the size of current Str allocation
    return std::string(mem, size_str); 

}

/** Reference-like string without copying. */
std::string_view Str::to_std_string_view(){

    std::string_view view = std::string_view(mem, size_str);

    return view;
}
const char* Str::to_c_str(){

    // make sure null-termination fits
    if( size() == capacity() )
    {
        reallocate(size() + 1);
    }

    *(mem+size_str) = 0x0;
    return data();
}

void Str::print_()
{
    print(*this);
}
void Str::print_line() const
{
    println(*this);
}

void Str::print_line_quotes()
{
    print("\"");
    print(*this);
    println("\"");
}

void Str::busy() {}




Str 
to_str_char(char ch)
{
    Str str = " ";
    return str[0] = ch;
}


Str Str::
to_str_int(int integer)
{
    if(integer == 0)
        return "0";

    size_t max_chars_int = 10;

    // std::vector<char> chars_;
    char chars[max_chars_int + 1];

    int divisor = 1000000000;

    for(size_t i = 0; i < max_chars_int; i++){
        // Extract currently largest digit
        int res =  integer / divisor;
        // remove that digit
        integer -= divisor * res;
        // append as ascii
        chars[i] = res + 0x30;
        // Prepare for next digit
        divisor /= 10;
    }
    chars[max_chars_int] = 0x0;

    Str new_str = chars;

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
to_str_float(float fl, int decimals)
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
    format[3] = 'f';
    format[4] = 0x0;

    char buffer[32];
    memset(buffer, 0x0, 32);
    snprintf(buffer, sizeof(buffer), format, fl);  // 6 digits after decimal

    return Str{buffer};
}

