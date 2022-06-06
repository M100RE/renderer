#include "renderer/ui/panel.h"
#include <iostream>

unsigned int inds[] = 
{
    0, 1, 2,
    0, 3, 2
};

panel::panel(rect_pos position, color col, pos_def bounds, int window_width, int window_height)
    : window_width(window_width), window_height(window_height),
    definite_position(position), definite_bounds(bounds), definite_color(col),
    shape(calculate_position(position), 8 * sizeof(float), inds, 6 * sizeof(unsigned int), "../src/ui/panel/panel.vert", "../src/ui/panel/panel.frag")
{
    receive_shader().uniform4f("color", col.r, col.g, col.b, col.a);
}

float panel::calculate_single_position(float pos, NUM_TYPE type, bool x_y)
{
    switch(type)
    {
        case REL: return pos;
        case PX: return pos / ((x_y) ? window_height : window_width) * 2 - 1;
        case PER: return pos / 50 - 1;
        default: return -10000;
    }
}

float* panel::calculate_position(rect_pos position)
{
    float raw_vertices[4];

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            int current_index = i * 2 + j;
            raw_vertices[current_index] = calculate_single_position(position.positions[current_index], position.types[current_index], j);
        }
    }

    raw_vertices[2] += 1 + raw_vertices[0];
    raw_vertices[3] += 1 + raw_vertices[1];

    if(definite_bounds.up_bound)
    {
        raw_vertices[1] *= -1;
        raw_vertices[3] *= -1;
    }
    if(definite_bounds.right_bound)
    {
        raw_vertices[0] *= -1;
        raw_vertices[2] *= -1;
    }

    float* vertices = new float[]
    {
        raw_vertices[0], raw_vertices[3],
        raw_vertices[2], raw_vertices[3],
        raw_vertices[2], raw_vertices[1],
        raw_vertices[0], raw_vertices[1]
    };

    return vertices;
}

void panel::resize(rect_pos position)
{
    float* vertices = calculate_position(position);
    change_vertices(vertices, 8 * sizeof(float));
}

void panel::update(int window_width, int window_height)
{
    if(this->window_width != window_width || this->window_height != window_height)
    {
        this->window_width = window_width;
        this->window_height = window_height;
        resize(definite_position);
    }

    draw();
}