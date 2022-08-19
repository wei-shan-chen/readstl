#ifndef WORLD_H
#define WORLD_H

#include <array>
#include <vector>

#include "Vertex.h"

struct World {
    std::vector<Vertex> squarett;
    std::vector<Vertex> square;
    std::vector<unsigned int> squ_indices;
    std::vector<Vertex> cube;
    std::vector<Vertex> tri;
    std::vector<Vertex> head;
    std::vector<Vertex> blade;
    std::vector<Vertex> tail;
    std::vector<Vertex> floor;

    //std::array<Attribute, 4> square; 
    //std::array<unsigned int, 6> squ_indices; 
    // std::array<float, 108> cube; 
    // std::array<float, 9> tri; 
    //std::array<unsigned int, 36> cube_ebo; 
};

void create_world();
void destroy_world();

extern struct World world;

#endif