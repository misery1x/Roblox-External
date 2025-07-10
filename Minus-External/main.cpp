#include <iostream>
#include <Windows.h>
#include <dwmapi.h>
#include <mmsystem.h>
#include <memory>
#include "overlay/overlay.h"
#include "features/esp.h"

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "winmm.lib")
#pragma warning(disable: 4995)

std::unique_ptr<ActorLoopClass> ActorLoop = std::make_unique<ActorLoopClass>();
std::unique_ptr<Overlay> OverlayInstance = std::make_unique<Overlay>();

int main()
{
    if (!ActorLoop->Initialize())
    {
        std::cout << "Failed to initialize ESP - make sure Roblox is running" << std::endl;
        std::cin.get();
        return 1;
    }

    if (!OverlayInstance->Initialize())
    {
        std::cout << "Failed to initialize overlay - check DirectX support" << std::endl;
        std::cin.get();
        return 1;
    }

    while (OverlayInstance->IsRunning())
    {
        OverlayInstance->BeginFrame();
        ActorLoop->Render(OverlayInstance.get());
        OverlayInstance->EndFrame();
    }

    return 0;
} 