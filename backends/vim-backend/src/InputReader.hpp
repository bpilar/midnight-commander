#ifndef INPUTREADER_H_
#define INPUTREADER_H_

#include <string>
#include <unordered_map>

#include <iostream>

#include <ncurses.h>

namespace backends
{

class InputReader
{
public:
    const static std::string resizeKey;
    static std::string read()
    {
        static const std::unordered_map<int, std::string> notStandard =
        {
            {27, "<ESC>"},
            {10, "<ENTER>"},
            {KEY_BACKSPACE, "<DEL>"},
            {KEY_UP, "<UARROW>"},
            {KEY_DOWN, "<DARROW>"},
            {KEY_LEFT, "<LARROW>"},
            {KEY_RIGHT, "<RARROW>"},
            {KEY_RESIZE, resizeKey}
        };

        int k = getch();
        // std::cerr << k << std::endl;
        // std::cerr << KEY_ENTER << std::endl;

        auto it = notStandard.find(k);
        if (it != notStandard.end())
            return it->second;

        std::string cursesName(keyname(k));

        if (cursesName.size() == 2 && cursesName[0] == '^')
            return "<CTRL>" + cursesName.substr(1);

        return cursesName;
    }
};

} // namespace backends
#endif

