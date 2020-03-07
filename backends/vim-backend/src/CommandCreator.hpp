#ifndef COMMANDCREATOR_H_
#define COMMANDCREATOR_H_

#include <functional>
#include <string>
#include <utility>
#include <algorithm>
#include <optional>
#include <stdexcept>

#include "KeyCommand.hpp"
#include "TextCommand.hpp"

namespace backends
{

class CommandCreator
{
public:
    static std::unique_ptr<Command> create(std::string sequence, std::function<void()> callback, std::string comment, std::function<void()> changeModeCallback)
    {
        if (sequence[0] == '#' && sequence.find('#', 1) != std::string::npos)
        {
            static const std::string name = "#vim#";
            if (sequence.substr(0, name.size()) != name)
                return std::unique_ptr<Command>();
            else
                sequence = sequence.substr(name.size());
        }

        sequence.erase(std::min({sequence.size(), sequence.find('%'), sequence.find('.')}));

        static const std::string editS = "!EDIT";
        if (sequence.size() >= editS.size()
            && std::equal(editS.begin(), editS.end(), sequence.end() - editS.size()))
        {
            sequence.erase(sequence.size() - editS.size());
            callback = [callback, changeModeCallback](){ callback(); changeModeCallback(); };
        }

        if (sequence.empty())
            return std::unique_ptr<Command>();

        if (sequence[0] == ':')
        {
            try
            {
                return std::unique_ptr<Command>(new TextCommand(sequence, callback, comment));
            }
            catch (const std::invalid_argument& e)
            {
                return std::unique_ptr<Command>();
            }
        }

        return std::unique_ptr<Command>(new KeyCommand(sequence, callback, comment));
    }
};


} // namespace

#endif
