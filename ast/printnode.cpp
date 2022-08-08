
#include "printnode.hpp"
#include "../vm/vm.hpp"

using namespace std;
using namespace rbasic::vm;
using namespace rbasic::ast;

PrintNode::PrintNode(vector<ASTNode*> &nodes)
{
    for(ASTNode *n : nodes)
        args.push_back(n);
}

void PrintNode::generate(vector<Instr*> &insts)
{
    // generate instructions for each argument first
    for(ASTNode *n : args)
        n->generate(insts);

    // add PRINT instruction
    insts.push_back(new Instr(Instr::InstPrint, args.size()));
}