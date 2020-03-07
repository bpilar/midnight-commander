#ifndef STATUSWINDOW_H_
#define STATUSWINDOW_H_

#include <string>
#include <ncurses.h>
#include <panel.h>

#include "CursesWindow.hpp"

namespace backends
{

class StatusWindow
{
public:
    StatusWindow(bool hidden = false)
        : cursesWindow(1, COLS, LINES-1, 0, hidden)
    {
        moveToBottom();
        if (hidden)
            hide();
    };

    void print(const std::string& s)
    {
        text = s;
        windowPrint();
    }

    void clear()
    {
        cursesWindow.clear();
    }

    void fixAfterResize()
    {
        cursesWindow.resize(1, COLS);
        cursesWindow.move(LINES - 1, 0);
        windowPrint();
    }

    void focus()
    {
        cursesWindow.focus();
    }
    void moveToBottom()
    {
        cursesWindow.moveToBottom();
    }
    void hide()
    {
        cursesWindow.hide();
    }

private:
    CursesWindow cursesWindow;
    std::string text;

    void windowPrint()
    {
        cursesWindow.clear();
        cursesWindow.print(text, 0, 0);
    }
};



} // namespace backends
#endif

