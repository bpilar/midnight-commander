
#include "VimBackend.hpp"

#include <iostream>

#include "ToolBase.hpp"
#include "CursesWindow.hpp"
#include "StatusWindow.hpp"
#include "InputReader.hpp"
#include "Command.hpp"
#include "KeyCommand.hpp"
#include "CommandCreator.hpp"
#include "KeyCommandContainer.hpp"
#include "TextCommand.hpp"
#include "TextCommandContainer.hpp"
#include "VimBackend.hpp"
#include "Pager.hpp"


#include <ncurses.h>

namespace backends
{

struct VimBackend::Impl
{
    bool edition = false;
    bool text = false;

    bool tryQuit = false;

    std::function<void()> editionCallback;

    tools::ToolBase* tool;

    KeyCommandContainer keyCommandContainer;
    TextCommandContainer textCommandContainer;

    StatusWindow statusWindow;

    Pager helpPager;
    bool helpTextDirty = true;

    void showHelp()
    {
        if (helpTextDirty)
        {
            helpPager.updateText("HELP:\n\n" + keyCommandContainer.getHelpText() + textCommandContainer.getHelpText());
            helpTextDirty = false;
        }
        if (helpPager.show())
            resizeWindows();
    }

    void bind(const std::string &command, std::function<void()> callback, const std::string &comment)
    {
        auto commandPtr = CommandCreator::create(command, callback, comment, [this](){
            edition = true;
            statusWindow.print("* EDITION *");
        });
        if (commandPtr)
        {
            helpTextDirty = true;
            KeyCommand* keyPtr = dynamic_cast<KeyCommand*>(commandPtr.get());
            if (keyPtr)
            {
                if (keyPtr->getPrintableCommand() == "<EDITION>")
                    editionCallback = keyPtr->getCallback();
                else
                {
                    keyCommandContainer.add(std::move(*keyPtr));
                }
            }
            TextCommand* textPtr = dynamic_cast<TextCommand*>(commandPtr.get());
            if (textPtr)
            {
                textCommandContainer.add(std::move(*textPtr));
            }
        }
    }

    void resizeWindows()
    {
        statusWindow.fixAfterResize();
        statusWindow.moveToBottom();
        tool->setCoordinates(LINES-1, COLS, 0, 0);
        if (text)
            statusWindow.focus();
    }

    void run()
    {
        resizeWindows();
        std::string buffer;

        while (true)
        {
            CursesWindow::update();
            std::string key = InputReader::read();
            if (key == InputReader::resizeKey)
                resizeWindows();
            else if (edition)
            {
                if (key == "<ESC>")
                {
                    edition = false;
                    statusWindow.print("");
                }
                else
                {
                    tool->setEntry("KEY", key);
                    editionCallback();
                }
            }
            else if (key == ":")
            {
                buffer.clear();
                statusWindow.focus();
                statusWindow.print(":");
                text = true;
            }
            else if (text)
            {
                if (key == "<ENTER>")
                {
                    statusWindow.print("[" + buffer + "]");
                    statusWindow.moveToBottom();
                    CursesWindow::update();
                    if (!edition)
                        statusWindow.print(textCommandContainer.readBuffer(buffer, *tool));
                    buffer.clear(); 
                    text = false;
                }
                else if (key == "<ESC>")
                {
                    buffer.clear();
                    text = false;
                    statusWindow.clear();
                    statusWindow.moveToBottom();
                }
                else
                {
                    if (key == "<DEL>")
                    {
                        if (!buffer.empty())
                            buffer.pop_back();
                        statusWindow.print(":" + buffer);
                    }
                    else if (key.size() == 1)
                        buffer += key;
                    statusWindow.print(":" + buffer);
                }
            }
            else
            {
                buffer += key;
                keyCommandContainer.readBuffer(buffer);
            }
            if (tryQuit)
            {
                tryQuit = false;
                if (tool->getEntry("IS_SAVED") == "NO")
                    statusWindow.print("Cannot quit, file not saved");
                else
                    break;
            }
        }

    }
};

VimBackend::VimBackend()
{
    initscr();
    cbreak();
    keypad(stdscr, true);
    noecho();
    set_escdelay(25);

    pImpl = std::unique_ptr<Impl>(new Impl);

    pImpl->bind(":quit", [this]{ pImpl->tryQuit = true; }, "quit application");
    pImpl->bind(":help", [this]{ pImpl->showHelp(); }, "show help message");
}

VimBackend::~VimBackend()
{
    endwin();
}

void VimBackend::bind(const std::string &command, std::function<void()> callback, const std::string &helpMessage) noexcept
{
    pImpl->bind(command, callback, helpMessage);
}

void VimBackend::operator()()
{
    pImpl->run();
}

void VimBackend::setTool(tools::ToolBase& newTool)
{
    pImpl->tool = &newTool;
}

}
