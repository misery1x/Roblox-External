#pragma once

#include <windows.h>

namespace Offsets
{
    constexpr uintptr_t FakeDataModelPointer = 0x682B928;
    constexpr uintptr_t FakeDataModelToDataModel = 0x1B8;
    constexpr uintptr_t VisualEnginePointer = 0x66712A8;
    constexpr uintptr_t viewmatrix = 0x4B0;
    constexpr uintptr_t LocalPlayer = 0x128;
    constexpr uintptr_t Children = 0x80;
    constexpr uintptr_t ChildrenEnd = 0x8;
    constexpr uintptr_t Name = 0x78;
    constexpr uintptr_t ModelInstance = 0x340;
    constexpr uintptr_t Primitive = 0x178;
    constexpr uintptr_t Position = 0x13C;
    constexpr uintptr_t ClassDescriptor = 0x18;
    constexpr uintptr_t ClassDescriptorToClassName = 0x8;
    constexpr uintptr_t PlayerArrayStep = 0x10;
    constexpr uintptr_t StringLength = 0x10;
    constexpr uintptr_t Camera = 0x68;
    constexpr uintptr_t ViewMatrix = 0x4B0;
}