
#include "lib/print.hh"
#include "lib/cli.hh"




int main(int argc, char** argv)
{
    CLI cli {argc, argv};

    

    Print::ln("Hello from main_tasks!");

    return 0;
}