
#include "str.hh"

// #define VERBOSE_STR
// #define NEW_ALLOCATION


Str::Str() {
#ifdef VERBOSE_STR
    std::cout << "Str default constructor" << std::endl;
#endif
};
Str::Str(unsigned int string_size) {
#ifdef VERBOSE_STR
    std::cout << "Str size constructor" << std::endl;
#endif
    
    this->size_str = string_size;
    allocate(this->size_str);
};

Str::Str(unsigned int string_size, char initialization_value) {
#ifdef VERBOSE_STR
    std::cout << "Str initialization_value constructor" << std::endl;
#endif
    
    this->size_str = string_size;
    
    allocate(this->size_str);
    
    initalize(initialization_value);
};
Str::Str(const char *c_str) {
#ifdef VERBOSE_STR
    std::cout << "Str c_str constructor" << std::endl;
#endif
    
    
    size_str = std::strlen(c_str);

    allocate(this->size_str);
    
    initalize(c_str);
    
}


Str::Str(const Str& other) //= delete; 
    :   size_str {other.size()}
{
#ifdef VERBOSE_STR
    std::cout << "Str Copy Constructor" << std::endl;
#endif
    
    // Allocate same size
    if(!other.is_allocated())
        return;
    size_str = other.size();
    allocate(size_str);

    // Initialize with same memory contents
    if(!other.is_initialized())
        return;
    const char* other_mem = other.get_pointer();
    initalize(other_mem);

};

Str::Str(Str&& other) {
#ifdef VERBOSE_STR
    std::cout << "Str move constructor" << std::endl;
#endif
    
    mem = other.mem;
    size_str = other.size_str;
    allocated = other.allocated;
    initialized = other.initialized;
    other.rob();
}



Str::~Str(){
#ifdef VERBOSE_STR
    std::cout << "Str destructor with content = \"" << to_std_string() << "\"" << std::endl;
#endif
    release_mem();
}




char Str::operator[](size_t index){
    if(index > size_str-1)
        throw std::runtime_error("Str: index access out of bounds.");

    return *(mem+index);
}

Str& Str::operator+=(Str&& rhs){
    this->append(rhs);
    return *this;
}

Str& Str::operator+=(Str & rhs){
    this->append(rhs);
    return *this;
}

Str Str::operator+(Str&& rhs){
    Str new_str = *this;
    new_str.append(rhs);
    return new_str;
}

Str Str::operator+(Str& rhs){
    Str new_str = *this;
    new_str.append(rhs);
    return new_str;
}


Str& Str::operator=(const Str& other) // = delete; // {
{
#ifdef VERBOSE_STR
    std::cout << "Str copy assignment" << std::endl;
#endif

    if (this == &other)
        std::cout << "this != &other" << std::endl;
        
    if(!other.is_allocated())
        return *this;
    size_str = other.size();
    allocate(size_str);

    if(!other.is_initialized())
        return *this;
    const char* others_mem = other.get_pointer();
    initalize(others_mem);

    return *this;
}
Str& Str::operator=(Str&& other){
#ifdef VERBOSE_STR
    std::cout << "Str move assignment op" << std::endl;
#endif

    if (this == &other)
        std::cout << "this != &other" << std::endl;
    
    mem = other.mem;
    size_str = other.size_str;
    size_alloc = other.size_alloc;
    allocated = other.allocated;
    initialized = other.initialized;

    other.rob();

    return *this;
};


void Str::allocate(unsigned int size_to_alloc){
    size_alloc = size_to_alloc;

#ifdef NEW_ALLOCATION
    mem = static_cast<char*>(::operator new(size_alloc));
#else
    mem = (char*) std::malloc(size_to_alloc * sizeof(char));
#endif

    allocated = true;
}
void Str::reallocate(unsigned int new_size_alloc){
    

#ifdef NEW_ALLOCATION
    char* new_mem = static_cast<char*>(::operator new(size_alloc));
    memcpy(new_mem, mem, size_alloc);
    delete[] mem;
    mem = new_mem;
#else
    mem = (char*) std::realloc(mem , new_size_alloc * sizeof(char));
#endif

    size_alloc = new_size_alloc;

#ifdef VERBOSE_STR
    std::cout << "Realloc to size " << size_alloc << std::endl;
#endif
}
void Str::release_mem(){
    if(allocated){
#ifdef NEW_ALLOCATION
        delete[] mem;
#else
        free(mem);
#endif
    }
    size_alloc = 0;
    size_str = 0;
    allocated = false;
    initialized = false;
}
// Ownership moved. Robbed of all assets. No mem release.
void Str::rob(){
    mem = nullptr;
    size_alloc = 0;
    size_str = 0;
    allocated = false;
    initialized = false;
}
void Str::initalize(const char* new_mem_value){
    memcpy(mem, new_mem_value, size_str);
    initialized = true;
}
void Str::initalize(const char init_char){
    memset(mem, init_char, size_str);
    initialized = true;
}

char* Str::get_pointer() const {
    return mem;
}
unsigned int Str::size() const {
    return size_str;
}

unsigned int Str::alloc_size() const {
    return size_alloc;
}

bool Str::is_allocated() const {
    return allocated;
}
bool Str::is_initialized() const {
    return initialized;
}


void Str::append(const Str& str_to_append){

    unsigned int combined_str_size = size_str + str_to_append.size();

    if(size_alloc < combined_str_size )
        reallocate(combined_str_size);
    
    memcpy(mem+size_str, str_to_append.get_pointer(), str_to_append.size());
    
    size_str = combined_str_size;
}

Str& Str::cut_to_substr(unsigned int start_index, unsigned int size){

    if(start_index > size_str || start_index+size > size_str){
        std::cout << "Error: unable to cut Str from substring. Substring beyond string size. " << std::endl;
        return *this;
    }
        
    memmove(mem, mem+start_index, size);
    reallocate(size);
    size_str = size;

    return *this;
}
Str Str::substr(unsigned int start_index, unsigned int size){
    Str copy = *this;
    return copy.cut_to_substr(start_index, size);
}



std::string Str::to_std_string(){

    // Ok to ignore null-termination??
    // std::string std_str = std::string(mem);

    std::string std_str;

    if(mem == nullptr){
        std::cout << "WARN: returning std string with mem == nullptr" << std::endl;
        std_str = "";
    }
    else {
        std_str = std::string(mem, size_str); // specify the size of current Str allocation
    }

    return std_str;
}
/** Reference-like string without copying. */
std::string_view Str::to_std_string_view(){

    std::string_view view = std::string_view(mem, size_str);

    return view;
}
const char* Str::to_c_str(){

    // make sure null-termination fits
    if(size_alloc <= size_str )
        reallocate(size_str+1);

    *(mem+size_str) = 0x0; 
    return mem;
}

void Str::print(){
    
    if(!initialized)
        throw std::runtime_error("Tried to print uninitialized Str");
    
    for(size_t i = 0; i < size_str; i++)
        std::cout <<  (*this)[i];

    std::cout << std::flush;
}
void Str::println(){
    
    if(!initialized)
        throw std::runtime_error("Tried to print uninitialized Str");
    
    for(size_t i = 0; i < size_str; i++)
        std::cout <<  (*this)[i];

    std::cout << '\n' << std::flush;
}
void Str::println_quotes(){
    std::cout << '"';
    
    for(size_t i = 0; i < size_str; i++)
        std::cout <<  (*this)[i];

    std::cout << '"' << '\n' << std::flush;
}

void Str::busy() {}