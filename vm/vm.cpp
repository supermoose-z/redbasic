
#include <iostream>
#include <sstream>
#include <cstring>

#include "vm.hpp"

using namespace std;
using namespace rbasic;
using namespace rbasic::vm;

RuntimeError::RuntimeError(const string &str) : BaseError(str)
{

}

Instr::Instr(Instr::InstrType type)
{
    this->type = type;
}

Instr::Instr(Instr::InstrType type, int num) : Instr(type)
{
    numParam = num;
}

Instr::Instr(Instr::InstrType type, const char *str) : Instr(type)
{
    char *newStr = new char[strlen(str)];

    strcpy(newStr, str);
    strParam = newStr;
}

Instr::Instr(Instr::InstrType type, Value *val) : Instr(type)
{
    valParam = val;
}

Instr::~Instr()
{
    switch(type)
    {
        case Instr::InstInput:
        case Instr::InstPushVar:
        case Instr::InstPopVar:
        case Instr::InstAssign:
            delete[] strParam;
            break;

        default:
            break;
    }

    // if (valParam != NULL)
    //     delete valParam;
}

string Instr::toString()
{
    ostringstream oss;

    switch(type)
    {
        case Instr::InstBranchFalse:
            oss << "BranchFalse";
            break;

        case Instr::InstBranchTrue:
            oss << "BranchTrue";
            break;

        case Instr::InstReturn:
            oss << "Return";
            break;

        case Instr::InstJump:
            oss << "Jump";
            break;

        case Instr::InstEq:
            oss << "InstEq";
            break;

        case Instr::InstNe:
            oss << "InstNe";
            break;

        case Instr::InstLt:
            oss << "InstLt";
            break;

        case Instr::InstLte:
            oss << "InstLte";
            break;

        case Instr::InstGt:
            oss << "InstGt";
            break;

        case Instr::InstGte:
            oss << "InstGte";
            break;

        case Instr::InstAdd: 
            oss << "Add";
            break;

        case Instr::InstMul: 
            oss << "Mul";
            break;

        case Instr::InstDiv: 
            oss << "Div";
            break;

        case Instr::InstSub: 
            oss << "Sub";
            break;

        case Instr::InstPrint: 
            oss << "Print(" << numParam << ")";
            break;

        case Instr::InstInput: 
            oss << "Input(" << strParam << ")";
            break;

        case Instr::InstPushVal: 
            oss << "PushVal(" << valParam->toString() << ")";
            break;

        case Instr::InstPushVar: 
            oss << "PushVar(" << strParam << ")";
            break;

        case Instr::InstPopVar: 
            oss << "PopVar(" << strParam << ")";
            break;

        case Instr::InstAssign: 
            oss << "Assign(" << strParam << ")";
            break;

        case Instr::InstCall: 
            oss << "Call(" << numParam << ")";
            break;

        case Instr::InstPushScope:
            oss << "PushScope";
            break;

        case Instr::InstPopScope:
            oss << "PopScope";
            break;

    }

    return oss.str();
}

BasicVM::BasicVM()
{
    // add initial, global scope
    globalScope = new Scope();
    scopeStack.push(globalScope);
}

BasicVM::~BasicVM()
{
    // clear out scope and data stacks
    while (!scopeStack.empty())
    {
        Scope *top = scopeStack.top();
        delete top;

        scopeStack.pop();
    }

    while (!stack.empty())
    {
        Value *top = stack.top();
        delete top;

        stack.pop();
    }
}

