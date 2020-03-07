#ifndef TEXTCOMMAND_H_
#define TEXTCOMMAND_H_

#include <functional>
#include <string>
#include <utility>
#include <algorithm>
#include <vector>
#include <cassert>
#include <stdexcept>


#include "Command.hpp"
#include "TextCall.hpp"

namespace backends
{

class TextCommand : public Command
{
public:

    void operator()(TextCall c, tools::ToolBase& tool)
    {
        for (size_t i = 0; i < arguments.size(); i++)
            tool.setEntry(arguments[i], c.getArguments()[i]);
        callback();
    }

    const std::string& getBase() const
    {
        return base;
    }
    const std::vector<std::string>& getArguments() const
    {
        return arguments;
    }


private:

    std::string base;
    std::vector<std::string> arguments;

    std::string printableCommand;

    friend class CommandCreator;
    TextCommand(std::string sequence, std::function<void()> callback, std::string comment)
        : Command(callback, comment), printableCommand(sequence)
    {
        assert(sequence[0] == ':');

        size_t p = std::min(sequence.size(), sequence.find(' '));

        if(p < 2)
            throw std::invalid_argument("No base");

        base = sequence.substr(1, p - 1);

        while (p < sequence.size())
        {
            if (sequence[p] == ' ')
                p++;
            else if (sequence[p] != '$')
                throw std::invalid_argument("Invalid format, something that is neither valid argument nor base");
            else
            {
                p += 2;
                size_t argend = sequence.find('}', p);
                if (argend == std::string::npos)
                    throw std::invalid_argument("Parameter with no closing bracket");
                arguments.emplace_back(sequence.substr(p, argend - p));
                p = argend + 1;
            }
        }

        printableCommand = ":" + base;
        for(auto& i : arguments)
            printableCommand += " %{" + i + "}";
    }

    std::string getPrintableCommand() const override
    {
        return printableCommand;
    }
};

} // namespace
#endif
