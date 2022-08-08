
#include "scope.hpp"

#include <vector>

using namespace std;
using namespace rbasic::vm;

Scope::Scope() : variables()
{

}

Scope::~Scope()
{
    exitScope();
}

void Scope::inheritScope(Scope *parent)
{
    // copy variables
    for (auto &[key, var] : parent->variables)
    {
        var->incRef();
        variables[key] = var;
    }
}

void Scope::exitScope()
{
    cleanupVars();
}

Var *Scope::getVar(const string &name)
{
    return variables[name];
}

Var *Scope::setVar(const string &name, Value *value)
{
    Var *var = variables[name];

    if (!var)
    {
        var = new Var(name.c_str(), value);
        variables[name] = var;
    }
    else
    {
        var->setValue(value);
    }

    return var;
}

void Scope::cleanupVars()
{
    vector<string> varNames;

    for (auto &pair : variables)
        varNames.push_back(pair.first);

    // iterate through each variable, and destroy any with a 0 reference count
    for (auto name : varNames)
    {
        Var *var = variables[name];

        // dec ref count
        var->decRef();

        // if ref count is 0 then destroy the variable
        if (var->getRefCount() == 0)
        {
            variables.erase(name);
            delete var;
        }
    }
}