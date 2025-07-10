#pragma once
#include <windows.h>
#include <string>
#include <tlhelp32.h>

struct Vector3;
struct Matrix4x4;

class Memory
{
private:
    HANDLE process_handle;
    DWORD process_id;
    uintptr_t base_address;

public:
    Memory();
    ~Memory();
    
    DWORD GetProcessId(const std::string& process_name);
    uintptr_t GetModuleBase(const std::string& module_name);
    bool AttachToProcess(const std::string& process_name);
    
    template<typename T>
    T ReadMemory(uintptr_t address);
    
    std::string ReadString(uintptr_t address, size_t max_length = 64);
    
    uintptr_t GetBaseAddress() const;
    bool IsValid() const;
}; 