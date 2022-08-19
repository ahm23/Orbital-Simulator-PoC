#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "engine.h"

#include "../commons/ipc.h"

int main() {
    DWORD dwRead, dwWritten;
    HANDLE hStdin, hStdout;
    BOOL bSuccess;
    std::condition_variable_any cv;
    std::shared_mutex mut;
    std::atomic<int> status = 0;
    std::cout << "Running." << "\n";

    const int threadCount = 3;

    ENGINE* e = new ENGINE(threadCount, &mut, &cv, &status);      // potential to create more per system
    std::cout << "Nice." << "\n";
    while (status != threadCount);
    e->toggle = true;
    cv.notify_all();


    int counter = 0;


    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hStdin = GetStdHandle(STD_INPUT_HANDLE);

    if (hStdout == INVALID_HANDLE_VALUE || hStdin == INVALID_HANDLE_VALUE)
        ExitProcess(1);

    for (;;) {
        // Read from StdIN
        P_PACKET p;
        bSuccess = ReadFile(hStdin, p.buffer, sizeof(PACKET), &dwRead, NULL);
        // error handle
        if (p.p.type == REQ_TYPE::INIT) {
            B_INIT dat;
            memcpy(&dat, p.p.body, sizeof(B_INIT));
            e->addObject(p.p.id, dat);
            P_PACKET r = {
                p.p.id, REQ_TYPE::RES, 200
            };
            bSuccess = WriteFile(hStdout, r.buffer, sizeof(PACKET), &dwWritten, NULL);
            counter++;
        }
        if (p.p.type == REQ_TYPE::GET) {
            B_GET dat;
            memcpy(&dat, p.p.body, sizeof(B_GET));
            object obj = e->getObject(p.p.id);
            P_RES response = { 
                obj.time, obj.p, obj.v 
            };
            P_PACKET r = {
                p.p.id, REQ_TYPE::RES, 200
            };
            memcpy(&r.p.body, response.buffer, sizeof(response.buffer));
            bSuccess = WriteFile(hStdout, r.buffer, sizeof(PACKET), &dwWritten, NULL);
            // error handle
        }
    }
    return 0;
}