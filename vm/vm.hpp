
#ifndef __VM_HPP__
#define __VM_HPP__

#include <string>
#include <vector>
#include <stack>
#include "scope.hpp"
#include "../error.hpp"

namespace rbasic::vm
{
    class RuntimeError : public BaseError
    {
    public:
        RuntimeError(const std::string &msg);
    };

    class Instr
    {
    public:
        enum InstrType
        {
            InstBranchTrue,
            InstBranchFalse,
            InstCall,
            InstReturn, 
            InstJump,
            InstPushScope,
            InstPopScope,
            InstEq,
            InstNe,
            InstLt,
            InstLte,
            InstGt,
            InstGte,
            InstAdd,
            InstMul,
            InstDiv,
            InstSub,
            InstPrint,
            InstInput,
            InstPushVal,
            InstPushVar,
            InstPopVar,
            InstAssign,
        };

        InstrType type;

        int numParam;
        const char *strParam;
        Value *valParam;

        ~Instr();

        Instr(InstrType type);
        Instr(InstrType type, int num);
        Instr(InstrType type, const char *str);
        Instr(InstrType type, Value *val);

        std::string toString();
    };

    
    struct Block
    {
        std::vector<Instr*> code;

        ~Block()
        {
            for(Instr *inst : code)
                delete inst;
        }
    };


    class BasicVM
    {
    private:
        std::stack<Value*> stack;
        std::stack<Scope*> scopeStack;
        std::stack<int> callStack;
        std::vector<Instr*> program;
        Scope *globalScope;
        int instPtr;

        void exec(Instr *inst);
        Value *popVal();
        void getNumericArgs(double*, double*);
        void pushScope();
        void popScope();
        void jumpTo(int offset);
        void releaseProgram();

    public:
        BasicVM();
        ~BasicVM();

        Scope *getGlobalScope();
        void loadProgram(const std::vector<Instr*> &insts);
        void run();
    };

}



#endif
