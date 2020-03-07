#ifndef TEXTCALL_H_
#define TEXTCALL_H_

#include <functional>
#include <string>
#include <utility>
#include <algorithm>

namespace backends
{

class TextCall
{
public:
    const std::string& getBase() const
    {
        return base;
    }

    const std::vector<std::string> getArguments() const
    {
        return arguments;
    }

    TextCall(std::string callString)
    {
        size_t p = std::min(callString.size(), findNotEscaped(callString, ' '));
        base = callString.substr(0, p);
        removeEscapes(base);

        while (p < callString.size())
        {
            if (callString[p] == ' ')
                p++;
            else if (callString[p] == '\'')
            {
                size_t endBracket = findNotEscaped(callString, '\'', p+1);
                if (endBracket == std::string::npos)
                    throw std::invalid_argument("No \' closing argument");
                arguments.emplace_back(callString.substr(p + 1, endBracket - p - 1));
                p = endBracket + 1;
            }
            else
            {
                size_t endSpace = std::min(callString.size(), findNotEscaped(callString, ' ', p));
                arguments.emplace_back(callString.substr(p, endSpace - p));
                p = endSpace + 1;
            }
        }

        for(auto& s : arguments)
            removeEscapes(s);
    }

private:
    std::string base;
    std::vector<std::string> arguments;

    static size_t findNotEscaped(const std::string& s, char c, size_t pos = 0)
    {
        while (pos < s.size())
        {
            if (s[pos] == '\\')
                pos += 2;
            else
            {
                if (s[pos] == c)
                    return pos;
                pos++;
            }
        }
        return std::string::npos;
    }
    static void removeEscapes(std::string &s)
    {
        size_t i = 0;
        size_t j = 0;
        while (j < s.size())
        {
            s[i] = s[j];
            if (s[i] == '\\')
                s[i] = s[++j];
            i++;
            j++;
        }
        s.erase(i);
    }
};



} // namespace
#endif
