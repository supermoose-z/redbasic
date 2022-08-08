
#include "error.hpp"

using namespace rbasic;

BaseError::BaseError(const std::string &str)
{
    message = str;
}

const char *BaseError::getMessage()
{
    return message.c_str();
}