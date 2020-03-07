#ifndef CURSESWINDOW_H_
#define CURSESWINDOW_H_

#include <string>
#include <ncurses.h>
#include <panel.h>

namespace backends
{

class CursesWindow
{
public:
    CursesWindow(int linesC, int colsC, int posY, int posX, bool hidden = false)
    {
        windowPtr = newwin(linesC, colsC, posY, posX);
        panelPtr= new_panel(windowPtr);
        if (hidden)
            hide();
    }
    CursesWindow() : CursesWindow(0, 0, 0, 0, false) {};
    CursesWindow(CursesWindow&& other) : windowPtr(other.windowPtr),
        panelPtr(other.panelPtr), isHidden(other.isHidden)
    {
        other.panelPtr = nullptr;
        other.windowPtr = nullptr;
    }
    ~CursesWindow()
    {
        if (panelPtr != nullptr)
        {
            del_panel(panelPtr);
            delwin(windowPtr);
        }
    }

    static void update()
    {
        update_panels();
        doupdate();
    }

    void print(const std::string &s)
    {
        wprintw(windowPtr, s.c_str());
    }
    void print(const std::string &s, int y, int x)
    {
        mvwprintw(windowPtr, y, x, s.c_str());
    }
    void moveCursor(int y, int x)
    {
        wmove(windowPtr, y, x);
    }
    void clear()
    {
        werase(windowPtr);
    }

    void move(int y, int x)
    {
        move_panel(panelPtr, y, x);
    }
    void resize(int y, int x)
    {
        wresize(windowPtr, y, x);
        replace_panel(panelPtr, windowPtr);
    }

    void focus()
    {
        if (isHidden)
        {
            isHidden = false;
            show_panel(panelPtr);
        }
        else
            top_panel(panelPtr);
    }
    void moveToBottom()
    {
        if (isHidden)
        {
            show_panel(panelPtr);
            isHidden = false;
        }
        bottom_panel(panelPtr);
    }
    void hide()
    {
        moveToBottom();
        hide_panel(panelPtr);
        isHidden = true;
    }

private:
    WINDOW* windowPtr;
    PANEL* panelPtr;

    bool isHidden = false;
};



} // namespace backends
#endif

