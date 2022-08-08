#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <fstream>

#include "parser/parser.hpp"
#include "vm/vm.hpp"

using namespace std;
using namespace rbasic;
using namespace rbasic::lexer;
using namespace rbasic::ast;
using namespace rbasic::vm;
using namespace rbasic::parser;

bool loadFile(const string &path, string &code)
{
    fstream ifs(path);
    int size;

    if (ifs.bad())
    {
        cout << "failed to open file" << endl;
        return false;
    }

    ifs.seekg(0, ios::end);
    size = ifs.tellg();
    ifs.seekg(0);

    if (size <= 0)
    {
        cout << "Tried to open file but the size is " << size << endl;
        return false;
    }
    
    char buf[size];
    ifs.read(&buf[0], size);
    ifs.close();

    code.assign(buf);

    return true;
}


int main()
{
    string code;
    
    if (!loadFile("test.bas", code))
    {
        cout << "Unable to load file" << endl;
        return 0;
    }

    try
    {
        Parser parser;
        vector<Instr*> program;

        parser.parse(code);
        parser.compile(program);
        
        BasicVM vm;
        vm.loadProgram(program);
        vm.run();

        cout << "Done?" << endl;
    }
    catch(LexerError &err)
    {
        cout << "Lexer error: " << err.getMessage() << endl;
    }
    catch(ParserError &err)
    {
        cout << "Parser error: " << err.getMessage() << endl;
    }

    return 0;
}