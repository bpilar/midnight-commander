#ifndef VIMBACKEND_H_
#define VIMBACKEND_H_

#include <functional>
#include <memory>
#include <string>

#include "BackendBase.hpp"

namespace backends
{

class VimBackend : public BackendBase
{
public:
    VimBackend();
    ~VimBackend();
    void bind(const std::string &command, std::function<void()> callback, const std::string &helpMessage) noexcept override;
    void operator()() override;
    void setTool(tools::ToolBase &newTool) override;

protected:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace backends
#endif
