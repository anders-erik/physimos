
#pragma once

#include <iostream>
#include <string>

#include <cstring>
#include <new>



struct Str {

    bool allocated = false; /** memory allocated */
    bool initialized = false; /** Allocated memory initialized */
    // size_t size = 0;
    unsigned int size_alloc = 0;
    unsigned int size_str = 0;
    char* mem = nullptr;

    Str();
    Str(unsigned int string_size);
    // Str(char * str_mem, unsigned int string_size);
    Str(unsigned int string_size, char initialization_value);
    Str(const char *c_str);

    Str(const Str& other);
    Str(Str&& other);

    ~Str();

    char operator[](size_t index);

    Str& operator+=(Str&& rhs);
    Str& operator+=(Str& rhs);
    Str operator+(Str&& rhs);
    Str operator+(Str& rhs);

    Str& operator=(const Str& other);
    Str& operator=(Str&& other);


    unsigned int size() const;
    unsigned int alloc_size() const;
    bool is_allocated() const;
    bool is_initialized() const;

    void allocate(unsigned int size_to_alloc);
    void reallocate(unsigned int new_size_alloc);
    void release_mem();
    // Ownership moved. Robbed of all assets. No mem release.
    void rob();
    void initalize(const char* new_mem_value);
    void initalize(const char init_char);

    char* get_pointer() const;

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


    // void print();
    void println();
    void println_quotes();

    void busy();
};

