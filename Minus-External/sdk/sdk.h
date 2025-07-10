#pragma once
#include <windows.h>
#include <string>

struct Vector3
{
    float x, y, z;
};

struct Vector2
{
    float x, y;
};

struct Vector2D
{
    float x, y;
};

struct Matrix4x4
{
    float m[4][4];
};

class Math
{
public:
    static bool WorldToScreen(Vector3 world_pos, Vector2& screen_pos, Matrix4x4 view_matrix);
}; 