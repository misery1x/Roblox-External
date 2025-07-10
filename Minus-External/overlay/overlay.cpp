#include "overlay.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Overlay::Overlay() {}

Overlay::~Overlay()
{
    Cleanup();
}

bool Overlay::Initialize()
{
    CreateOverlayWindow();
    InitializeDirectX();
    InitializeImGui();
    return true;
}

void Overlay::CreateOverlayWindow()
{
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "Minus";
    RegisterClassEx(&wc);
    
    window_handle = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
        "Minus", "external-maryjayne",
        WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, GetModuleHandle(NULL), NULL
    );
    
    SetLayeredWindowAttributes(window_handle, RGB(0, 0, 0), 0, LWA_COLORKEY);
    ShowWindow(window_handle, SW_SHOW);
    UpdateWindow(window_handle);
}

void Overlay::InitializeDirectX()
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = GetSystemMetrics(SM_CXSCREEN);
    scd.BufferDesc.Height = GetSystemMetrics(SM_CYSCREEN);
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 0;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = window_handle;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    
    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,  D3D11_CREATE_DEVICE_SINGLETHREADED, NULL, 0, D3D11_SDK_VERSION, &scd,  &swapchain, &device,  &featureLevel,  &context);

    IDXGIDevice* dxgiDevice = nullptr;
    device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
    if (dxgiDevice)
    {
        IDXGIAdapter* dxgiAdapter = nullptr;
        dxgiDevice->GetAdapter(&dxgiAdapter);
        if (dxgiAdapter)
        {
            IDXGIFactory* dxgiFactory = nullptr;
            dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
            if (dxgiFactory)
            {
                dxgiFactory->MakeWindowAssociation(window_handle, DXGI_MWA_NO_ALT_ENTER);
                dxgiFactory->Release();
            }
            dxgiAdapter->Release();
        }
        dxgiDevice->Release();
    }
    
    ID3D11Texture2D* back_buffer;
    swapchain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
    device->CreateRenderTargetView(back_buffer, NULL, &render_target_view);
    back_buffer->Release();
}

void Overlay::InitializeImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.AntiAliasedLines = false;
    style.AntiAliasedFill = false;
    
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(window_handle);
    ImGui_ImplDX11_Init(device, context);
}

void Overlay::BeginFrame()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
    context->OMSetRenderTargets(1, &render_target_view, NULL);
}

void Overlay::EndFrame()
{
    DrawWatermark();
    
    ImGui::Render();
    
    static const float clear_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    context->ClearRenderTargetView(render_target_view, clear_color);
    
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    
    HRESULT hr = swapchain->Present(0, DXGI_PRESENT_DO_NOT_WAIT);
}

void Overlay::DrawText(Vector2 position, const char* text, unsigned int color)
{
    ImGui::GetForegroundDrawList()->AddText(ImVec2(position.x, position.y), color, text);
}

void Overlay::DrawWatermark()
{

}

bool Overlay::ShouldExit()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_QUIT) return true;
    }
    return false;
}

bool Overlay::IsRunning()
{
    return !ShouldExit();
}

LRESULT CALLBACK Overlay::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        return true;
    
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Overlay::Cleanup()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    
    if (render_target_view) render_target_view->Release();
    if (swapchain) swapchain->Release();
    if (context) context->Release();
    if (device) device->Release();
    
    if (window_handle) DestroyWindow(window_handle);
} 