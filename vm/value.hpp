
#ifndef __VALUE_HPP__
#define __VALUE_HPP__

#include <cstring>
#include <iostream>
#include <string>

#include "../error.hpp"

namespace rbasic::vm
{
    enum ValueType
    {
        ValueTypeNone,
        ValueTypeNumber,
        ValueTypeBool,
        ValueTypeString
    };

    class ValueError : public BaseError
    {
    public:
        ValueError(const std::string &msg);
    };

    class Value
    {
    private:
        ValueType type;
        unsigned long data;
        unsigned int refCount;

    public:
        static Value None();

        Value();
        Value(const Value &v);
        Value(double num);
        Value(bool flag);
        Value(const char *str);
        ~Value();

        ValueType getType();
        void release();

        double getNumberValue();
        bool getBoolValue();
        const char *getStringValue();

        void setValue(double f);
        void setValue(bool b);
        void setValue(const char *sz);

        void copyFrom(const Value &other);

        static int compare(Value *lhs, Value *rhs);
        static bool equals(Value *lhs, Value *rhs);

        Value &operator=(const Value &other);

        std::string toString();
    };

}

#endif