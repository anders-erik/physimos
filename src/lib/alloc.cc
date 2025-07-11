
#include <new>  // new
#include <cstring> // memcpy, 
#include <string> // malloc

#include "alloc.hh"


// #define ALLOCATE_WITH_NEW



char* Alloc::
allocate_bytes(unsigned int byte_count)
{
    return (char*) std::malloc(byte_count * sizeof(char));
}

char* Alloc::
reallocate_bytes(char * current_pointer, unsigned int current_size, unsigned int new_size)
{
    return (char*) std::realloc(current_pointer, new_size * sizeof(char));
}

void Alloc::
deallocate_bytes(char* bytes)
{
    std::free(bytes);
}



char * Alloc::
allocate_bytes_new(unsigned int byte_count)
{
    return static_cast<char*>(::operator new(byte_count));
}

char * Alloc::
reallocate_bytes_new(char * current_pointer, unsigned int current_size, unsigned int new_size)
{
    if(current_size == new_size)
        return current_pointer;
    
    char* new_mem = static_cast<char*>(::operator new(new_size));

    if(new_size > current_size)
        memcpy(new_mem, current_pointer, current_size); // Copy all current data
    else
        memcpy(new_mem, current_pointer, new_size); // Copy only new size

    delete[] current_pointer;
    return new_mem;
}

void Alloc::
deallocate_bytes_new(char * byte_pointer)
{
    delete[] byte_pointer;
}



