#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "../commons/ipc.h"

int main() {
    struct DATA {

    };


    char buffer[MAX_BUFSIZE];
    DWORD dwRead, dwWritten;
    HANDLE hStdin, hStdout;
    BOOL bSuccess;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    std::cout << "debug 3" << std::endl;
    if (hStdout == INVALID_HANDLE_VALUE || hStdin == INVALID_HANDLE_VALUE)
        ExitProcess(1);

    // Send something to this process's stdout using printf.
    printf("\n ** This is a message from the child process. ** \n");

    // This simple algorithm uses the existence of the pipes to control execution.
    // It relies on the pipe buffers to ensure that no data is lost.
    // Larger applications would use more advanced process control.

    for (;;) {
        // Read from standard input and stop on error or no data.
        bSuccess = ReadFile(hStdin, buffer, sizeof(buffer), &dwRead, NULL);
        PACKET p;
        memcpy(&p, buffer, sizeof(PACKET));

        bSuccess = WriteFile(hStdout, buffer, sizeof(buffer), &dwWritten, NULL);
        std::string path = "C:\\Users\\netagive\\Desktop\\Orbital\\test.vbs " + std::to_string(p.id);
        system(path.c_str());
        std::cout << p.id << std::endl;

        if (!bSuccess)
            break;
    }
    return 0;
}