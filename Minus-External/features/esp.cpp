#include "esp.h"
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <immintrin.h> 
#include "../imgui/imgui.h"
#include "../overlay/overlay.h"
#include "../sdk/offsets.h"

static std::vector<Player> cached_players;
static std::chrono::high_resolution_clock::time_point last_player_update;
static std::string temp_string;

ActorLoopClass::ActorLoopClass() 
{
    memory = std::make_unique<Memory>();
    game_base = 0;
    local_player = 0;
    
    cached_players.reserve(16);
}

bool ActorLoopClass::Initialize() 
{
    if (!memory->AttachToProcess("RobloxPlayerBeta.exe"))
    {
        std::cout << "Failed to attach to Roblox process" << std::endl;
        return false;
    }
   
    game_base = memory->GetBaseAddress();
    if (!game_base) 
    {
        std::cout << "Failed to get game base address" << std::endl;
        return false;
    }
    
    return true;
}

std::string ActorLoopClass::ReadString(uintptr_t address)
{
    temp_string.clear();
    temp_string.reserve(64);
    
    char character = 0;
    int offset = 0;

    while (offset < 200)
    {
        character = memory->ReadMemory<char>(address + offset);

        if (character == 0)
            break;

        offset++;
        temp_string.push_back(character);
    }

    return temp_string;
}

std::string ActorLoopClass::LengthReadString(uintptr_t string)
{
    const auto length = memory->ReadMemory<int>(string + Offsets::StringLength);

    if (length >= 16u)
    {
        const auto _new = memory->ReadMemory<uintptr_t>(string);
        return ReadString(_new);
    }
    else
    {
        return ReadString(string);
    }
}

std::string ActorLoopClass::GetInstanceName(uintptr_t instance_address)
{
    const auto _get = memory->ReadMemory<uintptr_t>(instance_address + Offsets::Name);

    if (_get)
        return LengthReadString(_get);

    return "???";
}

std::string ActorLoopClass::GetInstanceClassName(uintptr_t instance_address)
{
    const auto ptr = memory->ReadMemory<uintptr_t>(instance_address + Offsets::ClassDescriptor);
    const auto ptr2 = memory->ReadMemory<uintptr_t>(ptr + Offsets::ChildrenEnd);

    if (ptr2)
        return ReadString(ptr2);

    return "???";
}

uintptr_t ActorLoopClass::FindFirstChild(uintptr_t instance_address, const std::string& child_name)
{
    if (!instance_address)
        return 0;

    static std::unordered_map<uintptr_t, std::vector<std::pair<uintptr_t, std::string>>> cache;
    static std::unordered_map<uintptr_t, std::chrono::high_resolution_clock::time_point> last_update;

    auto now = std::chrono::high_resolution_clock::now();
    auto& children = cache[instance_address];
    auto& update_time = last_update[instance_address];

    if (children.empty() || now - update_time > std::chrono::seconds(1))
    {
        children.clear();
        
        auto start = memory->ReadMemory<uintptr_t>(instance_address + Offsets::Children);
        if (!start)
            return 0;

        auto end = memory->ReadMemory<uintptr_t>(start + Offsets::ChildrenEnd);
        auto childArray = memory->ReadMemory<uintptr_t>(start);
        if (!childArray || childArray >= end)
            return 0;

        children.reserve(32);

        for (uintptr_t current = childArray; current < end; current += 16)
        {
            auto child_instance = memory->ReadMemory<uintptr_t>(current);
            if (!child_instance)
                continue;
            std::string name = GetInstanceName(child_instance);
            children.emplace_back(child_instance, std::move(name));
        }
        update_time = now;
    }

    for (const auto& [child_instance, name] : children)
    {
        if (name == child_name)
            return child_instance;
    }

    return 0;
}

uintptr_t ActorLoopClass::FindFirstChildByClass(uintptr_t instance_address, const std::string& class_name)
{
    if (!instance_address)
        return 0;

    static std::unordered_map<uintptr_t, std::vector<std::pair<uintptr_t, std::string>>> cache;
    static std::unordered_map<uintptr_t, std::chrono::high_resolution_clock::time_point> last_update;

    auto now = std::chrono::high_resolution_clock::now();
    auto& children = cache[instance_address];
    auto& update_time = last_update[instance_address];

    if (children.empty() || now - update_time > std::chrono::seconds(1))
    {
        children.clear();
        children.reserve(32);
        
        auto start = memory->ReadMemory<uintptr_t>(instance_address + Offsets::Children);
        if (!start)
            return 0;

        auto end = memory->ReadMemory<uintptr_t>(start + Offsets::ChildrenEnd);
        auto childArray = memory->ReadMemory<uintptr_t>(start);
        if (!childArray || childArray >= end)
            return 0;

        for (uintptr_t current = childArray; current < end; current += 16)
        {
            auto child_instance = memory->ReadMemory<uintptr_t>(current);
            if (!child_instance)
                continue;
            std::string classname = GetInstanceClassName(child_instance);
            children.emplace_back(child_instance, std::move(classname));
        }
        update_time = now;
    }

    for (const auto& [child_instance, classname] : children)
    {
        if (classname == class_name)
            return child_instance;
    }

    return 0;
}

