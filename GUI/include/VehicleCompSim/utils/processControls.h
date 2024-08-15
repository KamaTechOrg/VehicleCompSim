#ifndef PROCESSCONTROLS_H
#define PROCESSCONTROLS_H
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
bool suspendProcess(DWORD pid);

// Function to resume all threads in a process
bool resumeProcess(DWORD pid);

#else

#include <signal.h>
#include <unistd.h>

// Function to suspend a process
bool suspendProcess(pid_t pid) ;

bool resumeProcess(pid_t pid) ;


#endif
#endif // PROCESSCONTROLS_H
