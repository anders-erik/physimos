#include <iostream>

#include "print.hh"
#include "llist.hh"
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

void list()
{
    LList<Str> llist;
    // llist.head->value = "head;"


    Print::buf("LList count = ");
    Print::ln(Str::SI(llist.count()));

    llist.append();
    Print::buf("LList count = ");
    Print::ln(Str::SI(llist.count()));

    llist.head->value = "head";
    Print::buf("LList.head.value = ");
    Print::ln(llist.head->value);
    // std::cout << head->count_back() << std::endl;
    
    LLNode<Str>* first = llist.append();;
    first->value = "first";
    Print::ln(first->value);
    // std::cout << head->count_back() << std::endl;

    LLNode<Str>* second = llist.append();;
    second->value = "second";
    Print::ln(second->value);
    // std::cout << head->count_back() << std::endl;

    Print::buf("LList count = ");
    Print::ln(Str::SI(llist.count()));
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
    list();
    // hash_uint_to_uchar();


    println( "\nEnd main_lib \n" );

    return 0;
}