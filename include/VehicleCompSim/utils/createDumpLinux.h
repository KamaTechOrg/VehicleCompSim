#ifndef CREATE_DUMP_LINUX_H
#define CREATE_DUMP_LINUX_H

#include <iostream>

bool CreateProcessDump(pid_t pid, const std::string& outputPath) {
    std::string command = "gcore -o " + outputPath + " " + std::to_string(pid);
    int result = std::system(command.c_str());

    if (result != 0) {
        std::cerr << "Failed to create full dump with gcore. Return code: " << result << std::endl;
        return false;
    }

    return true;
}
#endif // !#define CREATE_DUMP_LINUX_H
