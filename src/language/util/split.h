#pragma once
#include "imports.h"
#include "includes.h"
#include <sstream>

namespace Util
{
    inline std::vector<std::string> split(const std::string& str, const std::vector<char>& delimiters) {
        std::vector<std::string> splitStr;
        std::string currentString;
        
        for (const char c : str) {
            if (includes<char>(delimiters, c)) {
                if (currentString.length() > 0) {
                    splitStr.push_back(currentString);
                    currentString.clear();
                }
                continue;
            }

            currentString += c;
        }

        return splitStr;
    }

    inline std::vector<std::string> split(const std::string& str, const char delimiter) {
        std::vector<std::string> splitStr;
        std::string currentStr;
        std::stringstream strStream(str);

        while (std::getline(strStream, currentStr, delimiter)) {
            splitStr.push_back(currentStr);
        }

        return splitStr;
    }
}