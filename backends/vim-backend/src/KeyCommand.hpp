#ifndef KEYCOMMAND_H_
#define KEYCOMMAND_H_

#include <functional>
#include <string>
#include <utility>
#include <algorithm>

#include "Command.hpp"

namespace backends
{

class KeyCommand : public Command
{
public:
    bool isCall(const std::string& buffer) const
    {
        return buffer.size() >= keys.size()
            && std::equal(keys.begin(), keys.end(), buffer.end() - keys.size());
    }

    void operator()() const
    {
        callback();
    }

    std::string getKeys() const
    {
        return keys;
    }
    std::string getPrintableCommand() const override
    {
        return keys;
    }

private:
    std::string keys;

    friend class CommandCreator;
    KeyCommand(std::string _keys, std::function<void()> callback, std::string comment)
        : Command(callback, comment), keys(_keys) {}
};



} // namespace
#endif
