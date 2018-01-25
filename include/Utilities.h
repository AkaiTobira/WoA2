#pragma once
#define RUNNING_LINUX


//#define DEBUGG_RUN
//#define SDR_RUN
#define CREATE_RUN

#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

namespace Utils
{

inline void ReadQuotedString(
    std::stringstream& l_stream,
    std::string& l_string)
{
    l_stream >> l_string;
    if (l_string.at(0) == '"'){
        while (l_string.at(l_string.length() - 1) != '"' ||!l_stream.eof()){
            std::string str;
            l_stream >> str;
            l_string.append(" " + str);
        }
    }
    l_string.erase(std::remove(l_string.begin(),l_string.end(), '"'), l_string.end());
}


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
