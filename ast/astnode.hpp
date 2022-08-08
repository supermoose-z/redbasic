
#ifndef __ASTNODE_HPP
#define __ASTNODE_HPP

#include <vector>

namespace rbasic::vm
{
    class Instr;
    class Value;
}

namespace rbasic::ast
{
    class ASTNode
    {
    public:
        virtual ~ASTNode() { };

        virtual void generate(std::vector<rbasic::vm::Instr*> &instrs) = 0;
    };
}

#endif
