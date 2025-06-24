#pragma once

#include "vecmat.hh"

#include "scene/sid.hh"

#include "scene/object.hh"


typedef unsigned int QID;


enum class QuadType
{
    Scene2D,
};


struct Quad 
{
    QID qid = 0;
    QuadType qtype = QuadType::Scene2D;

    unsigned int texture_id = 0; // LEGACY

    SID sid_2D = 0; // 2D scene id -- part of future variant

    bool is_scene2D() { return qtype == QuadType::Scene2D; }
};


struct QuadO 
{
    TagO    tag;
    Quad    quad;

    QuadO(TagO tag, Quad quad)
        :   tag     {tag},
            quad    {quad}
    {
    }
};

