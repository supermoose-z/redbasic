
#include "binopnode.hpp"
#include "../vm/vm.hpp"

using namespace std;
using namespace rbasic::vm;
using namespace rbasic::ast;

BinOpNode::BinOpNode(OpType t, ASTNode *lhs, ASTNode *rhs)
{
    type = t;
    left = lhs;
    right = rhs;
}

void BinOpNode::generate(vector<Instr*> &insts)
{
    Instr::InstrType instType;

    // generate instructions for operands
    left->generate(insts);
    right->generate(insts);

    // generate operation
    switch(type)
    {
    case BinOpNode::Add:
        instType = Instr::InstAdd;
        break;

    case BinOpNode::Sub:
        instType = Instr::InstSub;
        break;  

    case BinOpNode::Mul:
        instType = Instr::InstMul;
        break;  

    case BinOpNode::Div:
        instType = Instr::InstDiv;
        break;  

    case BinOpNode::Eq :
        instType = Instr::InstEq;
        break;  

    case BinOpNode::Ne :
        instType = Instr::InstNe;
        break;  

    case BinOpNode::Lt :
        instType = Instr::InstLt;
        break;  

    case BinOpNode::Lte:
        instType = Instr::InstLte;
        break;  

    case BinOpNode::Gt :
        instType = Instr::InstGt;
        break;  

    case BinOpNode::Gte:
        instType = Instr::InstGte;
        break;  
    }

    insts.push_back(new Instr(instType));
}