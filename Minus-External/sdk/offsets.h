#pragma once

#include <windows.h>

namespace Offsets
{
    constexpr uintptr_t FakeDataModelPointer = 0x759FD28;
    constexpr uintptr_t FakeDataModelToDataModel = 0x1C0;
    constexpr uintptr_t VisualEnginePointer = 0x72FE710;
    constexpr uintptr_t viewmatrix = 0x4B0;
    constexpr uintptr_t LocalPlayer = 0x130;
    constexpr uintptr_t Children = 0x70;
    constexpr uintptr_t ChildrenEnd = 0x8;
    constexpr uintptr_t Name = 0x90;
    constexpr uintptr_t ModelInstance = 0x360;
    constexpr uintptr_t Primitive = 0x148;
    constexpr uintptr_t Position = 0x11C;
    constexpr uintptr_t ClassDescriptor = 0x18;
    constexpr uintptr_t ClassDescriptorToClassName = 0x8;
    constexpr uintptr_t PlayerArrayStep = 0x10;
    constexpr uintptr_t StringLength = 0x10;
    constexpr uintptr_t Camera = 0x410;
    constexpr uintptr_t ViewMatrix = 0x4B0;
}
