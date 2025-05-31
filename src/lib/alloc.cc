
#include <new>  // new
#include <cstring> // memcpy, 
#include <string> // malloc

#include "alloc.hh"


#define ALLOCATE_WITH_NEW


namespace Alloc {


char* allocate_bytes(unsigned int byte_count){

    #ifdef ALLOCATE_WITH_NEW
        return static_cast<char*>(::operator new(byte_count));
    #else
        return (char*) std::malloc(byte_count * sizeof(char));
    #endif
    
}

char* reallocate_bytes(char * old_pointer, unsigned int old_size, unsigned int new_size){

    #ifdef ALLOCATE_WITH_NEW
        char* new_mem = static_cast<char*>(::operator new(new_size));
        memcpy(new_mem, old_pointer, old_size);
        delete[] old_pointer;
        return new_mem;
    #else
        return (char*) std::realloc(old_pointer, new_size * sizeof(char));
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