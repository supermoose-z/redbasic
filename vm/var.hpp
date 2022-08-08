
#ifndef __VAR_HPP__
#define __VAR_HPP__

#include <string>
#include "value.hpp"

namespace rbasic::vm
{
    class Var
    {
    private:
        Value *value;
        std::string name;
        int count;

    public:
        Var(const char *name);
        Var(const char *name, Value *ptr);
        ~Var();

        const char *getName() const;

        void incRef();
        void decRef();
        int getRefCount() const;

        Value *getValue();
        void setValue(Value *val);
    };
}

#endif