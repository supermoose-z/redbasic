

#ifndef __AST_INPUTNODE_HPP__
#define __AST_INPUTNODE_HPP__

#include <vector>
#include <string>
#include "astnode.hpp"

namespace rbasic::ast
{
    class InputNode : public ASTNode
    {
    private:
        std::string varName;
        ASTNode *prompt;
    
    public:
        InputNode(const std::string &n, ASTNode *p);

        void generate(std::vector<rbasic::vm::Instr*> &insts);
    };
}

#endif
