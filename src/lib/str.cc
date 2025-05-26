
#include "str.hh"



Str::Str() {
    std::cout << "Str default constructor" << std::endl;
};
Str::Str(unsigned int string_size) {
    std::cout << "Str size constructor" << std::endl;
    this->size_str = string_size;
    allocate(this->size_str);
};
Str::Str(unsigned int string_size, char initialization_value) {
    std::cout << "Str initialization_value constructor" << std::endl;
    
    this->size_str = string_size;
    
    allocate(this->size_str);
    
    initalize(initialization_value);
};
Str::Str(const char *c_str) {
    std::cout << "Str c_str constructor" << std::endl;
    
    size_str = std::strlen(c_str);

    allocate(this->size_str);
    
    initalize(c_str);
    
}


Str::Str(const Str& other) //= delete; 
    :   size_str {other.size()}
{
    std::cout << "Str Copy Constructor" << std::endl;
    
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
    std::cout << "Str move constructor" << std::endl;
    
    mem = other.mem;
    size_str = other.size_str;
    allocated = other.allocated;
    initialized = other.initialized;
    other.rob();
}



Str::~Str(){
    std::cout << "Str destructor with content = \"" << to_std_string() << "\"" << std::endl;
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


Str& Str::operator=(const Str& other) // = delete; // {
{
    std::cout << "Str copy assignment" << std::endl;

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
    std::cout << "Str move assignment op" << std::endl;

    if (this == &other)
        std::cout << "this != &other" << std::endl;
    
    mem = other.mem;
    size_str = other.size_str;
    allocated = other.allocated;
    initialized = other.initialized;

    other.rob();

    return *this;
};


void Str::allocate(unsigned int size_to_alloc){
    mem = (char*) std::malloc(size_to_alloc * sizeof(char));
    size_alloc = size_to_alloc;
    allocated = true;
}
void Str::reallocate(unsigned int size_expansion){
    
    size_alloc = size_expansion;

    mem = (char*) std::realloc(mem , size_alloc * sizeof(char));
    std::cout << "Realloc to size " << size_alloc << std::endl;
}
void Str::release_mem(){
    if(allocated)
        free(mem);
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



std::string Str::to_std_string(){

    // Ok to ignore null-termination??
    // std::string std_str = std::string(mem);

    if(mem == nullptr)
        std::cout << "WARN: returning std string with mem == nullptr" << std::endl;
        

    std::string std_str_size;

    if(mem == nullptr)
        std_str_size = "";
    else
        std_str_size = std::string(mem, size_str); // specify the size of current Str allocation
    

    return std_str_size;
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