
#include "astnode.hpp"

namespace rbasic::ast
{
    class BinOpNode : public ASTNode
    {
    private:
        ASTNode *left, *right;

    public:
        enum OpType
        {
            Add,
            Sub,
            Mul,
            Div,
            Eq,
            Ne,
            Lt,
            Lte,
            Gt,
            Gte,
        } type;

        BinOpNode(OpType type, ASTNode *lhs, ASTNode *rhs);

        void generate(std::vector<rbasic::vm::Instr*> &insts);
    };
}