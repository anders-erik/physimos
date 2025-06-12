
#pragma once

#include <iostream>
#include <string>

#include <cstring>
#include <new>

struct Str;

/** Get single character as string */
Str to_str(char ch);
/** Integer to Str */
Str int_to_str(int integer);


struct Str {

    // bool allocated = false; /** memory allocated */
    // bool initialized = false; /** Allocated memory initialized */
    // size_t size = 0;
    unsigned int size_alloc = 0;
    unsigned int size_str = 0;
    char* mem = nullptr;

    Str();
    // Str(char * str_mem, unsigned int string_size);
    Str(unsigned int string_size, char initialization_value);
    Str(const char *c_str);
    
    /** Create string representation of integer. E.g. 123 -> "123" */
    Str(int integer);
    /** String representatin of float. Max decimal value is 8. */
    Str(float _float, unsigned char decimals);

    Str(const Str& other);
    Str(Str&& other);

    ~Str();


    Str& operator+=(Str&& rhs);
    Str& operator+=(Str& rhs);
    Str& operator+=(const char* c_str);
    Str operator+(Str&& rhs);
    Str operator+(Str& rhs);
    Str operator+(const char* c_str);

    Str& operator=(const Str& other);
    Str& operator=(Str&& other);

    bool operator==(const Str& other) const;

    char& operator[](size_t index); // mutable char
    const char operator[](size_t index) const; // read only



    char* data() const;
    unsigned int size() const;
    unsigned int capacity() const;
    bool has_size() const;
    bool has_capacity() const;

    void allocate(unsigned int size_to_alloc);
    void reallocate(unsigned int new_size_alloc);
    void release_mem();
    // Ownership moved. Robbed of all assets. No mem release.
    void rob();


    /** Append chars to first string. realloc if necessary */
    void append(const Str& str_to_append);
    /** Shrink the string to specified starting index and size. Returns itself in cut state. */
    Str& cut_to_substr(unsigned int pos, unsigned int new_size);
    /** Returns new Str as specified substring. */
    Str substr(unsigned int pos, unsigned int new_size);

    std::string to_std_string();
    /** Reference-like string without copying. */
    std::string_view to_std_string_view();
    const char* to_c_str();

    /** Underscape to prevent name-clash with global print function. */
    void print_();
    void print_line() const;
    void print_line_quotes();

    void busy();

    static Str to_str_char(char ch);
    static Str to_str_int(int integer);
    static Str to_str_float(float fl, int decimals);
};

