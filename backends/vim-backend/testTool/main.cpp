
#include "MyTool.hpp"
#include "VimBackend.hpp"

int main()
{
    tools::MyTool tool;
    backends::VimBackend backend;
    backend.setTool(tool);
    tool.setBackend(backend);
    backend();
}
