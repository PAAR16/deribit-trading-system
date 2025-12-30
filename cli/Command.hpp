#pragma once
#include <string>
#include <vector>

class Command {
public:
    virtual void execute(const std::vector<std::string>& args) = 0;
    virtual std::string help() const = 0;
    virtual ~Command() = default;
};