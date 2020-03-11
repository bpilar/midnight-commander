
// Compile with: g++ main.cpp -lncurses -o example

#include <iostream>
#include "midnight-commander.hpp"
#include "VimBackend.hpp"

int main()
{
    tools::Commander tool;
    backends::VimBackend backend;
    backend.setTool(tool);
    tool.setBackend(backend);
    backend();
    std::cout<<"Koniec\n";
}
