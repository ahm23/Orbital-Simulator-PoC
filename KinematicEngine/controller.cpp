#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "engine.h"

#include "../commons/ipc.h"

int main() {
    DWORD dwRead;
    HANDLE hStdin, hStdout;
    BOOL bSuccess;
    bool toggle_kinematic = false;
    std::condition_variable_any cv;
    std::shared_mutex mut;


    ENGINE* e = new ENGINE(1, &mut, &cv);


    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hStdin = GetStdHandle(STD_INPUT_HANDLE);

    if (hStdout == INVALID_HANDLE_VALUE || hStdin == INVALID_HANDLE_VALUE)
        ExitProcess(1);

    for (;;) {
        // Read from standard input and stop on error or no data.
        P_PACKET p;
        bSuccess = ReadFile(hStdin, p.buffer, sizeof(PACKET), &dwRead, NULL);
        if (p.p.type == REQ_TYPE::INIT) {
            B_INIT dat;
            memcpy(&dat, p.p.body, sizeof(B_INIT));
            e->addObject(p.p.id, dat);
        }

        //if (!bSuccess)
            // listening
    }
    return 0;
}