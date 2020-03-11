#ifndef COMMANDER_H_
#define COMMANDER_H_

#include <map>
#include <vector>
#include <string>
#include <ncurses.h>
#include <panel.h>
#include "ToolBase.hpp"
#include "BackendBase.hpp"

namespace std
{
vector<string> split(string str, string token)
    {
        vector<string>result;
        while(str.size())
        {
            int index = str.find(token);
            if(index!=string::npos)
            {
                result.push_back(str.substr(0,index));
                str = str.substr(index+token.size());
                if(str.size()==0) result.push_back(str);
            }
            else
            {
                result.push_back(str);
                str = "";
            }
        }
        return result;
    }

string GetStdoutFromCommand(string cmd)
    {
        string data;
        FILE * stream;
        const int max_buffer = 256;
        char buffer[max_buffer];
        cmd.append(" 2>&1");

        stream = popen(cmd.c_str(), "r");
        if (stream)
        {
            while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
            pclose(stream);
        }
        if(data[data.length()-1]==10) data.erase(data.length()-1);
        return data;
    }

bool isDir(string path)
    {
        string cmd="[ -d \"";
        cmd+=path;
        cmd+="\" ] && echo \"YES\" || echo \"NO\"";

        std::string res=std::GetStdoutFromCommand(cmd);
        if(res=="YES") return true;
        else return false;
    }

string getPath(std::string path, std::string name)
{
    std::string result=path;
    result+="/";
    result+=name;
    for(int i=0; i<result.length(); i++) if(result[i]==10) result.erase(i);
    return result;
}

}

// namespace to made the code prettier (and prevent conflicts) 
namespace tools
{
class Commander : public ToolBase
{
public:
    Commander()
    {
        setEntry("IS_SAVED", "NO");
        path[0]=std::GetStdoutFromCommand("pwd");
        path[1]=std::GetStdoutFromCommand("pwd");
        for(int i=0; i<path[0].length(); i++) if(path[0][i]==10) path[0].erase(i);
        for(int i=0; i<path[1].length(); i++) if(path[1][i]==10) path[1].erase(i);
    }
    ~Commander()
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

        // redraw window's contents
        //printText();
        refresh();
    }
    void setBackend(backends::BackendBase &newBackend) override
    {
        // register backend
        // see ToolBase.hpp
        ToolBase::setBackend(newBackend);
        setEntry("IS_SAVED", "YES");
        setEntry("BATCH", "NO");
        // bind commands to backend
        // it is very important:
        // you should register all your commands here



        newBackend.bind("<UARROW>", [this](void) {
            upper();
        }, "Previous file.");
        newBackend.bind("<DARROW>", [this](void) {
            lower();
        }, "Next file.");
        newBackend.bind("<LARROW>", [this](void) {
            lefter();
        }, "Left window.");
        newBackend.bind("<RARROW>", [this](void) {
            righter();
        }, "Right window.");
        newBackend.bind("<ENTER>", [this](void) {
            goTo();
        }, "Go to directory or open file");

        newBackend.bind(":cp", [this](void){
            copy();
        }, "Copy selected file/dictionary to target dictionary.");
        newBackend.bind(":copy ${ARG}", [this](void){
            copy(getEntry("ARG"));
        }, "Copy selected file/dictionary to given destination.");
        newBackend.bind(":move", [this](void){
            move();
        }, "Moves selected file/dictionary to target dictionary.");
        newBackend.bind(":makedir ${ARG}", [this](void){
            makedir(getEntry("ARG"));
        }, "Creates new dictionary in selected destination with given name.");
        newBackend.bind(":remove", [this](void){
            remove();
        }, "Removes selected file/dictionary.");
        newBackend.bind(":find ${ARG}", [this](void){
            find(getEntry("ARG"));
        }, "Opens (first) dictionary containing file with given name in it.");

        newBackend.bind(":batch", [this](void){
            batchCommand.clear();
            mvwprintw(window, 0, 0, "batch mode");
            setEntry("BATCH", "YES");
        }, "Turn on batch mode.");
        newBackend.bind(":nbatch", [this](void){
            if(getEntry("IS_SAVED")=="YES")
            {
                setEntry("BATCH", "NO");
                mvwprintw(window, 0, 0, "standard mode");
            }
            else mvwprintw(window, 0, 0, "batch not saved");
        }, "Turn off batch mode.");
        newBackend.bind(":commit", [this](void){
            commitBatch();
            setEntry("IS_SAVED", "YES");
        }, "Commits current batch.");
        newBackend.bind(":abort", [this](void){
            abortBatch();
            setEntry("IS_SAVED", "YES");
        }, "Aborts current batch.");

        

        newBackend.bind(":refresh", [this](void){
            refresh();
        }   , "Refresh content.");
        newBackend.bind(":forget", [this](void){
            setEntry("IS_SAVED", "YES");
        }, "Forces backend to think, that file has been saved.");
    }

private:
    
    std::map<std::string, std::string> entries;

    WINDOW *window = nullptr;
    PANEL *panel = nullptr;
    std::string path[2];
    std::vector<std::string> content[2];
    int selected[2]={0,0};
    int selectedtab=0;
    std::string batchCommand;

    // std::string command;
    // std::string ls;
    // std::vector<std::string> vec;
    std::string pointing=" <-----";




