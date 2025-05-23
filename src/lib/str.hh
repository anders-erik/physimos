
#pragma once

#include <iostream>
#include <string>

#include <cstring>
#include <new>



struct Str {

    // NOT USED
    struct StrStatus {
        bool allocated = false; 
        bool initialized = false;
    } status;

    bool allocated = false; /** memory allocated */
    bool initialized = false; /** Allocated memory initialized */
    size_t size = 0;
    char* mem = nullptr;

    Str(const Str& other) //= delete; 
        :   size {other.get_size()}
    {
        std::cout << "Copy Constructor" << std::endl;
        
        // Allocate same size
        if(!other.is_allocated())
            return;
        size = other.get_size();
        allocate();

        // Initialize with same memory contents
        if(!other.is_initialized())
            return;
        const char* other_mem = other.get_pointer();
        initalize(other_mem);

    };

    Str& operator=(const Str& other) // = delete; // {
    {
        if (this == &other)
            std::cout << "this != &other" << std::endl;
            
        if(!other.is_allocated())
            return *this;
        size = other.get_size();
        allocate();

        if(!other.is_initialized())
            return *this;
        const char* others_mem = other.get_pointer();
        initalize(others_mem);

        return *this;
    }
    Str(Str&& other) {
        std::cout << "MOVE CONSTRUCTOR" << std::endl;
        
        mem = other.mem;
        size = other.size;
        allocated = other.allocated;
        initialized = other.initialized;
        other.rob();
    }
    Str& operator=(Str&& other){
        std::cout << "MOVE ASSIGNMENT OP" << std::endl;

        if (this == &other)
            std::cout << "this != &other" << std::endl;
        
        mem = other.mem;
        size = other.size;
        allocated = other.allocated;
        initialized = other.initialized;

        other.rob();

        return *this;
    };

    Str() {
        std::cout << "Str default constructor" << std::endl;
    };
    Str(int size) {
        std::cout << "Str size constructor" << std::endl;
        this->size = size;
        allocate();
    };
    Str(int size, char initialization_value) {
        std::cout << "Str initialization_value constructor" << std::endl;
        
        this->size = size;
        
        allocate();
        
        initalize(initialization_value);
    };
    Str(const char *c_str) {
        std::cout << "Str c_str constructor" << std::endl;
        
        size = std::strlen(c_str);

        allocate();
        
        initalize(c_str);
        
    }

    ~Str(){
        std::cout << "Str destructor with content = \"" << to_std_string() << "\"" << std::endl;
        release_mem();
    }


    char operator[](size_t index){
        if(index > size-1)
            throw std::runtime_error("Str: index access out of bounds.");

        return *(mem+index);
    }

    void allocate(){
        mem = (char*) std::malloc(size * sizeof(char));
        allocated = true;
    }
    void release_mem(){
        if(allocated)
            free(mem);
        allocated = false;
        initialized = false;
    }
    // Ownership moved. Robbed of all assets. No mem release.
    void rob(){
        mem = nullptr;
        allocated = false;
        initialized = false;
    }
    void initalize(const char* new_mem_value){
        memcpy(mem, new_mem_value, size);
        initialized = true;
    }
    void initalize(const char init_char){
        memset(mem, init_char, size);
        initialized = true;
    }

    char* get_pointer() const {
        return mem;
    }
    size_t get_size() const {
        return size;
    }
    StrStatus get_status() const {
        return status;
    }
    bool is_allocated() const {
        return allocated;
    }
    bool is_initialized() const {
        return initialized;
    }

    std::string to_std_string(){

        // Ok to ignore null-termination??
        // std::string std_str = std::string(mem);

        std::string std_str_size;

        if(mem == nullptr)
            std_str_size = "";
        else
            std_str_size = std::string(mem, size); // specify the size of current Str allocation
        

        return std_str_size;
    }
    /** Reference-like string without copying. */
    std::string_view to_std_string_view(){

        std::string_view view = std::string_view(mem, size);

        return view;
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

    void busy() {}
};

