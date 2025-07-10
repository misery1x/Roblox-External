#pragma once
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <chrono>
#include <iostream>
#include "../sdk/sdk.h"

struct ImDrawList;
struct ImVec2;
typedef int ImGuiCol;

class Overlay
{
private:
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    IDXGISwapChain* swapchain = nullptr;
    ID3D11RenderTargetView* render_target_view = nullptr;
    HWND window_handle = nullptr;

    std::chrono::high_resolution_clock::time_point last_frame_time;
    float current_fps = 0.0f;
    int frame_count = 0;
    std::chrono::high_resolution_clock::time_point fps_timer;
    
public:
    Overlay();
    ~Overlay();
    
    bool Initialize();
    void Cleanup();
    
    void BeginFrame();
    void EndFrame();
    
    void DrawText(Vector2 position, const char* text, unsigned int color = 0xFFFFFFFF);
    void DrawWatermark();
    
    float GetFPS() const { return current_fps; }

    bool ShouldExit();
    bool IsRunning();
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
    void CreateOverlayWindow();
    void InitializeDirectX();
    void InitializeImGui();
};