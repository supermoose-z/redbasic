
#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <vector>
#include <queue>
#include <string>
#include <set>

#include "../error.hpp"
#include "../ast/ast.hpp"
#include "../lexer/lexer.hpp"

namespace rbasic::parser
{
    class ParserError : public BaseError
    {
    public:
        ParserError(const std::string &msg);
    };

    class Parser
    {
    private:
        std::set<rbasic::ast::ASTNode*> allNodes;
        std::queue<rbasic::lexer::LexerToken*> tokens;
        std::queue<rbasic::ast::ASTNode*> statements;
        rbasic::ast::ASTNode *rootNode = NULL;
        rbasic::lexer::Lexer lexer;

        rbasic::lexer::LexerToken *getNextToken();

        void addNode(rbasic::ast::ASTNode *n);

        void parseExprList(std::vector<rbasic::ast::ASTNode*> &exprs);
        rbasic::ast::ASTNode *parseExpr();
        rbasic::ast::ASTNode *parsePrint();
        rbasic::ast::ASTNode *parseInput();
        rbasic::ast::ASTNode *parseAssign();
        rbasic::ast::ASTNode *parseStatement();

    public:
        Parser();
        ~Parser();

        void parse(const std::string &src);
        void compile(std::vector<rbasic::vm::Instr*> &program);
    };
}

#endif
