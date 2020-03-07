#ifndef KEYCOMMANDCONTAINER_H_
#define KEYCOMMANDCONTAINER_H_

#include <functional>
#include <string>
#include <utility>
#include <algorithm>
#include <set>

#include "KeyCommand.hpp"

namespace backends
{

class KeyCommandContainer
{
public:
    void add(KeyCommand command)
    {
        commandSet.emplace(std::move(command));
    }
    void readBuffer(std::string& buffer)
    {
        for(const auto& c : commandSet)
            if(c.isCall(buffer))
            {
                buffer.clear();
                c();
                return;
            }
    }
    std::string getHelpText()
    {
        std::string temp;
        for(auto &i : commandSet)
            temp += i.getHelpText();
        return temp;
    }

private:
    //TODO more efficient data structure?
    struct commandCmp
    {
        bool operator()(const KeyCommand& a, const KeyCommand& b) const
        {
            return a.getKeys().size() > b.getKeys().size();
        }
    };
    std::multiset<KeyCommand, commandCmp> commandSet;
};


} // namespace
#endif