void BasicVM::exec(Instr *inst)
{
    Scope *scope = scopeStack.top();

    switch(inst->type)
    {
        case Instr::InstBranchTrue:
            {
                Value *val = popVal();

                if (val->getBoolValue())
                {
                    jumpTo(inst->numParam);
                    return;
                }
            }
            break;

        case Instr::InstBranchFalse:
            {
                Value *val = popVal();

                if (!val->getBoolValue())
                {
                    jumpTo(inst->numParam);
                    return;
                }
            }
            break;

        case Instr::InstCall: 
            callStack.push(instPtr);
            jumpTo(inst->numParam);
            pushScope();
            return;

        case Instr::InstReturn:
            {
                if (callStack.size() == 0)
                    throw RuntimeError("Call stack exhausted");

                jumpTo(callStack.top());
                popScope();

                callStack.pop();
            }
            return;
        
        case Instr::InstJump:
            jumpTo(inst->numParam);
            return;

        case Instr::InstPushScope:
            pushScope();
            break;

        case Instr::InstPopScope:
            popScope();
            break;

        case Instr::InstEq:
            {
                Value *lhs = popVal();
                Value *rhs = popVal();

                stack.push(new Value(Value::equals(lhs, rhs)));
            }
            break;

        case Instr::InstNe:
            {
                Value *lhs = popVal();
                Value *rhs = popVal();

                stack.push(new Value(!Value::equals(lhs, rhs)));   
            }
            break;

        case Instr::InstLt:
            {
                Value *lhs = popVal();
                Value *rhs = popVal();

                stack.push(new Value(Value::compare(lhs, rhs) <= -1));
            }
            break;

        case Instr::InstLte:
            {
                Value *lhs = popVal();
                Value *rhs = popVal();

                stack.push(new Value(Value::compare(lhs, rhs) <= 0));
            }
            break;

        case Instr::InstGt:
            {
                Value *lhs = popVal();
                Value *rhs = popVal();

                stack.push(new Value(Value::compare(lhs, rhs) >= 1));
            }
            break;

        case Instr::InstGte:
            {
                Value *lhs = popVal();
                Value *rhs = popVal();

                stack.push(new Value(Value::compare(lhs, rhs) >= 0));
            }
            break;


        case Instr::InstAdd: 
            {
                double lhs, rhs;
                getNumericArgs(&lhs, &rhs);

                stack.push(new Value(lhs + rhs));
            }
            break;

        case Instr::InstMul: 
            {
                double lhs, rhs;
                getNumericArgs(&lhs, &rhs);

                stack.push(new Value(lhs * rhs));
            }
            break;

        case Instr::InstDiv: 
            {
                double lhs, rhs;
                getNumericArgs(&lhs, &rhs);

                stack.push(new Value(lhs / rhs));
            }
            break;

        case Instr::InstSub: 
            {
                double lhs, rhs;
                getNumericArgs(&lhs, &rhs);

                stack.push(new Value(lhs - rhs));
            }
            break;

        case Instr::InstPrint: 
            for(int i=0; i<inst->numParam; i++)
            {
                Value *v = popVal();
                cout << v->toString() << " ";
            }
            cout << endl;
            break;

        case Instr::InstInput: 
            {
                Value *prompt = popVal();
                cout << prompt->toString() << " ";

                string input;
                getline(cin, input);

                scope->setVar(inst->strParam, new Value(input.c_str()));
            }
            break;

        case Instr::InstPushVal: 
            stack.push(inst->valParam);
            break;

        case Instr::InstPushVar: 
            {
                Var *var = scope->getVar(inst->strParam);
                if (var != NULL)
                    stack.push(var->getValue());
            }
            break;

        case Instr::InstPopVar:
            {
                Value *val = popVal();
                scope->setVar(inst->strParam, val);
            }
            break;

        case Instr::InstAssign: 
            {
                Value *val = popVal();
                scope->setVar(inst->strParam, val);
            }
            break;


    }

    instPtr++;
}

Value *BasicVM::popVal()
{
    if (!stack.empty())
    {
        Value *v = stack.top();
        
        stack.pop();

        return v;
    }
    else
    {
        throw RuntimeError("Stack underflow error");
    }
}

void BasicVM::getNumericArgs(double *lhs, double *rhs)
{
    Value *a = popVal(), *b = popVal();

    *lhs = a ? a->getNumberValue() : 0;
    *rhs = b ? b->getNumberValue() : 0;
}

Scope *BasicVM::getGlobalScope()
{
    return globalScope;
}

void BasicVM::loadProgram(const std::vector<Instr*> &insts)
{
    releaseProgram();

    program.reserve(insts.size());

    for(Instr *i : insts)
        program.push_back(i);
}

void BasicVM::run()
{
    instPtr = 0;

    while (instPtr < program.size())
    {
        exec(program.at(instPtr));
    }
}

void BasicVM::pushScope()
{
    // create new scope
    Scope *newScope = new Scope();

    // inherit the current scope
    newScope->inheritScope(scopeStack.top());

    // make new scope current
    scopeStack.push(newScope);
}

void BasicVM::popScope()
{
    if (scopeStack.size() > 1)
    {
        Scope *prev = scopeStack.top();

        prev->exitScope();

        scopeStack.pop();
    }
}

void BasicVM::jumpTo(int offset)
{
    if (offset < 0 || offset >= program.size())
        throw new RuntimeError("Instruction index out of range");

    instPtr = offset;
}

void BasicVM::releaseProgram()
{
    for(Instr *instr : program)
    {
        delete instr;
    }

    program.clear();
}