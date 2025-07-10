#pragma once
#include "../sdk/memory.h"
#include "../sdk/sdk.h"
#include <vector>
#include <string>
#include <memory>

class Overlay;

struct Player {
    uintptr_t address;
    std::string name;
    Vector3 position;
    bool valid;
};

class ActorLoopClass
{
public:
    ActorLoopClass();
    bool Initialize();
   
    void Render();                  
    void Render(Overlay* overlay);

    std::vector<Player> GetPlayers();
    Matrix4x4 GetViewMatrix();

    bool WorldToScreen(const Vector3& world_pos, Vector2D& screen_pos, const Matrix4x4& view_matrix);

private:
    std::unique_ptr<Memory> memory;
    uintptr_t game_base;
    uintptr_t local_player;

    std::string ReadString(uintptr_t address);
    std::string LengthReadString(uintptr_t string);
    std::string GetInstanceName(uintptr_t instance_address);
    std::string GetInstanceClassName(uintptr_t instance_address);
    
    uintptr_t FindFirstChild(uintptr_t instance_address, const std::string& child_name);
    uintptr_t FindFirstChildByClass(uintptr_t instance_address, const std::string& class_name);
};

extern std::unique_ptr<ActorLoopClass> ActorLoop;