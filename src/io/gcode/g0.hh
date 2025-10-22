#pragma once

#include "math/vecmat.hh"

#include <variant>
#include <vector>

using GPoint = d3;


struct G0
{
    d3 p_start;
    d3 p_end;
};


struct G1
{
    d3 p_start;
    d3 p_end;
};

// Single size vector blocks, but variable block structure.
// NOTE: could be done the other way around, with fixed block structure and variable size vectors!
using g_block = std::variant<G0, G1>;
using g_code  = std::vector<g_block>;


