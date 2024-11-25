#pragma once

#include <exception>
#include <string>
#include <iostream>

namespace settings
{
    const static unsigned short kBufferSize = 1024;    

}//settings

namespace logger
{
    void ErrorException(const std::string& error, const std::string& s);

}//logger

enum class ECommandStatus : uint16_t
{
    kSucces = 0
    , kSuccesIntersection
    , kSuccesSymmetricDifference

    , kError
    , kErrorTableNotFound
    , kErrorArgMissed
    , kErrorInsertIdExist 
};

enum class ECommand : uint16_t
{
    kNull = 0

    , kInsert
    , kTruncate
    , kIntersection
    , kSymmetricDifference
};

