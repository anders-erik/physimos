
#include "scene/mesh_line.hh"
#include "mesh_line.hh"


void MeshLine::append(const MeshLine& mesh_to_append)
{
    for(const Line& line : mesh_to_append.lines)
        lines.push_back(line);
}

void MeshLine::rotate_x(float angle)
{
    m3f3 RX = m3f3::rotation_x(angle);

    for(auto& line : lines)
    {
        line.a = RX * line.a;
        line.b = RX * line.b;
    }
}

void MeshLine::rotate_y(float angle)
{
    m3f3 RY = m3f3::rotation_y(angle);

    for(auto& line : lines)
    {
        line.a = RY * line.a;
        line.b = RY * line.b;
    }
}

void MeshLine::rotate_z(float angle)
{
    m3f3 RZ = m3f3::rotation_z(angle);

    for(auto& line : lines)
    {
        line.a = RZ * line.a;
        line.b = RZ * line.b;
    }
}


void MeshLine::circle(float radius, uint pts)
{
    lines.clear();

    float angle_delta = PI2f / ((float) pts);
    float angle = 0.0f;

    for(uint i=0; i<pts; i++)
    {
        f3 a = {    cosf(angle), 
                    sinf(angle),
                    0.0f        };

        f3 b = {    cosf(angle + angle_delta ),
                    sinf(angle + angle_delta ),
                    0.0f                        };

        lines.emplace_back( a, b );

        angle += angle_delta;
    }
}



void MeshLine::cube_origin_aligned()
{
    lines.clear();

    float SX = 1.0f;
    float SY = 1.0f;
    float SZ = 1.0f;
    

    // z = 0
    f3 v0 = { 0.0f,   0.0f,   0.0f    };
    f3 v1 = { SX,     0.0f,   0.0f    };
    f3 v2 = { SX,     SY,     0.0f    };
    f3 v3 = { 0.0f,   SY,     0.0f    };
    f3 v4 = { 0.0f,   0.0f,   SZ      };
    f3 v5 = { SX,     0.0f,   SZ      };
    f3 v6 = { SX,     SY,     SZ      };
    f3 v7 = { 0.0f,   SY,     SZ      };

    // bottom
    lines.emplace_back(v0, v1);
    lines.emplace_back(v1, v2);
    lines.emplace_back(v2, v3);
    lines.emplace_back(v3, v0);

    // top
    lines.emplace_back(v4, v5);
    lines.emplace_back(v5, v6);
    lines.emplace_back(v6, v7);
    lines.emplace_back(v7, v4);

    // vertival
    lines.emplace_back(v0, v4);
    lines.emplace_back(v1, v5);
    lines.emplace_back(v2, v6);
    lines.emplace_back(v3, v7);
}


void MeshLine::
aabb(AABBf aabb)
{
    float oversize = 0.01f;
    float half_oversize = oversize * 0.5f;

    float SX = (aabb.max.x - aabb.min.x);
    float SY = (aabb.max.x - aabb.min.x);
    float SZ = (aabb.max.x - aabb.min.x);

    float MIN_X = aabb.min.x - SX * half_oversize;
    float MIN_Y = aabb.min.y - SY * half_oversize;
    float MIN_Z = aabb.min.z - SZ * half_oversize;

    float MAX_X = aabb.max.x + SX * half_oversize;
    float MAX_Y = aabb.max.y + SY * half_oversize;
    float MAX_Z = aabb.max.z + SZ * half_oversize;

    // z = 0
    f3 v0 = { MIN_X,   MIN_Y,   MIN_Z  };
    f3 v1 = { MAX_X,   MIN_Y,   MIN_Z  };
    f3 v2 = { MAX_X,   MAX_Y,   MIN_Z  };
    f3 v3 = { MIN_X,   MAX_Y,   MIN_Z  };

    f3 v4 = { MIN_X,   MIN_Y,   MAX_Z  };
    f3 v5 = { MAX_X,   MIN_Y,   MAX_Z  };
    f3 v6 = { MAX_X,   MAX_Y,   MAX_Z  };
    f3 v7 = { MIN_X,   MAX_Y,   MAX_Z  };

    // bottom
    lines.emplace_back(v0, v1);
    lines.emplace_back(v1, v2);
    lines.emplace_back(v2, v3);
    lines.emplace_back(v3, v0);

    // top
    lines.emplace_back(v4, v5);
    lines.emplace_back(v5, v6);
    lines.emplace_back(v6, v7);
    lines.emplace_back(v7, v4);

    // vertival
    lines.emplace_back(v0, v4);
    lines.emplace_back(v1, v5);
    lines.emplace_back(v2, v6);
    lines.emplace_back(v3, v7);
}


void MeshLine::bounding_sphere(float radius)
{
    float oversize_factor = 1.01;

    float R = radius * oversize_factor;

    uint  circle_point_count = 20;
    circle(R, circle_point_count);

    MeshLine linemesh_tmp;

    linemesh_tmp.circle(R, circle_point_count);
    linemesh_tmp.rotate_x(1.57f);
    append(linemesh_tmp);

    linemesh_tmp.circle(R, circle_point_count);
    linemesh_tmp.rotate_y(1.57f);
    append(linemesh_tmp);
}