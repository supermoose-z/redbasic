
#include "valuenode.hpp"
#include "../vm/vm.hpp"

using namespace std;
using namespace rbasic::vm;
using namespace rbasic::ast;

ValueNode::ValueNode(ValueType t)
{
    type = t;
    boolValue = false;
    numValue = 0;
    strValue = "";
}

ValueNode::ValueNode(ValueType t, bool b) : ValueNode(t)
{
    boolValue = b;
}

ValueNode::ValueNode(ValueType t, const std::string &str) : ValueNode(t)
{
    strValue = str;
}

ValueNode::ValueNode(ValueType t, double num) : ValueNode(t)
{
    type = ValueNode::Number;
    numValue = num;
}

void ValueNode::setNone()
{
    type = ValueNode::None;
    boolValue = false;
    numValue = 0;
    strValue = "";
}

void ValueNode::generate(std::vector<rbasic::vm::Instr*> &insts)
{
    Value *value;

    switch(type)
    {
    case ValueNode::None:
        value = new Value(&Value::None);
        break;

    case ValueNode::VarRef:
        insts.push_back(new Instr(Instr::InstPushVar, strValue.c_str()));
        return;

    case ValueNode::String:
        value = new Value(strValue.c_str());
        break;

    case ValueNode::Bool:
        value = new Value(boolValue);
        break;

    case ValueNode::Number:
        value = new Value(numValue);
        break;
    }

    insts.push_back(new Instr(Instr::InstPushVal, value));
}
