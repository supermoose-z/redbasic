
#ifndef __ERROR_HPP__
#define __ERROR_HPP__

#include <string>

namespace rbasic
{
    class BaseError
    {
    private:
        std::string message;

    public:
        BaseError(const std::string &err);

        const char *getMessage();
    };
}

#endif
