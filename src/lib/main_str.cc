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

void move_construct__(Str&& rstr){
    Str new_str (std::move(rstr));
}
void move_construct_(Str& rstr){
    Str new_str (std::move(rstr));
}
void move_construct(Str rstr){
    Str new_str (std::move(rstr));
}
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
    // Str str_5a_2 = str_5a; // Copy deleted!

    // REMEMBER EXPLICIT 'Str' constructor or initializer list!
    // Str str_5b (5, 'b');             // OK - initialization_value constructor
    // Str str_5b = { 5, 'b' };         // OK - initialization_value constructor
    // Str str_5b = Str(5, 'b');        // OK - initialization_value constructor
    Str str_5b = 5;                 // OK - Str size constructor
    // Str str_5b = (5, 'b');       // OK - Str size constructor (comma operator, 5 is discarded and 'b' is passed to the matching Str-constructor! Thus 98 chars will be allocated but no initialized)



    // Hypothesis: {...} or Str(...) always calls the appropriate constructor first, returns object(reference?), then uses that object accordingly. If constructor is called implicity (...) then it will never move, and just construct that object normally, even inside move (std::move((...))). I guess implicit construction only works for single argument construction...

    str_5a = Str(5, 'a'); // Str size constructor + MOVE OPERATOR + destructor of "aaaaa"
    str_5a = std::move(str_5b); // MOVE ASSIGNMENT OPERATOR 
    // Str new_str ({5, 'c'}); // initialization_value constructor = Str new_str {5, 'c'};
    // Str new_str = std::move((5, 'c')); // Str size constructor
    // Str new_str (std::move((5))); // Str size constructor
    Str new_str (std::move((5))); // // initialization_value cons + Move cons + ~Str
    // Str new_str (std::move(Str(5, 'c'))); // initialization_value cons + Move cons + ~Str
    // Str new_str (std::move(str_5a)); // MOVE CONSTRUCTOR

    // int i = (5, 'f'); // Discards 5 -> {'f'} = {102}

    // move_construct(str_5a);  // Copy not available
    move_construct_(str_5a);    // No movement requored
    move_construct__(std::move(str_5a)); // Have to explicity move
    
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

    std::vector<Str> strings; // Zero capacity
    // std::cout << strings.capacity() << std::endl;

    
    // Str str10 (10);
    // Str str20 (20);
    // Str str30 (30);

    std::vector<Str> vec;
    vec.emplace_back();
    vec.pop_back();

    {
    Str& str10 = strings.emplace_back(10);  // 1st elem : allocates 1 when executed
    Str& str20 = strings.emplace_back(20);  // 2nd elem : allocates 2, moves 1
    Str& str30 = strings.emplace_back(30);  // 3rd elem : allocates 4, moves 2
    Str& str40 = strings.emplace_back(40);
    Str& str50 = strings.emplace_back(50);  // 5th elem : allocates 8, moves 4
    Str& str60 = strings.emplace_back(60);
    Str& str70 = strings.emplace_back(80);
    Str& str90 = strings.emplace_back(90); 
    Str& str100 = strings.emplace_back(100); // 9th elem : allocates 16, moves 8
    Str& str110 = strings.emplace_back(110);

    std::cout << strings.capacity() << std::endl;

    str10.busy();
    str20.busy();
    str30.busy();
    str40.busy();
    str50.busy();
    str60.busy();
    str70.busy();
    str90.busy();
    str100.busy();
    str110.busy();

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



    // constructors();
    // double_free();
    string_vector();
    // free_delete();
    // str_c_and_std_interface();
    


    std::cout << "Str End" << std::endl << std::endl;
    return 0;
}