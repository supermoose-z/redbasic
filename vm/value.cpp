
#include <iostream>
#include <sstream>
#include <cmath>

#include "value.hpp"

using namespace std;
using namespace rbasic::vm;

template <typename T> int _sign(T val) 
{
    return (T(0) < val) - (val < T(0));
}

ValueError::ValueError(const string &msg) : BaseError(msg)
{

}

Value Value::None()
{
    return Value();
}

Value::Value()
{
    type = ValueTypeNone;
    data = 0;
    refCount = 1;
}

Value::Value(const Value &other) : Value()
{
    copyFrom(other);
}

Value::Value(double num) : Value()
{
    type = ValueTypeNumber;
    setValue(num);
}

Value::Value(bool flag) : Value()
{
    type = ValueTypeBool;
    setValue(flag);
}

Value::Value(const char *str) : Value()
{
    type = ValueTypeString;
    setValue(str);
}

Value::~Value()
{
    release();
}

void Value::release()
{
    if (type == ValueTypeString && data != 0)
    {
        char *str = reinterpret_cast<char*>(data);

        std::cout << "releasing string: " << str << std::endl;

        delete[] str;
    }

    data = 0;
}

ValueType Value::getType()
{
    return type;
}

double Value::getNumberValue()
{
    return type == ValueTypeNumber ? static_cast<double>(data) : 0;
}

bool Value::getBoolValue()
{
    return data == 1;
}

const char *Value::getStringValue()
{
    return type == ValueTypeString ? reinterpret_cast<const char*>(data) : NULL;
}


void Value::setValue(double f)
{
    release();

    type = ValueTypeNumber;
    data = static_cast<unsigned long>(f);
}

void Value::setValue(bool b)
{
    type = ValueTypeBool;
    data = b ? 1 : 0;
}

void Value::setValue(const char *sz)
{
    release();

    char *newStr = new char[std::strlen(sz)];
    std::strcpy(newStr, sz);

    data = reinterpret_cast<unsigned long>(newStr);
}

void Value::copyFrom(const Value &other)
{
    switch(other.type)
    {
    case ValueTypeNone:
        type = ValueTypeNone;
        break;

    case ValueTypeNumber:
    case ValueTypeBool:
        data = other.data;
        break;

    case ValueTypeString:
        setValue(reinterpret_cast<const char*>(other.data));
        break;
    }

    type = other.type;
}

int Value::compare(Value *lhs, Value *rhs)
{
    // raise error if values are not the same type
    if (lhs->getType() != rhs->getType())
        throw ValueError("Cannot compare values of different types!");

    if (lhs->getType() == ValueTypeNumber)
    {
        return _sign(lhs->getNumberValue() - rhs->getNumberValue());
    }
    else if (lhs->getType() == ValueTypeString)
    {
        return strcmp(lhs->getStringValue(), rhs->getStringValue());
    }
    else if (lhs->getType() == ValueTypeBool)
    {
        if (lhs->getBoolValue() && rhs->getBoolValue())
            return 0;
        else if (lhs->getBoolValue())
            return -1;
        else if (rhs->getBoolValue())
            return 1;
    }
    else if (lhs->getType() == ValueTypeNone)
    {
        return 0;
    }
}

bool Value::equals(Value *lhs, Value *rhs)
{
    // return false if values are different type
    if (lhs->getType() != rhs->getType())
        return false;

    switch(lhs->getType())
    {
    case ValueTypeNone:
        return true;

    case ValueTypeBool:
        return lhs->getBoolValue() == rhs->getBoolValue();
        
    case ValueTypeNumber:
        return lhs->getNumberValue() == rhs->getNumberValue();

    case ValueTypeString:
        return strcmp(lhs->getStringValue(), rhs->getStringValue()) == 0;
    }
}

Value &Value::operator=(const Value &other)
{
    copyFrom(other);

    return *this;
}

string Value::toString()
{
    ostringstream oss("");

    switch(type)
    {
    case ValueTypeNone:
        oss << "(None)";
        break;
    case ValueTypeBool:
        oss << (getBoolValue() ? "True" : "False");
        break;
    case ValueTypeString:
        oss << getStringValue();
        break;
    case ValueTypeNumber:
        oss << getNumberValue();
        break;
    }

    return oss.str();
}