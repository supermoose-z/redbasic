
#include "inputnode.hpp"
#include "../vm/vm.hpp"

using namespace std;
using namespace rbasic::vm;
using namespace rbasic::ast;

InputNode::InputNode(const string &name, ASTNode *expr)
{
    varName = name;
    prompt = expr;
}

void InputNode::generate(vector<Instr*> &insts)
{
    // generate instructions for prompt expression
    prompt->generate(insts);

    // add INPUT instruction
    insts.push_back(new Instr(Instr::InstInput, varName.c_str()));
}