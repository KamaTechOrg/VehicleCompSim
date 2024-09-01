#ifndef CREATE_DUMP_H
#define CREATE_DUMP_H

#ifdef _WIN32
#include "createDumpWindows.h"
bool CreateProcessDump(DWORD pid, const std::string& outputPath);
#else
#include "createDumpLinux.h"
bool CreateProcessDump(pid_t pid, const std::string& outputPath) {

#endif // _WIN32


#endif // CREATE_DUMP_H