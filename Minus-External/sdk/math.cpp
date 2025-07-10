#include "sdk.h"

bool Math::WorldToScreen(Vector3 world_pos, Vector2& screen_pos, Matrix4x4 view_matrix)
{
    float w = view_matrix.m[3][0] * world_pos.x + view_matrix.m[3][1] * world_pos.y + view_matrix.m[3][2] * world_pos.z + view_matrix.m[3][3];
    if (w < 0.01f) return false;
    float x = view_matrix.m[0][0] * world_pos.x + view_matrix.m[0][1] * world_pos.y + view_matrix.m[0][2] * world_pos.z + view_matrix.m[0][3];
    float y = view_matrix.m[1][0] * world_pos.x + view_matrix.m[1][1] * world_pos.y + view_matrix.m[1][2] * world_pos.z + view_matrix.m[1][3];
    screen_pos.x = (GetSystemMetrics(SM_CXSCREEN) / 2.0f) + (x / w) * (GetSystemMetrics(SM_CXSCREEN) / 2.0f);
    screen_pos.y = (GetSystemMetrics(SM_CYSCREEN) / 2.0f) - (y / w) * (GetSystemMetrics(SM_CYSCREEN) / 2.0f);
    return (screen_pos.x >= 0 && screen_pos.x <= GetSystemMetrics(SM_CXSCREEN) && screen_pos.y >= 0 && screen_pos.y <= GetSystemMetrics(SM_CYSCREEN));
} 