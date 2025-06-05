#include <vector>
#include <iostream>
#include <chrono>

#include "str.hh"
#include "opt.hh"
#include "print.hh"

// #include "res.hh"


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
    print(str_10c);
    println(str_10c);
    // str_10c.print();
    // str_10c.println();
    // str_10c.~Str(); // double free

    Str str_c ("hola");
    // str_c.println();
    println(str_c);
    // str_c.~Str(); // double free
    // str_c[1];

    Str str_5a (5, 'a');
    // Str str_5a_2 = str_5a; // Copy deleted!

    // REMEMBER EXPLICIT 'Str' constructor or initializer list!
    // Str str_5b (5, 'b');             // OK - initialization_value constructor
    // Str str_5b = { 5, 'b' };         // OK - initialization_value constructor
    // Str str_5b = Str(5, 'b');        // OK - initialization_value constructor
    // Str str_5b = 5;                 // OK - Str size constructor
    // Str str_5b = (5, 'b');       // OK - Str size constructor (comma operator, 5 is discarded and 'b' is passed to the matching Str-constructor! Thus 98 chars will be allocated but no initialized)



    // Hypothesis: {...} or Str(...) always calls the appropriate constructor first, returns object(reference?), then uses that object accordingly. If constructor is called implicity (...) then it will never move, and just construct that object normally, even inside move (std::move((...))). I guess implicit construction only works for single argument construction...

    str_5a = Str(5, 'a'); // Str size constructor + MOVE OPERATOR + destructor of "aaaaa"
    // str_5a = std::move(str_5b); // MOVE ASSIGNMENT OPERATOR 
    // Str new_str ({5, 'c'}); // initialization_value constructor = Str new_str {5, 'c'};
    // Str new_str = std::move((5, 'c')); // Str size constructor
    // Str new_str (std::move((5))); // Str size constructor
    // Str new_str (std::move((5))); // // initialization_value cons + Move cons + ~Str
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

    
    // Str str1 (1);
    // str1 = std::move(str1);
    // Str str20 (20);
    // Str str30 (30);

    std::vector<Str> vec;
    vec.emplace_back();
    vec.pop_back();

    {
    Str& str10 = strings.emplace_back(10, 'a');  // 1st elem : allocates 1 when executed
    Str& str20 = strings.emplace_back(20, 'a');  // 2nd elem : allocates 2, moves 1
    Str& str30 = strings.emplace_back(30, 'a');  // 3rd elem : allocates 4, moves 2
    Str& str40 = strings.emplace_back(40, 'a');
    Str& str50 = strings.emplace_back(50, 'a');  // 5th elem : allocates 8, moves 4
    Str& str60 = strings.emplace_back(60, 'a');
    Str& str70 = strings.emplace_back(80, 'a');
    Str& str90 = strings.emplace_back(90, 'a'); 
    Str& str100 = strings.emplace_back(100, 'a'); // 9th elem : allocates 16, moves 8
    Str& str110 = strings.emplace_back(110, 'a');

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
    std::cout << sizeof(Str) << std::endl;
    std::cout << sizeof(std::string) << std::endl;
    
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


void opt_move_str(){

    OptMove<int> opt_empty;
    std::cout << "opt_empty.has_value() = " << opt_empty.has_value() << std::endl;

    int i = 123;
    OptMove<int> opt_int = { 10 };
    // std::cout << "opt_int.has_value() = " << opt_int.has_value() << std::endl;
    std::cout << "opt_int.consume() = " << opt_int.consume() << std::endl;
    // opt_int = i; // No copy
    opt_int = std::move(i); // only move
    std::cout << "opt_int.consume() = " << opt_int.consume() << std::endl;

    Str str {10, 'x'};
    // OptMove<Str> opt_str_copy = str; // No copy
    // OptMove<Str> opt_move_str = Str(10, 'b');
    // OptMove<Str> opt_str_list = Str(10, 'b') ;
    // OptMove<Str> opt_str_list = str; // implicit move not ok
    OptMove<Str> opt_str_list = std::move(str);
    // std::cout << "opt_move_str.has_value() = " << opt_str_copy.has_value() << std::endl;
    Str opt_str_list_value = opt_str_list.consume();
    Str opt_str_list_value_consumed_again = opt_str_list.consume();
    std::cout << "value.consume() = " << opt_str_list_value.to_std_string() << std::endl;
    std::cout << "value_consumed_again.consume() = " << opt_str_list_value_consumed_again.to_std_string() << std::endl;

}

void res_str(){
    // Str str = "apa";
    // Res<Str> res_int { str };

}

void concat(){
    Str str_lval {5, 'z'};
    Str str = "123";
    // str.println_quotes();
    println(Str("\"") + str + Str("\""));
    str += "456";
    str.println_quotes();
    str += "789";
    str.println_quotes();

    bool test = true;
    if(test)
        str += "asdf";
    else
        str += "fdsa";
    str.println();

    // str += str_lval;
    // str.println_quotes();

    const char* str_c = str.to_c_str();
    std::cout << "str_c: \"" << str_c << "\"" << std::endl;
    std::cout << "strlen(str_c): " << strlen(str_c) << std::endl;
    
}

void substr(){

    Str str = "abcdef";
    Str str_ = str.substr(0, 0);
    Str str_abc = str.substr(0, 3);
    Str str_abcdef = str.substr(0, 6);

    std::cout << "str        = "; str.println();
    std::cout << "str_       = "; str_.println();
    std::cout << "str_abc    = "; str_abc.println();
    std::cout << "str_abcdef = "; str_abcdef.println();
    
    str.cut_to_substr(3, 3);
    std::cout << "str        = "; str.println();

    Str bounds = "abc";
    bounds.cut_to_substr(0, 20000);
    std::cout << bounds.to_std_string() << std::endl;
    
}

struct Timer {
    std::chrono::system_clock::time_point start_tp;
    std::chrono::system_clock::time_point stop_tp;
    std::chrono::nanoseconds duration_ns;

    std::chrono::_V2::system_clock::rep reps;

    void start(){
        start_tp = std::chrono::system_clock::now();
    }
    void stop(){
        stop_tp = std::chrono::system_clock::now();

        duration_ns = stop_tp - start_tp;
        reps = duration_ns.count();
    }
    std::chrono::nanoseconds get_duration(){
        return duration_ns;
    }
    void print_duration(Str str_msg){
        // str_msg.print();
        print(str_msg);
        std::cout << " " << reps << std::endl;
    }

};

void str_perf(){

    Timer timer;
    
    // Str.time() / std::string.time()
    //
    // c_str construction ~ 3
    // substr ~ <1
    // copy   ~ 2-3
    // move   ~ 5-6

    // Str
    timer.start();
    for(size_t i = 0; i < 1000; i++){
        // Str str = "This is a short string made for testing purposes.";

        // Str str {10, '3'};
        // str.substr(0, 10);
        // str.substr(0, 9);

        Str str0 = "copy|move";
        for (size_t i = 0; i < 1000; i++){
            Str str_cpy = str0;
            // Str str_cpy = std::move(str0);
        }
        
    }
    timer.stop();
    timer.print_duration("Duration Str         [ns]: ");

    // std::string
    timer.start();
    for(size_t i = 0; i < 1000; i++){

        // std::string std_string = "This is a short string made for testing purposes.";

        // std::string std_string  {10, '3'};
        // std_string.substr(0, 10);
        // std_string.substr(0, 9);

        std::string str0 = "copy|move";
        for (size_t i = 0; i < 1000; i++){
            std::string str_cpy = str0;
            // std::string str_cpy = std::move(str0);
        }
    }
    timer.stop();
    timer.print_duration("Duration std::string [ns]: ");

}


void print_hh(){
    Str str = "STRING TO PRINT";

    println(str);
    print(str);
    print("\n");

    println_e(str);
    print_e(str);
    print_e("\n");

}


int main(){

    std::cout << "Str main." << std::endl << std::endl;



    // constructors();
    // double_free();
    // string_vector();
    // free_delete();
    // str_c_and_std_interface();
    
    // opt_move_str();
    // res_str();

    // concat();
    substr();

    // str_perf();

    // print_hh();


    std::cout << "Str End" << std::endl << std::endl;
    return 0;
}