
#include "print.hh"

#include "list.hh"



int main() {

    // println( Str("Start main_lib") );
    println( "Start main_lib\n" );

    Str str = "hola";
    print_format(str);
    print_format(str, str);

    println_any(str);
    println_any("hola");
    println_any(std::move(Str{"hola"}));

    println("");

    // println_list(str, str);
    println_list(str);

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





    println( "End main_lib \n" );

    return 0;
}