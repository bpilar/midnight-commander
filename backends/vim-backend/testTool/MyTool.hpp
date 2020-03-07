#ifndef EXAMPLETOOL_H_
#define EXAMPLETOOL_H_

#include <map>
#include <cassert>
#include <ncurses.h>
#include <panel.h>

#include "ToolBase.hpp"
#include "BackendBase.hpp"

namespace tools
{

class MyTool : public ToolBase
{
public:
    MyTool()
    {
        setEntry("IS_SAVED", "NO");
    }
    ~MyTool()
    {
        if (window != nullptr)
        {
            del_panel(panel);
            delwin(window);
        }
    }
    void setEntry(const std::string &key, const std::string &value) noexcept override
    {
        entries[key] = value;
    }
    std::string getEntry(const std::string &key) noexcept override
    {
        return entries[key];
    }
    void setCoordinates(int width, int height, int startx, int starty) noexcept override
    {
        if (window != nullptr)
        {
            del_panel(panel);
            delwin(window);
        }

        window = newwin(width, height, startx, starty);
        panel = new_panel(window);

        printText();
    }
    void setBackend(backends::BackendBase &newBackend) override
    {
        ToolBase::setBackend(newBackend);

        newBackend.bind("<EDITION>", [this]() {
            std::string temp = getEntry("KEY");

            if (temp == "<DEL>")
            {
                if (!text.empty())
                    text.pop_back();
            }
            else
                text.insert(text.end(), temp.begin(), temp.end());

            printText();
        },
                        "");
        newBackend.bind("open", [this](){ text += "open"; printText(); }, "");
        // newBackend.bind("i", [this](){ text += "iii"; printText(); }, "");
        newBackend.bind("e!EDIT", [this](){}, "");
        newBackend.bind("#vim#bbdd!EDIT", [this](){ text += "aadd"; printText(); }, "");
        newBackend.bind("#not-vim#eee", [this](){ text += "ERROR"; printText(); }, "");
        newBackend.bind("qqq!EDIT\%idk", [this](){ text += "qqq"; printText(); }, "");
        newBackend.bind(":insert ${text}!EDIT\%idk", [this](){ text += getEntry("text"); printText(); }, li);
        newBackend.bind(":insert2 ${text} ${text2}\%idk", [this](){ text += "1. \"" + getEntry("text") + "\" 2. \"" + getEntry("text2") + "\"\n" ; printText(); }, "");
        newBackend.bind(":!EDIT\%idk", [this](){ text += getEntry("text"); printText(); }, "");
    }

private:
    std::map<std::string, std::string> entries;
    WINDOW *window = nullptr;
    PANEL *panel = nullptr;
    std::string text;

    void printText()
    {
        werase(window);
        mvwprintw(window, 0, 0, text.c_str());
    }

