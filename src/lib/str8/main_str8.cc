#include <vector>
#include <iostream>

#include <string>

#include <cstring>
#include <cstdlib>


struct str8 {

    bool allocated = false; /** memory allocated */
    bool initialized = false; /** Allocated memory initialized */
    size_t size;
    char* mem;

    str8() {
        std::cout << "str8 default constructor" << std::endl;
    };
    str8(int size) {
        std::cout << "str8 size constructor" << std::endl;
        this->size = size;
        mem = (char*) std::malloc(size * sizeof(char));
        allocated = true;
    };
    str8(int size, char initialization_value) {
        std::cout << "str8 initialization_value constructor" << std::endl;
        this->size = size;
        mem = (char*) std::malloc(size * sizeof(char));
        allocated = true;
        memset(mem, initialization_value, size);
        initialized = true;
    };
    str8(const char *c_str) {
        std::cout << "str8 c_str constructor" << std::endl;
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

    std::string to_std_string(){

        // Ok to ignore null-termination??
        // std::string std_str = std::string(mem);
        
        // specify the size of current str8 allocation
        std::string std_str_size = std::string(mem, size);

        return std_str_size;
    }
    const char* to_c_str(){
        return mem;
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



template <typename T>
struct MyAllocator {
    using value_type = T;

    MyAllocator() = default;

    template <typename U>
    MyAllocator(const MyAllocator<U>&) {}

    T* allocate(std::size_t n) {
        std::cout << "Allocating " << n << " objects\n";
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t n) {
        std::cout << "Deallocating " << n << " objects\n";
        ::operator delete(p);
    }
};

int main(){
    std::cout << "str8 main." << std::endl << std::endl;

    {
    str8 str_10 (10, 'c');
    str_10.print();
    str_10.println();
    // str_10.~str8(); // double free

    str8 str_c ("hola");
    str_c.println();
    // str_c.~str8(); // double free
    // str_c[1];

    std::vector<str8> strings;
    
    // strings.push_back(str_10);
    // strings.pop_back();
    // strings.pop_back();
    // strings.pop_back();

    }


    {
    std::cout << std::endl << "New" << std::endl;
    
    void* n = ::operator new (sizeof(int));

    void* nn = new str8;                            // calls constructor
    void* nnn = ::operator new (sizeof(str8));      // no constructor : only uses size of struct
    str8* new_str8 = static_cast<str8*>( new str8 );// calls constructor

    // str8* ni = (str8*) n;
    // *ni = 10;
    
    // std::cout << ni << std::endl;
    // ni->println();
    // std::cout << *n << std::endl;
    std::cout << "end new" << std::endl << std::endl;
    }

    std::cout << "STD: " << std::endl;
    
    // string literal ok
    str8 str_to_std_c = "sdafljadas"; // c_str constructor
    std::string std_str_c = str_to_std_c.to_std_string();
    std::cout << std_str_c << std::endl;

    // not null-terminated
    str8 str_to_std_a (10, 'a');
    std::string std_str_a = str_to_std_a.to_std_string();
    std::cout << std_str_a << std::endl;
    // print retrieved 'c_str'
    std::cout << str_to_std_a.to_c_str() << std::endl;

    std::cout << "STD end: " << std::endl << std::endl;
    

    std::cout << "End str8" << std::endl << std::endl;
    

    return 0;
}