#ifndef CREATE_DUMP_WINDOWS_H
#define CREATE_DUMP_WINDOWS_H

#include <windows.h>
#include <dbghelp.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

std::wstring ConvertToWide(const std::string& str) {
    // Use std::wstring_convert for conversion (deprecated in C++17 but still works)
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

bool CreateProcessDump(DWORD pid, const std::string& outputPath) {//const TCHAR*
    // Load DbgHelp.dll
    HMODULE hDbgHelp = LoadLibrary(_T("DbgHelp.dll"));
    if (!hDbgHelp) {
        std::cerr << "Failed to load DbgHelp.dll\n";
        return false;
    }

    // Get the address of MiniDumpWriteDump
    typedef BOOL(WINAPI* MiniDumpWriteDump_t)(
        HANDLE hProcess,
        DWORD ProcessId,
        HANDLE hFile,
        MINIDUMP_TYPE DumpType,
        PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
        PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
        PMINIDUMP_CALLBACK_INFORMATION CallbackParam
        );
    MiniDumpWriteDump_t MiniDumpWriteDump = (MiniDumpWriteDump_t)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
    if (!MiniDumpWriteDump) {
        std::cerr << "Failed to get address of MiniDumpWriteDump\n";
        FreeLibrary(hDbgHelp);
        return false;
    }

    // Open the process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        std::cerr << "Failed to open process: " << GetLastError() << '\n';
        FreeLibrary(hDbgHelp);
        return false;
    }

    // Create the dump file
    HANDLE hFile = CreateFile(ConvertToWide(outputPath).c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create dump file: " << GetLastError() << '\n';
        CloseHandle(hProcess);
        FreeLibrary(hDbgHelp);
        return false;
    }

    // Write the dump
    MINIDUMP_EXCEPTION_INFORMATION mdei = { 0 };
    mdei.ThreadId = GetCurrentThreadId();
    mdei.ExceptionPointers = NULL;
    mdei.ClientPointers = FALSE;

    BOOL result = MiniDumpWriteDump(hProcess, pid, hFile, MiniDumpWithFullMemory, &mdei, NULL, NULL);
    if (!result) {
        std::cerr << "MiniDumpWriteDump failed: " << GetLastError() << '\n';
    }

    // Clean up
    CloseHandle(hFile);
    CloseHandle(hProcess);
    FreeLibrary(hDbgHelp);

    return result != 0;
}

#endif // !#define CREATE_DUMP_WINDOWS_H
