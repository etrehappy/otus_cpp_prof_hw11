#include "settings.h"

void logger::ErrorException(const std::string& error, const std::string& func_name)
{
    //std::cerr << "Exсeption " << func_name << " " << error << std::endl;
    std::wcout << L"Exception " << std::wstring(func_name.begin(), func_name.end()) << L" " << std::wstring(error.begin(), error.end()) << std::endl;
}

