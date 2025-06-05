
#include "print.hh"
#include "list.hh"
#include "hash.hh"



void prints(){

    Str str = "hola";
    print_format(str);
    print_format(str, str);

    println_any(str);
    println_any("hola");
    // println_any(std::move(Str{"hola"}));

    println("");

    // println_list(str, str);
    println_list(str);

}

void list(){


    LListNode<Str>* head = new LListNode<Str>;
    head->value = "head";
    println(head->value);
    std::cout << head->count_back() << std::endl;
    
    LListNode<Str>* first = head->append();
    first->value = "first";
    println(first->value);
    std::cout << head->count_back() << std::endl;

    LListNode<Str>* second = head->append();
    second->value = "second";
    println(second->value);
    std::cout << head->count_back() << std::endl;

}

void hash_uint_to_uchar(){


    auto print_hash = [](unsigned int num) -> unsigned char {

        unsigned char hash_output = hash<unsigned int, unsigned char>(num);

        std::cout << std::to_string(num) << " > " << std::to_string(hash_output) << std::endl;
        return hash_output;
    };


    print_hash(127);
    print_hash(22);
    print_hash(1046);
    print_hash(65558);
    print_hash(98988997);

}

int main() {

    println( "Start main_lib\n" );


    // prints();
    // list();
    hash_uint_to_uchar();


    println( "\nEnd main_lib \n" );

    return 0;
}