    void upper()
    {
        selected[selectedtab]--;
        if(selected[selectedtab]<0) selected[selectedtab]+=content[selectedtab].size();
        refresh();
        return ;
    }
    void lower()
    {
        selected[selectedtab]++;
        if(selected[selectedtab]>=content[selectedtab].size()) selected[selectedtab]-=content[selectedtab].size();
        refresh();
        return ;
    }
    void lefter()
    {
        selectedtab=0;
        refresh();
        return ;
    }
    void righter()
    {
        selectedtab=1;
        refresh();
        return ;
    }
    void goTo()
    {
        if(content[selectedtab][selected[selectedtab]]==".") { refresh(); return ;}
        if(content[selectedtab][selected[selectedtab]]=="..")
        {
            std::string command="dirname ";
            command+=path[selectedtab];
            std::string ls=std::GetStdoutFromCommand(command);
            for(int i=0; i<ls.length(); i++) if(ls[i]==10) ls.erase(i);
            path[selectedtab]=ls;
        }
        else
        {
            std::string command=std::getPath(path[selectedtab],content[selectedtab][selected[selectedtab]]);
            if(std::isDir(command))
            {
                path[selectedtab]=command;
                selected[selectedtab]=0;
            }
            else
            {
                openFile(command);
            }
        }
        refresh();
        return ;
    }

    void copy(std::string dest)
    {
        if(selected[0]>1)
        {
            std::string command="cp -r ";
            command+=std::getPath(path[0],content[0][selected[0]]);
            command+=" ";
            command+=dest;
            if(getEntry("BATCH")=="NO") std::string ls=std::GetStdoutFromCommand(command);
            else
            {
                batchCommand+=command;
                batchCommand+="; ";
                setEntry("IS_SAVED", "NO");
            }
        }
        refresh();
        return ;
    }
    void copy()
    {
        copy(path[1]);
        return ;
    }
    void move()
    {
        if(selected[0]>1)
        {
            std::string command="mv ";
            command+=std::getPath(path[0],content[0][selected[0]]);
            command+=" ";
            command+=path[1];
            if(getEntry("BATCH")=="NO") std::string ls=std::GetStdoutFromCommand(command);
            else
            {
                batchCommand+=command;
                batchCommand+="; ";
                setEntry("IS_SAVED", "NO");
            }
        }
        refresh();
        return ;
    }
    void makedir(std::string name)
    {
        std::string command="mkdir ";
        command+=std::getPath(path[selectedtab],name);
        if(getEntry("BATCH")=="NO") std::string ls=std::GetStdoutFromCommand(command);
            else
            {
                batchCommand+=command;
                batchCommand+="; ";
                setEntry("IS_SAVED", "NO");
            }
        refresh();
        return ;
    }
    void remove()
    {
        std::string command="rm -r ";
        command+=std::getPath(path[selectedtab],content[selectedtab][selected[selectedtab]]);
        if(getEntry("BATCH")=="NO") std::string ls=std::GetStdoutFromCommand(command);
            else
            {
                batchCommand+=command;
                batchCommand+="; ";
                setEntry("IS_SAVED", "NO");
            }
        refresh();
        return ;
    }

    void find(std::string name)
    {
        std::string command="find ";
        command+=path[selectedtab];
        command+=" -name *";
        command+=name;
        command+="*";
        std::string ls=std::GetStdoutFromCommand(command);
        for(int i=0; i<ls.length();i++) if(ls[i]==10) ls[i]=' ';
        std::vector<std::string> vec=std::split(ls," ");
        if(vec.size()==0) {refresh(); return ;}
        command="dirname ";
        command+=vec[0];
        std::string dict=std::GetStdoutFromCommand(command);
        path[selectedtab]=dict;
        refresh();
        return ;
    }

    void commitBatch()
    {
        std::string ls=std::GetStdoutFromCommand(batchCommand);
        batchCommand.clear();
        refresh();
        return ;
    }
    void abortBatch()
    {
        batchCommand.clear();
        refresh();
        return ;
    }

    void openFile(std::string path)
    {
        //"urxvt -e vim main.cpp &"
        std::string editor="vim";
        std::string command="urxvt -e ";
        command+=editor;
        command+=" ";
        command+=path;
        command+=" &";
        std::string errr=std::GetStdoutFromCommand(command);
    }
    

    void refresh()
    {
        std::string command="ls -a ";
        command+=path[0];
        std::string ls=std::GetStdoutFromCommand(command);
        for(int i=0; i<ls.length(); i++) if(ls[i]==10) ls[i]=' ';
        std::vector<std::string> vec=std::split(ls," ");
        content[0]=vec;

        command="ls -a ";
        command+=path[1];
        ls=std::GetStdoutFromCommand(command);
        for(int i=0; i<ls.length(); i++) if(ls[i]==10) ls[i]=' ';
        vec=std::split(ls," ");
        content[1]=vec;
        printText();
        return ;
    }

    void printText()
    {
        werase(window);
        int maxY, maxX;
        getmaxyx(window, maxY, maxX);
        for(int i=0; i<content[0].size(); i++)
        {
            std::string ls=content[0][i];
            if(selected[0]==i) ls+=pointing;
            mvwprintw(window, i, 0, ls.c_str());
        }
        for(int i=0; i<content[1].size(); i++)
        {
            std::string ls=content[1][i];
            if(selected[1]==i) ls+=pointing;
            mvwprintw(window, i, maxX/2, ls.c_str());
        }
        wrefresh(window);
        return ;
        //mvwprintw(window, 0, 0, text.c_str());
    }
};
} // namespace tools
#endif /* !COMMANDER_H_ */
