
#ifndef __AST_VALUE_HPP__
#define __AST_VALUE_HPP__

#include <string>

#include "astnode.hpp"

namespace rbasic::ast
{
    class ValueNode : public ASTNode
    {
    private:
        double numValue;
        bool boolValue;
        std::string strValue;

    public:
        enum ValueType
        {
            None,
            String,
            Number,
            Bool,
            VarRef,          
        } type;

        ValueNode(ValueType);
        ValueNode(ValueType, bool);
        ValueNode(ValueType, const std::string &);
        ValueNode(ValueType, double);

        void setNone();
        
        void generate(std::vector<rbasic::vm::Instr*> &insts);
    };
}

#endif
