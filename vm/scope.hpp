
#ifndef __SCOPE_HPP__
#define __SCOPE_HPP__

#include <map>

#include "value.hpp"
#include "var.hpp"

namespace rbasic::vm
{
    //struct Block;

    class Scope
    {
    private:
        std::map<const std::string, Var*> variables;
        //std::map<const std::string, Block*> routines;

    public:  
        Scope();
        ~Scope();

        // create a new scope that inherits the variables of this scope
        void inheritScope(Scope *scope);

        // copy the values of any inherited variables 
        void exitScope();

        // get a variable by name
        Var *getVar(const std::string &name);
        Var *setVar(const std::string &name, Value *value);

        // cleanup variables pending deletion
        void cleanupVars();
    };
}

#endif
