
#ifndef __AST_ASSIGNNODE_HPP__
#define __AST_ASSIGNNODE_HPP__

#include <vector>
#include <string>
#include "astnode.hpp"

namespace rbasic::ast
{
    class AssignNode : public ASTNode
    {
    private:
        std::string varName;
        ASTNode *expr;

    public:
        AssignNode(const std::string &name, ASTNode *expr);

        void generate(std::vector<rbasic::vm::Instr*> &insts);
    };
}

#endif