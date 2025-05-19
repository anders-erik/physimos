#include <iostream>

#include <cstring>
#include <cstdlib>

struct str8 {

    bool allocated = false; /** memory allocated */
    bool initialized = false; /** Allocated memory initialized */
    size_t size;
    char* mem;


    str8(int size) {
        this->size = size;
        mem = (char*) std::malloc(size * sizeof(char));
        allocated = true;
    };
    str8(int size, char initialization_value) {
        this->size = size;
        mem = (char*) std::malloc(size * sizeof(char));
        allocated = true;
        memset(mem, initialization_value, size);
        initialized = true;
    };
    str8(const char *c_str) {
        size = std::strlen(c_str);
        mem = (char*) std::malloc(size * sizeof(char));
        allocated = true;
        memcpy(mem, c_str, size);
        initialized = true;
    }

    ~str8(){
        std::cout << "str8 destructor" << std::endl;
        free(mem);
    }


    char operator[](size_t index){
        if(index > size-1)
            throw std::runtime_error("str8: index access out of bounds.");

        return *(mem+index);
    }

    void print(){
        
        if(!initialized)
            throw std::runtime_error("Tried to print uninitialized str8");
        
        for(size_t i = 0; i < size; i++)
            std::cout <<  (*this)[i];

        std::cout << std::flush;
    }
    void println(){
        
        if(!initialized)
            throw std::runtime_error("Tried to print uninitialized str8");
        
        for(size_t i = 0; i < size; i++)
            std::cout <<  (*this)[i];

        std::cout << '\n' << std::flush;
    }

};

int main(){
    std::cout << "str8 main." << std::endl << std::endl;

    str8 str_10 (10, 'c');
    str_10.print();
    str_10.println();
    // str_10.~str8(); // double free

    str8 str_c ("hola");
    str_c.println();
    // str_c.~str8(); // double free
    // str_c[1];

    return 0;
}