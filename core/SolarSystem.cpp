#include "SolarSystem.h"
#include "celestial/Moon.h"
#include <yaml-cpp/yaml.h>
#include <Windows.h>
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>

#define BUFSIZE 4096 

long SolarSystem::update_freq = 1000;

void ErrorExit(PTSTR lpszFunction)

// Format a readable error message, display a message box, 
// and exit from the application.
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(1);
}



SolarSystem::SolarSystem() {

	loadStars();
    loadPlanets();
    loadMoons();

    for (int i = 0; i < iBuffer_star.size(); i++) {
        //initializePlanetaryOrbit(i + 1);
        initializeMechanics(i, iBuffer_star[i], STAR);
    }

    for (int i = 0; i < iBuffer_planet.size(); i++) {
        //initializePlanetaryOrbit(i + 1);
        initializeMechanics(i, iBuffer_planet[i], PLANET);
    }

    for (int i = 0; i < iBuffer_moon.size(); i++) {
        //initializePlanetaryOrbit(i + 1);
        initializeMechanics(i, iBuffer_moon[i], MOON);
    }

    mapSystem();
    startEngine();
    ke = new KinematicEngine(&elements, 1, &kinematic_m, &kinematic_cv);

    std::lock_guard<std::shared_mutex> lk(kinematic_m);
    toggle_kinematic = true;
    kinematic_cv.notify_all();
}

std::vector<Element*> SolarSystem::loadStars() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\stars.json");
    std::vector<Element*> starList;
    int start = (int)elements.size();
    int count = parser.parseObjects<Element, Star>(&elements);
    for (int i = start; i < count; i++) {
        iBuffer_star.push_back(i);
        starMap[elements[i]->obj->getName()] = elements[i]->obj->getID() - 1;
    }
    return starList;
}

std::vector<Element*> SolarSystem::loadPlanets() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\planets.json");
    std::vector<Element*> planetList;
    int start = (int)elements.size();
    int count = parser.parseObjects<Element, Planet>(&elements);
    for (int i = start; i < start + count; i++) {
        iBuffer_planet.push_back(i);
        planetMap[elements[i]->obj->getName()] = elements[i]->obj->getID() - 1;
    }
    return planetList;
}

std::vector<Element*> SolarSystem::loadMoons() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\moons.json");
    std::vector<Element*> moonList;
    int start = (int)elements.size();
    int count = parser.parseObjects<Element, Moon>(&elements);
    for (int i = start; i < start + count; i++) {
        iBuffer_moon.push_back(i);
        moonMap[elements[i]->obj->getName()] = elements[i]->obj->getID() - 1;
    }
    return moonList;
}

std::vector<Element*> SolarSystem::loadSattelites() {
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\core\\Sattelites.dat");
    std::vector<Element*> satteliteList;
    parser.parseObjects<Element, Sattelite>(&elements);
    return satteliteList;// INCOMPLETE
}

void SolarSystem::initializeMechanics(int index, int num, ObjectTypes type) {
    Element* el;
    std::string ref_object;
    Eigen::Vector3d Position, Velocity;
    int ref_type;
    
    FileParser parser("C:\\Users\\netagive\\Desktop\\Orbital\\" + (std::string)filenames[type - 1]);

    switch (type) {
    case STAR:
        el = elements[num];
        break;
    case PLANET:
        el = elements[num];
        break;
    default:
        el = elements[num];
        break;
    }
    OrbitInit init = parser.parseOrbit(index, el->obj, &ref_type, &ref_object, index);
    if (ref_type == ObjectTypes::BARYCENTRE) {
        init.init_mu = 0;
        el->anchor = NULL;
    }
    else {
        el->anchor = getObjectFromName((ObjectTypes)ref_type, ref_object);
        init.init_mu = calculate_mu(el->obj->getMass(), el->anchor->getMass());
    }
    el->obj->setMu(init.init_mu);
    switch (init.type) {
        case 0:
            el->obj->orbit.initOrbitCOE_TA(init, &Position, &Velocity);
            break;
        case 1:
            el->obj->orbit.initOrbitCOE_R(init, &Position, &Velocity);
            break;
        case 2:
            el->obj->orbit.initOrbitTLE(init, &Position, &Velocity); //TODO
            break;
        case 3:
            el->obj->orbit.initOrbitCOE_ML(init, &Position, &Velocity);
            break;
    }

    el->obj->setPos(Position);
    el->obj->setVel(Velocity);

    DWORD dwRead, dwWritten;
    BOOL bSuccess = FALSE;

    bSuccess = WriteFile(StdIN_W, "test", BUFSIZE, &dwWritten, NULL);
    if (!bSuccess) std::cout << "b";
    /*
    if (el->kinematic->initKinematicProcess(Position, Velocity)) {
        std::std::cout << "\033[0;32;49mSUCCESS: Initialized Kinematic Process for Object: " << el->obj->getName() << "\033[0m" << std::endl;
    }
    else {
        std::std::cout << "WARN: Attempt to Initialize Kinematic Process Failed for Object: " << el->obj->getName() << std::endl;
        std::std::cout << "Reason | Kinematic Process Already Initialized" << std::endl;
    }*/ 
}


