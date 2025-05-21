
#pragma once

#include <iostream>
#include <string>

#include <cstring>


struct Str {

    bool allocated = false; /** memory allocated */
    bool initialized = false; /** Allocated memory initialized */
    size_t size;
    char* mem;

    void deown(){
        allocated = false;
        initialized = false;
    }

    Str(const Str& other) = delete;
    //     std::cout << "Copy Constructor" << std::endl;
    //     // other.deown();
    // };
    Str& operator=(const Str& other) = delete; // {
    //     other.deown();
    //     return *this;
    // };
    Str(Str&& other) {
        std::cout << "MOVE" << std::endl;
        
        mem = other.mem;
        size = other.size;
        allocated = other.allocated;
        initialized = other.initialized;
        other.deown();
    }
    Str& operator=(Str&& other){

        if (this == &other)
            std::cout << "this != &other" << std::endl;
        
        mem = other.mem;
        size = other.size;
        allocated = other.allocated;
        initialized = other.initialized;

        other.deown();

        return *this;
    };

    Str() {
        std::cout << "Str default constructor" << std::endl;
    };
    Str(int size) {
        std::cout << "Str size constructor" << std::endl;
        this->size = size;
        mem = (char*) std::malloc(size * sizeof(char));
        allocated = true;
    };
    Str(int size, char initialization_value) {
        std::cout << "Str initialization_value constructor" << std::endl;
        this->size = size;
        mem = (char*) std::malloc(size * sizeof(char));
        allocated = true;
        memset(mem, initialization_value, size);
        initialized = true;
    };
    Str(const char *c_str) {
        std::cout << "Str c_str constructor" << std::endl;
        size = std::strlen(c_str);
        mem = (char*) std::malloc(size * sizeof(char));
        allocated = true;
        memcpy(mem, c_str, size);
        initialized = true;
    }

    ~Str(){
        std::cout << "Str destructor with content = \"" << to_std_string() << "\"" << std::endl;
        if(allocated)
            free(mem);
        allocated = false;
        initialized = false;
    }


    char operator[](size_t index){
        if(index > size-1)
            throw std::runtime_error("Str: index access out of bounds.");

        return *(mem+index);
    }

    std::string to_std_string(){

        // Ok to ignore null-termination??
        // std::string std_str = std::string(mem);
        
        // specify the size of current Str allocation
        std::string std_str_size = std::string(mem, size);

        return std_str_size;
    }
    const char* to_c_str(){
        return mem;
    }

    void print(){
        
        if(!initialized)
            throw std::runtime_error("Tried to print uninitialized Str");
        
        for(size_t i = 0; i < size; i++)
            std::cout <<  (*this)[i];

        std::cout << std::flush;
    }
    void println(){
        
        if(!initialized)
            throw std::runtime_error("Tried to print uninitialized Str");
        
        for(size_t i = 0; i < size; i++)
            std::cout <<  (*this)[i];

        std::cout << '\n' << std::flush;
    }

};

