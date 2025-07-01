
#pragma once


namespace Alloc {


char* allocate_bytes(   unsigned int byte_count );

char* reallocate_bytes( char* old_pointer, 
                        unsigned int old_size, 
                        unsigned int new_size   );

void  deallocate_bytes( char* byte_pointer      );


char* allocate_bytes_new(   unsigned int byte_count );

char* reallocate_bytes_new( char* old_pointer, 
                            unsigned int old_size, 
                            unsigned int new_size   );

void  deallocate_bytes_new( char* byte_pointer      );


// char* allocate_type(   unsigned int obj_count );

// char* reallocate_type( char* old_pointer, 
//                         unsigned int old_size, 
//                         unsigned int new_size   );
                        
// void  deallocate_type( char* byte_pointer      );

template <typename T>
T*  allocate_type(uint objects_to_allocate)
{
    
}


};