void SolarSystem::mapSystem() {
    for (int i = 0; i < elements.size(); i++) {
        Element* el = elements[i];
        Element* el_tmp = el;
        int iterator = 0;
        CelestialBody* next_anchor;
        while (iterator > -1) {
            el->depth_map[iterator] = (CelestialBody*)el_tmp->obj;
            el->depth_map_reverse[(CelestialBody*)el_tmp->obj] = iterator;
            next_anchor = (CelestialBody*)el_tmp->anchor;      /// might revert to Objects & cast to clst in utils_planetary
            if (!next_anchor || next_anchor == NULL)
                break;
            el_tmp = getElementFromName(next_anchor->getType(), next_anchor->getName());
            iterator++;
        }
    }
}

void SolarSystem::start() {

}


Object* SolarSystem::getObjectFromName(ObjectTypes type, std::string name) {
    switch (type) {
    case STAR:
        return elements[starMap[name]]->obj;
    case PLANET:
        return elements[planetMap[name]]->obj;
    case MOON:
        return elements[moonMap[name]]->obj;
    default:
        return nullptr;
    }
}

Element* SolarSystem::getElementFromName(ObjectTypes type, std::string name) {
    switch (type) {
    case STAR:
        return elements[starMap[name]];
    case PLANET:
        return elements[planetMap[name]];
    case MOON:
        return elements[moonMap[name]];
    default:
        return nullptr;
    }
}


void SolarSystem::startEngine() {


    HANDLE g_hInputFile = NULL;

    SECURITY_ATTRIBUTES saAttr;

    // Set the bInheritHandle flag so pipe handles are inherited. 

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's STDOUT. 
    if (!CreatePipe(&StdOUT_R, &StdOUT_R, &saAttr, 0))
        std::cout << "err";
        //ErrorExit(TEXT("StdoutRd CreatePipe"));

    // Ensure the read handle to the pipe for STDOUT is not inherited.
    if (!SetHandleInformation(StdOUT_R, HANDLE_FLAG_INHERIT, 0))
        std::cout << "err";
        //ErrorExit(TEXT("Stdout SetHandleInformation"));

    // Create a pipe for the child process's STDIN. 
    if (!CreatePipe(&StdIN_R, &StdIN_W, &saAttr, 0))
        std::cout << "err";
        //ErrorExit(TEXT("Stdin CreatePipe"));

    // Ensure the write handle to the pipe for STDIN is not inherited. 
    if (!SetHandleInformation(StdIN_W, HANDLE_FLAG_INHERIT, 0))
        std::cout << "err";
        //ErrorExit(TEXT("Stdin SetHandleInformation"));

    // Create the child process. 
    //CreateChildProcess();

    TCHAR szCmdline[] = TEXT("child");
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

    // Set up members of the PROCESS_INFORMATION structure. 

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    // Set up members of the STARTUPINFO structure. 
    // This structure specifies the STDIN and STDOUT handles for redirection.

    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = StdOUT_W;
    siStartInfo.hStdOutput = StdOUT_W;
    siStartInfo.hStdInput = StdIN_R;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // Create the child process. 

    bSuccess = CreateProcess(
        "C:\\Users\\netagive\\Desktop\\Orbital\\x64\\Release\\KinematicEngine.exe",
        NULL,     // command line 
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        0,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION 

     // If an error occurs, exit the application. 
    if (!bSuccess)
        std::cout << "dead";
        //ErrorExit(TEXT("CreateProcess"));
    else {
        // Close handles to the child process and its primary thread.
        // Some applications might keep these handles to monitor the status
        // of the child process, for example. 

        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);

        // Close handles to the stdin and stdout pipes no longer needed by the child process.
        // If they are not explicitly closed, there is no way to recognize that the child process has ended.

        CloseHandle(StdOUT_W);
        CloseHandle(StdIN_R);
    }

    // Write to the pipe that is the standard input for a child process. 
    // Data is written to the pipe's buffers, so it is not necessary to wait
    // until the child process is running before writing data.

    //WriteToPipe();
    printf("\n->Contents of written to child STDIN pipe.\n");

    // Read from pipe that is the standard output for child process. 

    printf("\n->Contents of child process STDOUT:\n\n");
    //ReadFromPipe();

    printf("\n->End of parent execution.\n");

    // The remaining open handles are cleaned up when this process terminates. 
    // To avoid resource leaks in a larger application, close handles explicitly. 

}