    std::string li = 
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam ut neque eu turpis hendrerit ultrices sit amet porta nulla. Morbi molestie dictum leo vel hendrerit. Sed sed libero orci. Nullam bibendum, nulla quis tempor volutpat, nisi nulla rutrum justo, ut consequat nunc ex vel urna. In placerat sodales vestibulum. Nam egestas cursus neque quis dictum. Sed viverra egestas porttitor. Ut congue sagittis efficitur.\n"
"Ut mollis erat ut lacus dictum, eu faucibus turpis pellentesque. Praesent ut turpis sed ante bibendum aliquet. Cras tempor ultricies elit, vel lacinia elit convallis eget. Aenean ultrices, nisi vehicula sollicitudin ullamcorper, tellus diam vehicula ante, vitae tincidunt est lorem et nunc. Aenean quis massa a nisl aliquet viverra. Curabitur ornare eu mauris at varius. Pellentesque egestas lacus quis lobortis eleifend. Proin augue justo, malesuada nec mi sed, sollicitudin scelerisque leo. Vestibulum pharetra nibh at erat dictum, et congue ex tristique. Nulla eu nunc scelerisque, consequat urna ut, pretium quam. Ut sit amet mattis purus. Aliquam et tincidunt turpis. Fusce eleifend molestie enim et faucibus.\n"
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec et justo hendrerit, laoreet diam vel, posuere risus. Nam ac convallis purus, quis rutrum risus. Aenean quis vulputate risus, in scelerisque nunc. Fusce eu mattis velit. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Duis vel mattis augue. Proin congue maximus tristique. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Fusce congue ullamcorper massa vitae ullamcorper. Duis elementum massa ac eros finibus elementum.\n"
"Vivamus placerat bibendum lorem, ut vulputate lacus ullamcorper vitae. Phasellus sodales lorem a felis laoreet, sit amet semper ante scelerisque. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Cras vehicula tortor eu porta condimentum. Sed sed augue eu est tristique gravida eu quis dolor. Duis quis nulla vitae arcu porta malesuada vel nec mauris. Donec suscipit libero id orci consectetur ultrices. Cras id accumsan justo, id finibus est. Fusce iaculis, enim non eleifend pharetra, neque nibh tristique ex, vel pellentesque augue urna eget arcu.\n"
"Suspendisse potenti. Fusce aliquam varius leo in laoreet. Mauris tincidunt magna elit, sit amet pretium libero congue et. Quisque ac orci leo. Mauris ac ultricies ante, in pulvinar ipsum. Aenean erat leo, convallis in ex imperdiet, porta auctor felis. Nullam vehicula eget mi eu iaculis. Sed aliquam, odio et dapibus sollicitudin, tellus nunc auctor nunc, et vestibulum massa massa nec ipsum. Integer malesuada magna erat, at volutpat orci luctus eu. Duis sodales mauris nibh, eget faucibus elit malesuada nec. Vivamus et dictum ante, ac lacinia ipsum.\n"
"Vivamus placerat bibendum lorem, ut vulputate lacus ullamcorper vitae. Phasellus sodales lorem a felis laoreet, sit amet semper ante scelerisque. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Cras vehicula tortor eu porta condimentum. Sed sed augue eu est tristique gravida eu quis dolor. Duis quis nulla vitae arcu porta malesuada vel nec mauris. Donec suscipit libero id orci consectetur ultrices. Cras id accumsan justo, id finibus est. Fusce iaculis, enim non eleifend pharetra, neque nibh tristique ex, vel pellentesque augue urna eget arcu.\n"
"Suspendisse potenti. Fusce aliquam varius leo in laoreet. Mauris tincidunt magna elit, sit amet pretium libero congue et. Quisque ac orci leo. Mauris ac ultricies ante, in pulvinar ipsum. Aenean erat leo, convallis in ex imperdiet, porta auctor felis. Nullam vehicula eget mi eu iaculis. Sed aliquam, odio et dapibus sollicitudin, tellus nunc auctor nunc, et vestibulum massa massa nec ipsum. Integer malesuada magna erat, at volutpat orci luctus eu. Duis sodales mauris nibh, eget faucibus elit malesuada nec. Vivamus et dictum ante, ac lacinia ipsum.\n"
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam ut neque eu turpis hendrerit ultrices sit amet porta nulla. Morbi molestie dictum leo vel hendrerit. Sed sed libero orci. Nullam bibendum, nulla quis tempor volutpat, nisi nulla rutrum justo, ut consequat nunc ex vel urna. In placerat sodales vestibulum. Nam egestas cursus neque quis dictum. Sed viverra egestas porttitor. Ut congue sagittis efficitur.\n"
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam ut neque eu turpis hendrerit ultrices sit amet porta nulla. Morbi molestie dictum leo vel hendrerit. Sed sed libero orci. Nullam bibendum, nulla quis tempor volutpat, nisi nulla rutrum justo, ut consequat nunc ex vel urna. In placerat sodales vestibulum. Nam egestas cursus neque quis dictum. Sed viverra egestas porttitor. Ut congue sagittis efficitur.\n"
"Ut mollis erat ut lacus dictum, eu faucibus turpis pellentesque. Praesent ut turpis sed ante bibendum aliquet. Cras tempor ultricies elit, vel lacinia elit convallis eget. Aenean ultrices, nisi vehicula sollicitudin ullamcorper, tellus diam vehicula ante, vitae tincidunt est lorem et nunc. Aenean quis massa a nisl aliquet viverra. Curabitur ornare eu mauris at varius. Pellentesque egestas lacus quis lobortis eleifend. Proin augue justo, malesuada nec mi sed, sollicitudin scelerisque leo. Vestibulum pharetra nibh at erat dictum, et congue ex tristique. Nulla eu nunc scelerisque, consequat urna ut, pretium quam. Ut sit amet mattis purus. Aliquam et tincidunt turpis. Fusce eleifend molestie enim et faucibus.\n"
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec et justo hendrerit, laoreet diam vel, posuere risus. Nam ac convallis purus, quis rutrum risus. Aenean quis vulputate risus, in scelerisque nunc. Fusce eu mattis velit. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Duis vel mattis augue. Proin congue maximus tristique. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Fusce congue ullamcorper massa vitae ullamcorper. Duis elementum massa ac eros finibus elementum.\n"
"Vivamus placerat bibendum lorem, ut vulputate lacus ullamcorper vitae. Phasellus sodales lorem a felis laoreet, sit amet semper ante scelerisque. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Cras vehicula tortor eu porta condimentum. Sed sed augue eu est tristique gravida eu quis dolor. Duis quis nulla vitae arcu porta malesuada vel nec mauris. Donec suscipit libero id orci consectetur ultrices. Cras id accumsan justo, id finibus est. Fusce iaculis, enim non eleifend pharetra, neque nibh tristique ex, vel pellentesque augue urna eget arcu.\n"
"Suspendisse potenti. Fusce aliquam varius leo in laoreet. Mauris tincidunt magna elit, sit amet pretium libero congue et. Quisque ac orci leo. Mauris ac ultricies ante, in pulvinar ipsum. Aenean erat leo, convallis in ex imperdiet, porta auctor felis. Nullam vehicula eget mi eu iaculis. Sed aliquam, odio et dapibus sollicitudin, tellus nunc auctor nunc, et vestibulum massa massa nec ipsum. Integer malesuada magna erat, at volutpat orci luctus eu. Duis sodales mauris nibh, eget faucibus elit malesuada nec. Vivamus et dictum ante, ac lacinia ipsum.\n"
"Vivamus placerat bibendum lorem, ut vulputate lacus ullamcorper vitae. Phasellus sodales lorem a felis laoreet, sit amet semper ante scelerisque. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Cras vehicula tortor eu porta condimentum. Sed sed augue eu est tristique gravida eu quis dolor. Duis quis nulla vitae arcu porta malesuada vel nec mauris. Donec suscipit libero id orci consectetur ultrices. Cras id accumsan justo, id finibus est. Fusce iaculis, enim non eleifend pharetra, neque nibh tristique ex, vel pellentesque augue urna eget arcu.\n"
"Ut mollis erat ut lacus dictum, eu faucibus turpis pellentesque. Praesent ut turpis sed ante bibendum aliquet. Cras tempor ultricies elit, vel lacinia elit convallis eget. Aenean ultrices, nisi vehicula sollicitudin ullamcorper, tellus diam vehicula ante, vitae tincidunt est lorem et nunc. Aenean quis massa a nisl aliquet viverra. Curabitur ornare eu mauris at varius. Pellentesque egestas lacus quis lobortis eleifend. Proin augue justo, malesuada nec mi sed, sollicitudin scelerisque leo. Vestibulum pharetra nibh at erat dictum, et congue ex tristique. Nulla eu nunc scelerisque, consequat urna ut, pretium quam. Ut sit amet mattis purus. Aliquam et tincidunt turpis. Fusce eleifend molestie enim et faucibus.\n"
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec et justo hendrerit, laoreet diam vel, posuere risus. Nam ac convallis purus, quis rutrum risus. Aenean quis vulputate risus, in scelerisque nunc. Fusce eu mattis velit. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Duis vel mattis augue. Proin congue maximus tristique. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Fusce congue ullamcorper massa vitae ullamcorper. Duis elementum massa ac eros finibus elementum.\n"
"Vivamus placerat bibendum lorem, ut vulputate lacus ullamcorper vitae. Phasellus sodales lorem a felis laoreet, sit amet semper ante scelerisque. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Cras vehicula tortor eu porta condimentum. Sed sed augue eu est tristique gravida eu quis dolor. Duis quis nulla vitae arcu porta malesuada vel nec mauris. Donec suscipit libero id orci consectetur ultrices. Cras id accumsan justo, id finibus est. Fusce iaculis, enim non eleifend pharetra, neque nibh tristique ex, vel pellentesque augue urna eget arcu.\n"
"Suspendisse potenti. Fusce aliquam varius leo in laoreet. Mauris tincidunt magna elit, sit amet pretium libero congue et. Quisque ac orci leo. Mauris ac ultricies ante, in pulvinar ipsum. Aenean erat leo, convallis in ex imperdiet, porta auctor felis. Nullam vehicula eget mi eu iaculis. Sed aliquam, odio et dapibus sollicitudin, tellus nunc auctor nunc, et vestibulum massa massa nec ipsum. Integer malesuada magna erat, at volutpat orci luctus eu. Duis sodales mauris nibh, eget faucibus elit malesuada nec. Vivamus et dictum ante, ac lacinia ipsum.\n"
"Vivamus placerat bibendum lorem, ut vulputate lacus ullamcorper vitae. Phasellus sodales lorem a felis laoreet, sit amet semper ante scelerisque. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Cras vehicula tortor eu porta condimentum. Sed sed augue eu est tristique gravida eu quis dolor. Duis quis nulla vitae arcu porta malesuada vel nec mauris. Donec suscipit libero id orci consectetur ultrices. Cras id accumsan justo, id finibus est. Fusce iaculis, enim non eleifend pharetra, neque nibh tristique ex, vel pellentesque augue urna eget arcu.\n"
"Suspendisse potenti. Fusce aliquam varius leo in laoreet. Mauris tincidunt magna elit, sit amet pretium libero congue et. Quisque ac orci leo. Mauris ac ultricies ante, in pulvinar ipsum. Aenean erat leo, convallis in ex imperdiet, porta auctor felis. Nullam vehicula eget mi eu iaculis. Sed aliquam, odio et dapibus sollicitudin, tellus nunc auctor nunc, et vestibulum massa massa nec ipsum. Integer malesuada magna erat, at volutpat orci luctus eu. Duis sodales mauris nibh, eget faucibus elit malesuada nec. Vivamus et dictum ante, ac lacinia ipsum.\n"
"Suspendisse potenti. Fusce aliquam varius leo in laoreet. Mauris tincidunt magna elit, sit amet pretium libero congue et. Quisque ac orci leo. Mauris ac ultricies ante, in pulvinar ipsum. Aenean erat leo, convallis in ex imperdiet, porta auctor felis. Nullam vehicula eget mi eu iaculis. Sed aliquam, odio et dapibus sollicitudin, tellus nunc auctor nunc, et vestibulum massa massa nec ipsum. Integer malesuada magna erat, at volutpat orci luctus eu. Duis sodales mauris nibh, eget faucibus elit malesuada nec. Vivamus et dictum ante, ac lacinia ipsum.\n"
"Suspendisse potenti. Fusce aliquam varius leo in laoreet. Mauris tincidunt magna elit, sit amet pretium libero congue et. Quisque ac orci leo. Mauris ac ultricies ante, in pulvinar ipsum. Aenean erat leo, convallis in ex imperdiet, porta auctor felis. Nullam vehicula eget mi eu iaculis. Sed aliquam, odio et dapibus sollicitudin, tellus nunc auctor nunc, et vestibulum massa massa nec ipsum. Integer malesuada magna erat, at volutpat orci luctus eu. Duis sodales mauris nibh, eget faucibus elit malesuada nec. Vivamus et dictum ante, ac lacinia ipsum.\n"
"Suspendisse potenti. Fusce aliquam varius leo in laoreet. Mauris tincidunt magna elit, sit amet pretium libero congue et. Quisque ac orci leo. Mauris ac ultricies ante, in pulvinar ipsum. Aenean erat leo, convallis in ex imperdiet, porta auctor felis. Nullam vehicula eget mi eu iaculis. Sed aliquam, odio et dapibus sollicitudin, tellus nunc auctor nunc, et vestibulum massa massa nec ipsum. Integer malesuada magna erat, at volutpat orci luctus eu. Duis sodales mauris nibh, eget faucibus elit malesuada nec. Vivamus et dictum ante, ac lacinia ipsum.\n"
"Suspendisse potenti. Fusce aliquam varius leo in laoreet. Mauris tincidunt magna elit, sit amet pretium libero congue et. Quisque ac orci leo. Mauris ac ultricies ante, in pulvinar ipsum. Aenean erat leo, convallis in ex imperdiet, porta auctor felis. Nullam vehicula eget mi eu iaculis. Sed aliquam, odio et dapibus sollicitudin, tellus nunc auctor nunc, et vestibulum massa massa nec ipsum. Integer malesuada magna erat, at volutpat orci luctus eu. Duis sodales mauris nibh, eget faucibus elit malesuada nec. Vivamus et dictum ante, ac lacinia ipsum.\n"
"Suspendisse potenti. Fusce aliquam varius leo in laoreet. Mauris tincidunt magna elit, sit amet pretium libero congue et. Quisque ac orci leo. Mauris ac ultricies ante, in pulvinar ipsum. Aenean erat leo, convallis in ex imperdiet, porta auctor felis. Nullam vehicula eget mi eu iaculis. Sed aliquam, odio et dapibus sollicitudin, tellus nunc auctor nunc, et vestibulum massa massa nec ipsum. Integer malesuada magna erat, at volutpat orci luctus eu. Duis sodales mauris nibh, eget faucibus elit malesuada nec. Vivamus et dictum ante, ac lacinia ipsum.\n"
"Suspendisse potenti. Fusce aliquam varius leo in laoreet. Mauris tincidunt magna elit, sit amet pretium libero congue et. Quisque ac orci leo. Mauris ac ultricies ante, in pulvinar ipsum. Aenean erat leo, convallis in ex imperdiet, porta auctor felis. Nullam vehicula eget mi eu iaculis. Sed aliquam, odio et dapibus sollicitudin, tellus nunc auctor nunc, et vestibulum massa massa nec ipsum. Integer malesuada magna erat, at volutpat orci luctus eu. Duis sodales mauris nibh, eget faucibus elit malesuada nec. Vivamus et dictum ante, ac lacinia koniec.\n";

};

} // namespace tools
#endif
