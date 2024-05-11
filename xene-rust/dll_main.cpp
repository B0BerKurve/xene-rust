#include <Windows.h>
#include "includes.hpp"
#include "game/il2cpp.hpp"
#include "game/hooks.hpp"

BOOL CtrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
    case CTRL_C_EVENT:
        //std::cout << "Ctrl+C is disabled in this application." << std::endl;
        return TRUE;  // Return TRUE to indicate that you handled the signal
    default:
        return FALSE; // Return FALSE for other signals
    }
}

void CreateConsole()
{
    if (!AllocConsole()) {
        // Add error handling here.
        DWORD error = GetLastError();
        std::cerr << "Failed to allocate console. Error code: " << error << std::endl;
        return;
    }

    FILE* fDummy;
    if (freopen_s(&fDummy, "CONOUT$", "w", stdout) != 0) {
        std::cerr << "Failed to redirect stdout." << std::endl;
        return;
    }
    if (freopen_s(&fDummy, "CONOUT$", "w", stderr) != 0) {
        std::cerr << "Failed to redirect stderr." << std::endl;
        return;
    }
    if (freopen_s(&fDummy, "CONIN$", "r", stdin) != 0) {
        std::cerr << "Failed to redirect stdin." << std::endl;
        return;
    }
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) {
        std::cerr << "Failed to set Ctrl+C handler." << std::endl;
    }

    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    HANDLE hConOut = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hConIn = CreateFile("CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hConOut != INVALID_HANDLE_VALUE && hConIn != INVALID_HANDLE_VALUE) {
        SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
        SetStdHandle(STD_ERROR_HANDLE, hConOut);
        SetStdHandle(STD_INPUT_HANDLE, hConIn);

        std::wcout.clear();
        std::wclog.clear();
        std::wcerr.clear();
        std::wcin.clear();
    }
    else {
        std::cerr << "Failed to create console file handles." << std::endl;
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD callReason, LPVOID lpReserved)
{
    if (callReason == DLL_PROCESS_ATTACH)
    {
        CreateConsole();
        //MessageBox(NULL, "Fuck", "Fuck", MB_OK);
        il2cpp::init();
        hooks::init();
    }
    return TRUE;
}