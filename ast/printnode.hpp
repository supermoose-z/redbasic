
#ifndef __AST_PRINTNODE_HPP__
#define __AST_PRINTNODE_HPP__

#include <vector>
#include "astnode.hpp"

namespace rbasic::ast
{
    class PrintNode : public ASTNode
    {
    private:
        std::vector<ASTNode*> args;
    
    public:
        PrintNode(std::vector<ASTNode*> &args);

        void generate(std::vector<rbasic::vm::Instr*> &insts);
    };
}

#endif
