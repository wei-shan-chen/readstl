#ifndef STLMODEL_H
#define STLMODEL_H

#include <vector>
#include "Vertex.h"
typedef float vector3f[3];

class STLmodel{
public:
    STLmodel();
    ~STLmodel();
    bool LoadSTLfile(const char *stlFileName);
    std::vector<Vertex> m_MeshTri;
    int triangleNum = 0;
    float m_scale = 100;

private:
    vector3f m_MaxPos;
    vector3f m_MinPos;
};

#endif