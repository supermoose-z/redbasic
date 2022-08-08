
#include "assignnode.hpp"
#include "../vm/vm.hpp"

using namespace std;
using namespace rbasic::vm;
using namespace rbasic::ast;

AssignNode::AssignNode(const std::string &name, ASTNode *n)
{
    varName = name;
    expr = n;
}

void AssignNode::generate(vector<Instr*> &insts)
{
    expr->generate(insts);

    // add PRINT instruction
    insts.push_back(new Instr(Instr::InstAssign, varName.c_str()));
}