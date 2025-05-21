#include <vector>
#include <iostream>


#include "str.hh"



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


void constructors(){


    {
    Str str_10c (10, 'c');
    str_10c.print();
    str_10c.println();
    // str_10c.~Str(); // double free

    Str str_c ("hola");
    str_c.println();
    // str_c.~Str(); // double free
    // str_c[1];

    Str str_5a (5, 'a');
    // Str str_5a_2 = str_5a;

    // REMEMBER EXPLICIT 'Str' constructor or initializer list!
    // Str str_5b = Str(5, 'b'); 
    // Str str_5b = { 5, 'b' }; 
    Str str_5b = (5, 'b'); // Here the value '98' will take '5's place!

    str_5a = (5, 'a');

    }

}

void double_free(){

    // Double free
    {
    Str str_A1 (10, 'A'); // fill ten chars with 'A'
    str_A1.println();

    // Str str_A2 = str_A1;  // Make copy that also owns the string pointer
    // str_A2.println();

    }// Double free as both objects tries to free the same pointer


    // NOT double free
    {
    Str str_A1 (10, 'A'); // fill ten chars with 'A'
    str_A1.println();

    Str& str_A2 = str_A1;  // Make copy that also owns the string pointer
    str_A2.println();

    }// NOT Double free as second object is reference. Does not call destruct!

}


void string_vector(){

    std::vector<Str> strings;
    
    // Str str10 (10);
    // Str str20 (20);
    // Str str30 (30);

    std::vector<Str> vec;
    vec.emplace_back();
    vec.pop_back();

    {
    Str& str10 = strings.emplace_back(10);
    Str& str20 = strings.emplace_back(20);
    Str& str30 = strings.emplace_back(30);
    } // Does not call destructors because they are references

    // Here constructores are called
    // strings.pop_back();
    // strings.pop_back();

}

void allocator(){

}

void free_delete(){


    std::cout << std::endl << "New" << std::endl;
    
    void* n = ::operator new (sizeof(int));

    void* nn = new Str;                            // calls constructor
    void* nnn = ::operator new (sizeof(Str));      // no constructor : only uses size of struct
    Str* new_str8 = static_cast<Str*>( new Str );// calls constructor

    // Str* ni = (Str*) n;
    // *ni = 10;
    
    // std::cout << ni << std::endl;
    // ni->println();
    // std::cout << *n << std::endl;
    std::cout << "end new" << std::endl << std::endl;

}


void str_c_and_std_interface(){


    std::cout << "STD: " << std::endl;
    
    // string literal ok
    Str str_to_std_c = "sdafljadas"; // c_str constructor
    std::string std_str_c = str_to_std_c.to_std_string();
    std::cout << std_str_c << std::endl;

    // not null-terminated
    Str str_to_std_a (10, 'a');
    std::string std_str_a = str_to_std_a.to_std_string();
    std::cout << std_str_a << std::endl;
    // print retrieved 'c_str'
    std::cout << str_to_std_a.to_c_str() << std::endl;

    std::cout << "STD end: " << std::endl << std::endl;

}



int main(){
    std::cout << "Str main." << std::endl << std::endl;



    constructors();

    double_free();

    string_vector();

    free_delete();

    str_c_and_std_interface();
    


    std::cout << "Str End" << std::endl << std::endl;
    return 0;
}