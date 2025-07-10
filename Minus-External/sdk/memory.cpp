#include "memory.h"
#include "sdk.h"

Memory::Memory() : process_handle(nullptr), process_id(0), base_address(0) {}

Memory::~Memory()
{
    if (process_handle) CloseHandle(process_handle);
}

DWORD Memory::GetProcessId(const std::string& process_name)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &entry))
    {
        do
        {
            if (process_name == entry.szExeFile)
            {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        } while (Process32Next(snapshot, &entry));
    }
    
    CloseHandle(snapshot);
    return 0;
}

uintptr_t Memory::GetModuleBase(const std::string& module_name)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;
    
    MODULEENTRY32 entry;
    entry.dwSize = sizeof(MODULEENTRY32);
    
    if (Module32First(snapshot, &entry))
    {
        do
        {
            if (module_name == entry.szModule)
            {
                CloseHandle(snapshot);
                return (uintptr_t)entry.modBaseAddr;
            }
        } while (Module32Next(snapshot, &entry));
    }
    
    CloseHandle(snapshot);
    return 0;
}

bool Memory::AttachToProcess(const std::string& process_name)
{
    process_id = GetProcessId(process_name);
    if (process_id == 0) return false;
    
    process_handle = OpenProcess(PROCESS_VM_READ, FALSE, process_id);
    if (!process_handle) return false;
    
    base_address = GetModuleBase(process_name);
    if (base_address == 0) return false;
    
    return true;
}

template<typename T>
T Memory::ReadMemory(uintptr_t address)
{
    T value = {};
    ReadProcessMemory(process_handle, (LPCVOID)address, &value, sizeof(T), nullptr);
    return value;
}

std::string Memory::ReadString(uintptr_t address, size_t max_length)
{
    char buffer[256] = {};
    ReadProcessMemory(process_handle, (LPCVOID)address, buffer, min(max_length, sizeof(buffer) - 1), nullptr);
    return std::string(buffer);
}

uintptr_t Memory::GetBaseAddress() const 
{
    return base_address; 
}

bool Memory::IsValid() const
{
    return process_handle != nullptr; 
}

template int Memory::ReadMemory<int>(uintptr_t address);
template char Memory::ReadMemory<char>(uintptr_t address);
template float Memory::ReadMemory<float>(uintptr_t address);
template uintptr_t Memory::ReadMemory<uintptr_t>(uintptr_t address);
template struct Vector3 Memory::ReadMemory<struct Vector3>(uintptr_t address);
template struct Matrix4x4 Memory::ReadMemory<struct Matrix4x4>(uintptr_t address); 