std::vector<Player> ActorLoopClass::GetPlayers()
{
    auto now = std::chrono::high_resolution_clock::now();
    
    static std::vector<uintptr_t> cached_player_addresses;
    static std::chrono::high_resolution_clock::time_point last_structure_update;
    
    bool need_structure_update = cached_player_addresses.empty() || 
                                now - last_structure_update > std::chrono::seconds(2);
    
    if (need_structure_update)
    {
        cached_player_addresses.clear();
        
        uintptr_t fake_datamodel = memory->ReadMemory<uintptr_t>(game_base + Offsets::FakeDataModelPointer);
        if (!fake_datamodel) return cached_players;
        
        uintptr_t datamodel = memory->ReadMemory<uintptr_t>(fake_datamodel + Offsets::FakeDataModelToDataModel);
        if (!datamodel) return cached_players;
        
        uintptr_t players_service = FindFirstChildByClass(datamodel, "Players");
        if (!players_service) return cached_players;
        
        local_player = memory->ReadMemory<uintptr_t>(players_service + Offsets::LocalPlayer);
        
        auto start = memory->ReadMemory<uintptr_t>(players_service + Offsets::Children);
        if (!start) return cached_players;
        auto end = memory->ReadMemory<uintptr_t>(start + Offsets::ChildrenEnd);
        
        static std::vector<uintptr_t> all_children;
        all_children.clear();
        all_children.reserve(16);
        
        for (auto instances = memory->ReadMemory<uintptr_t>(start); instances != end; instances += 16)
        {
            uintptr_t child = memory->ReadMemory<uintptr_t>(instances);
            if (child) all_children.push_back(child);
        }
        
        for (const auto& child : all_children)
        {
            std::string class_name = GetInstanceClassName(child);
            if (class_name == "Player" && child != local_player)
            {
                cached_player_addresses.push_back(child);
            }
        }
        
        last_structure_update = now;
    }
    
    cached_players.clear();
    cached_players.reserve(cached_player_addresses.size());
    
    for (const auto& player_addr : cached_player_addresses)
    {
        Player player;
        player.address = player_addr;
        player.valid = false;
        
        player.name = GetInstanceName(player_addr);
        
        uintptr_t character = memory->ReadMemory<uintptr_t>(player_addr + Offsets::ModelInstance);
        if (!character) continue;
        
        uintptr_t root_part = FindFirstChild(character, "HumanoidRootPart");
        if (!root_part) continue;
        
        uintptr_t primitive = memory->ReadMemory<uintptr_t>(root_part + Offsets::Primitive);
        if (!primitive) continue;
        
        player.position = memory->ReadMemory<Vector3>(primitive + Offsets::Position);
        player.valid = true;
        
        cached_players.push_back(std::move(player));
    }
    
    return cached_players;
}

void ActorLoopClass::Render()
{
    if (!memory) return;
    
    auto view_matrix = GetViewMatrix();
    auto players = GetPlayers();
    
    for (const auto& player : players)
    {
        if (!player.valid) continue;
        
        Vector2D screen_pos;
        if (WorldToScreen(player.position, screen_pos, view_matrix))
        {
            ImDrawList* draw_list = ImGui::GetForegroundDrawList();
            
            ImVec2 name_pos(screen_pos.x - (player.name.length() * 3.5f), screen_pos.y - 20);
            draw_list->AddText(name_pos, IM_COL32(0, 255, 0, 255), player.name.c_str());
        }
    }
}

void ActorLoopClass::Render(Overlay* overlay)
{
    if (!memory || !overlay) return;
    
    auto view_matrix = GetViewMatrix();
    auto players = GetPlayers();
    
    if (players.empty()) return;
    
    static std::vector<ImVec2> positions;
    static std::vector<const char*> names;
    static std::vector<ImU32> colors;
    
    positions.clear();
    names.clear();
    colors.clear();
    
    const size_t player_count = players.size();
    positions.reserve(player_count);
    names.reserve(player_count);
    colors.reserve(player_count);
    
    for (const auto& player : players)
    {
        if (!player.valid) continue;
        
        Vector2D screen_pos;
        if (WorldToScreen(player.position, screen_pos, view_matrix))
        {
            float text_offset = player.name.length() * 3.5f;
            positions.emplace_back(screen_pos.x - text_offset, screen_pos.y - 25);
            names.push_back(player.name.c_str());
            colors.push_back(0xFF00FF00);
        }
    }
    
    ImDrawList* draw_list = ImGui::GetForegroundDrawList();
    
    _mm_prefetch((const char*)draw_list, _MM_HINT_T0);
    
    for (size_t i = 0; i < positions.size(); ++i)
    {
        draw_list->AddText(positions[i], colors[i], names[i]);
    }
}

Matrix4x4 ActorLoopClass::GetViewMatrix()
{
    Matrix4x4 view_matrix{};
    
    uintptr_t visual_engine = memory->ReadMemory<uintptr_t>(game_base + 0x66712A8);
    if (!visual_engine) return view_matrix;
    
    view_matrix = memory->ReadMemory<Matrix4x4>(visual_engine + 0x4B0);
    return view_matrix;
}

bool ActorLoopClass::WorldToScreen(const Vector3& world_pos, Vector2D& screen_pos, const Matrix4x4& view_matrix) 
{
    const float* m = &view_matrix.m[0][0];

    _mm_prefetch((const char*)m, _MM_HINT_T0);

    float w = m[12] * world_pos.x + m[13] * world_pos.y + m[14] * world_pos.z + m[15];

    if (w <= 0.001f)
        return false;

    float inv_w = 1.0f / w;

    screen_pos.x = (m[0] * world_pos.x + m[1] * world_pos.y + m[2] * world_pos.z + m[3]) * inv_w;
    screen_pos.y = (m[4] * world_pos.x + m[5] * world_pos.y + m[6] * world_pos.z + m[7]) * inv_w;

    static constexpr float screen_width_half = 960.0f;
    static constexpr float screen_height_half = 540.0f;

    screen_pos.x = screen_width_half * (screen_pos.x + 1.0f);
    screen_pos.y = screen_height_half * (1.0f - screen_pos.y);

    return true;
} 