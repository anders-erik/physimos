
#include <new>  // new
#include <cstring> // memcpy, 
#include <string> // malloc

#include "alloc.hh"


// #define ALLOCATE_WITH_NEW


namespace Alloc {


char* allocate_bytes(unsigned int byte_count){

    #ifdef ALLOCATE_WITH_NEW
        return static_cast<char*>(::operator new(byte_count));
    #else
        return (char*) std::malloc(byte_count * sizeof(char));
    #endif
    
}

char* reallocate_bytes(char * current_pointer, unsigned int current_size, unsigned int new_size){


    #ifdef ALLOCATE_WITH_NEW

        char* new_mem = static_cast<char*>(::operator new(new_size));

        if(new_size > current_size)
            memcpy(new_mem, current_pointer, current_size); // Copy all current data
        else
            memcpy(new_mem, current_pointer, new_size); // Copy only new size

        delete[] current_pointer;
        return new_mem;

    #else

        return (char*) std::realloc(current_pointer, new_size * sizeof(char));

    #endif

}


void deallocate_bytes(char * bytes){

    #ifdef ALLOCATE_WITH_NEW
        delete[] bytes;
    #else
            free(bytes);
    #endif

}

};