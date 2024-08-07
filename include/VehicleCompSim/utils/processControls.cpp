#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

#ifdef Q_OS_WIN
    #include <windows.h>
    #include <tlhelp32.h>

// Function to suspend all threads in a process
bool suspendProcess(DWORD pid) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return false;

    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);
    if (!Thread32First(hSnapshot, &te32)) {
        CloseHandle(hSnapshot);
        return false;
    }

    bool result = true;
    do {
        if (te32.th32OwnerProcessID == pid) {
            HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
            if (hThread != NULL) {
                if (SuspendThread(hThread) == (DWORD)-1) {
                    result = false;
                }
                CloseHandle(hThread);
            }
        }
    } while (Thread32Next(hSnapshot, &te32));

    CloseHandle(hSnapshot);
    return result;
}

// Function to resume all threads in a process
bool resumeProcess(DWORD pid) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return false;

    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);
    if (!Thread32First(hSnapshot, &te32)) {
        CloseHandle(hSnapshot);
        return false;
    }

    bool result = true;
    do {
        if (te32.th32OwnerProcessID == pid) {
            HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
            if (hThread != NULL) {
                if (ResumeThread(hThread) == (DWORD)-1) {
                    result = false;
                }
                CloseHandle(hThread);
            }
        }
    } while (Thread32Next(hSnapshot, &te32));

    CloseHandle(hSnapshot);
    return result;
}

#else
#include <signal.h>
#include <unistd.h>

// Function to suspend a process
bool suspendProcess(pid_t pid) {
    if (kill(pid, SIGSTOP) == 0) {
        return true;
    }
}

// Function to resume a process
bool resumeProcess(pid_t pid) {
    if (kill(pid, SIGCONT) == 0) {
        return true;
    }
}

#endif
