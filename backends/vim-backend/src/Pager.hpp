#ifndef PAGER_H_
#define PAGER_H_

#include <string>
#include <iostream>
#include "CursesWindow.hpp"
#include "InputReader.hpp"

namespace backends
{

class Pager
{
public:
    Pager() : textWindow(LINES-1, COLS, 0, 0, true), statusWindow(true) {}

    bool show()
    {
        bool resized = false;
        pos = 0;

        textWindow.focus();
        statusWindow.focus();
        resize();

        while (true)
        {
            updateStatus();
            write();

            CursesWindow::update();
            std::string input = InputReader::read();

            if (input == InputReader::resizeKey)
            {
                resized = true;
                resize();
            }

            if (input == "q" or input == "<ESC>")
                break;
            if (input == "k" or input == "<UARROW>")
            {
                if (pos > 0)
                    pos--;
            }
            else if ((input == "j" or input == "<DARROW>") && pos < maxPos)
                pos++;
        }

        textWindow.hide();
        statusWindow.hide();

        return resized;
    }

    void updateText(const std::string& newText)
    {
        text = newText;
        if (text.back() != '\n')
            text += '\n';
        makeLines();
    }

private:
    void resize()
    {
        if (oldLINES != LINES || oldCOLS != COLS)
        {
            oldLINES = LINES;
            oldCOLS = COLS;
            makeLines();
            textWindow.resize(LINES-1, COLS);
        }
        statusWindow.fixAfterResize();
    }
    void updateStatus()
    {
        if(maxPos == 0)
        {
            static const std::string base = "*** quit with q ***";
            statusWindow.print(base + std::string(std::max(0, int(COLS - base.size())), ' '));
        }
        else
        {
            static const std::string base = "*** scroll with j/k or arrows, quit with q/ESC ***";
            std::string counter;
            if (pos == maxPos)
                counter = "[BOTTOM]";
            else if (pos == 0)
                counter = "[TOP]";
            else
                counter = "[" + std::to_string(pos + 1) + "/" + std::to_string(maxPos + 1) + "]";

            statusWindow.print(base + std::string(std::max(0, int(COLS - base.size() - counter.size())), ' ') + counter);
        }
    }
    void write()
    {
        textWindow.clear();
        for(size_t i = 0; i < size_t(LINES - 1) && i + pos < lines.size(); i++)
            textWindow.print(lines[i + pos], i, 0);
    }
    void makeLines()
    {
        lines = { "\t" };

        size_t i = 0;
        while (i < text.size())
        {
            size_t next = std::min(text.find('\n', i), text.find(' ', i));
            if (text[next] == '\n')
            {
                if (size_t(COLS) >= lines.back().size() + (next - i))
                    lines.back() += text.substr(i, next-i);
                else
                    lines.push_back(text.substr(i, next-i));
                lines.push_back("\t");
            }
            else
            {
                if (size_t(COLS) >= lines.back().size() + (next - i))
                {
                    lines.back() += text.substr(i, next-i);
                    if (lines.back().size() != size_t(COLS))
                        lines.back() += " ";
                }
                else
                    lines.push_back(text.substr(i, next-i));
            }
            i = next + 1;
        }

        if (lines.back() == "\t")
            lines.pop_back();


        maxPos = std::max(0, int(lines.size() - LINES + 1));
    }

    int pos = 0;
    int maxPos = 0;


    std::string text;
    std::vector<std::string> lines;

    CursesWindow textWindow;
    StatusWindow statusWindow;

    int oldLINES = -1, oldCOLS = -1;
};



} // namespace backends
#endif

