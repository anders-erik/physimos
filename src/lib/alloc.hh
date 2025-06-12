
#pragma once


namespace Alloc {


char* allocate_bytes(unsigned int byte_count);

char* reallocate_bytes(char* old_pointer, unsigned int old_size, unsigned int new_size);

void  deallocate_bytes(char* byte_pointer);


};