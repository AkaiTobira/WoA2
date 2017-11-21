#pragma once
#define RUNNING_LINUX


//#define DEBUGG_RUN
//#define SDR_RUN
#define CREATE_RUN

#include <iostream>
#include <string>
#include <algorithm>

namespace Utils
{
#ifdef RUNNING_WINDOWS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <Shlwapi.h>
inline std::string GetWorkingDirectory()
{
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
    return std::string( buffer ).substr( 0, pos) + "/";

}
#elif defined RUNNING_LINUX
#include <unistd.h>
inline std::string GetWorkingDirectory()
{
    char cwd[1024];
    if(getcwd(cwd,sizeof(cwd)) != nullptr)
    {
        std::string pathing = std::string(cwd);

        #ifdef DEBUGG_RUN
            return pathing + "/bin/Debug/";
        #endif 
            return  pathing + "/bin/Debug/";
        
    }
    return "";
}
#endif
}
