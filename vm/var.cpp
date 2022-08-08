
#include "var.hpp"

using namespace rbasic::vm;

Var::Var(const char *name) : count(0), value(), name(name)
{

}

Var::Var(const char *name, Value *v) : count(1), value(v), name(name)
{

}

Var::~Var()
{
    //value.release();
    if (value)
        delete value;
}

const char *Var::getName() const
{
    return name.c_str();
}

void Var::incRef()
{
    count++;
}

void Var::decRef()
{
    if (count > 0)
        count--;
}

int Var::getRefCount() const
{
    return count;
}

Value *Var::getValue()
{
    return value;
}

void Var::setValue(Value *newVal)
{
    value = newVal;
}