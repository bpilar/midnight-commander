#ifndef COMMAND_H_
#define COMMAND_H_

#include <functional>
#include <string>
#include <utility>
#include <algorithm>

namespace backends
{

class Command
{
public:
    std::string getHelpText() const
    {
        return getPrintableCommand() + " - " + comment + '\n';
    }

    const std::function<void()>& getCallback() const
    {
        return callback;
    }

protected:
    Command(std::function<void()> _callback, std::string _comment)
        : callback(_callback), comment(_comment) {}

    virtual std::string getPrintableCommand() const = 0;

    std::function<void()> callback;
    std::string comment;
};



} // namespace
#endif
