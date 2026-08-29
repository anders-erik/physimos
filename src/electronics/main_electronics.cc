#include "lib/print.hh"
#include "lib/arr.hh"
#include "lib/pair.hh"

class Resistor
{
public:
    double resistance;
};



class Voltage
{
public:
    double value;
};







struct CircuitNode
{
    enum Type
    {
        Voltage,
        Current,
        Resistor,
    };

    Arr<CircuitNode> input_nodes;
    Arr<CircuitNode> output_nodes;

    CircuitNode() {}
    CircuitNode(Arr<CircuitNode> _input_nodes, Arr<CircuitNode> _output_nodes)
        :   input_nodes {_input_nodes}, 
            output_nodes {_output_nodes}
    {

    }
};


class Circuit
{
public:
    
    Arr<Pair<uint, CircuitNode>> nodes;
    
    
};

int main()
{
    println("'Hello' from Electronics!");



    println("'Bye bye' from Electronics!");
    return 0;
}