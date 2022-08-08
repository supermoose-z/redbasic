
#include <set>
#include <cstring>
#include "parser.hpp"

using namespace std;
using namespace rbasic::vm;
using namespace rbasic::ast;
using namespace rbasic::lexer;
using namespace rbasic::parser;

const set<string> reservedWords {
    "IF", "THEN", "ELSE", "ELSEIF", 
    "PRINT", "INPUT", "TRUE", "FALSE"
};

bool isReservedWord(const string &str)
{
    return reservedWords.find(str) != reservedWords.end();
}

bool opNodeTypeFromToken(LexerToken *tok, BinOpNode::OpType *type)
{
    switch(tok->type)
    {
        case LexerToken::Add: 
            *type = BinOpNode::Add;
            return true;

        case LexerToken::Sub: 
            *type = BinOpNode::Sub;
            return true;

        case LexerToken::Mul: 
            *type = BinOpNode::Mul;
            return true;

        case LexerToken::Div: 
            *type = BinOpNode::Div;
            return true;

        case LexerToken::Eq: 
            *type = BinOpNode::Eq;
            return true;

        case LexerToken::Ne: 
            *type = BinOpNode::Ne;
            return true;

        case LexerToken::Lt: 
            *type = BinOpNode::Lt;
            return true;

        case LexerToken::Lte: 
            *type = BinOpNode::Lte;
            return true;

        case LexerToken::Gt: 
            *type = BinOpNode::Gt;
            return true;

        case LexerToken::Gte: 
            *type = BinOpNode::Gte;
            return true;
            
        default:
            return false;
    }
}

ParserError::ParserError(const string &msg) : BaseError(msg)
{

}

Parser::Parser()
{

}

Parser::~Parser()
{
    // clear out AST nodes
    for (ASTNode *n : allNodes)
        delete n;
}

LexerToken *Parser::getNextToken()
{
    if (tokens.empty())
        throw ParserError("Unexpected end of token stream");

    return tokens.front();
}

void Parser::addNode(ASTNode *n)
{
    //allNodes.push_back(n);
    if (allNodes.find(n) == allNodes.end())
        allNodes.insert(n);
}

// parse an expression
ASTNode *Parser::parseExpr()
{
    // get next token
    LexerToken *tok = getNextToken();

    // value/left-side node
    ASTNode *lhs;

    // get next token and treat it as potentially the left-side of a BinOp, or a value literal/var ref
    switch(tok->type)
    {
    case LexerToken::ParenL:
        {
            // consume current token
            tokens.pop();

            // get expression
            ASTNode *expr = parseExpr();

            // if next token isn't a closing ) then raise error
            tok = getNextToken();
            if (tok->type != LexerToken::ParenR)
                throw ParserError("Expected closing ')', but found " + tok->toString() + " instead");

            // return expression token
            lhs = expr;
        }
        break;
    case LexerToken::String:
        lhs = new ValueNode(ValueNode::String, tok->strValue);
        break;

    case LexerToken::Number:
        lhs = new ValueNode(ValueNode::Number, tok->numValue);
        break;
        
    case LexerToken::Identifier:
        if (tok->strValue == "TRUE")
            lhs = new ValueNode(ValueNode::Bool, true);
        else if (tok->strValue == "FALSE")
            lhs = new ValueNode(ValueNode::Bool, false);
        else if (isReservedWord(tok->strValue))
            throw ParserError(tok->strValue + " is a reserved word");
        else
            lhs = new ValueNode(ValueNode::VarRef, tok->strValue);
        break;

    default:
        throw ParserError("Unexpected token: " + tok->toString());
    }

    // consume current token and get next token
    tokens.pop();

    // if there are no more tokens left then return the current node
    if (tokens.empty())
    {
        addNode(lhs);
        return lhs;
    }
    
    // otherwise check next token
    tok = getNextToken();

    // if next token is an operand then extract the next expression from the token stream
    BinOpNode::OpType opType;
    if (opNodeTypeFromToken(tok, &opType))
    {
        // consume current token
        tokens.pop();

        // get next expression
        ASTNode *rhs = parseExpr();

        // create binop node
        BinOpNode *opNode = new BinOpNode(opType, lhs, rhs);
        addNode(opNode);

        return opNode;

    }
    // otherwise just return the lhs node
    else
    {
        addNode(lhs);
        return lhs;
    }
}

void Parser::parseExprList(vector<ASTNode*> &list)
{
    ASTNode *expr;

    // get expression
    expr = parseExpr();
    list.push_back(expr);

    // while next token is a comma, get another expression
    //while(next->type == LexerToken::Comma)
    LexerToken *next;
    while(!tokens.empty())
    {
        // get next token, if it isn't a comma then exit out of this loop
        next = getNextToken();
        if (next->type != LexerToken::Comma)
            break;

        // consume token
        tokens.pop();

        // extract next expression
        expr = parseExpr();
        list.push_back(expr);
    }
}

ASTNode *Parser::parsePrint()
{
    // get next token
    LexerToken *tok = getNextToken();

    // if it is PRINT 
    if (tok->type == LexerToken::Identifier && tok->strValue == "PRINT")
    {
        // consume token
        tokens.pop();

        // parse print arguments
        vector<ASTNode*> args;
        parseExprList(args);
        
        reverse(args.begin(), args.end());

        if (args.size() == 0)
            throw ParserError("PRINT requires at least one argument");

        PrintNode *node = new PrintNode(args);
        addNode(node);

        return node;
    }
    else
    {
        return NULL;
    }
}

ASTNode *Parser::parseInput()
{
    LexerToken *tok = getNextToken();

    if (tok->type == LexerToken::Identifier && tok->strValue == "INPUT")
    {
        // consume INPUT token
        tokens.pop();
        
        // get prompt expression
        ASTNode *prompt = parseExpr();
        string varName;

        // make sure next token is a comma
        tok = getNextToken();
        if (tok->type == LexerToken::Comma)
            tokens.pop();
        else
            throw ParserError("Expected ',' but found " + tok->toString() + " instead");

        // get variable name to save input to
        tok = getNextToken();
        if (tok->type == LexerToken::Identifier && !isReservedWord(tok->strValue))
        {
            varName = tok->strValue;
            tokens.pop();
        }
        else
        {
            throw ParserError("Cannot use reserved word '" + tok->strValue + "' as a variable name");
        }

        // create node
        InputNode *node = new InputNode(varName, prompt);
        addNode(node);

        return node;
    }
    else
    {
        return NULL;
    }
}

ASTNode *Parser::parseAssign()
{
    LexerToken *tok = getNextToken();

    if (tok->type != LexerToken::Identifier)
        return NULL;

    if (isReservedWord(tok->strValue))
        throw ParserError("Unexpected reserved word: " + tok->strValue);

    // consume token and check next
    string varName = tok->strValue;
    tokens.pop();

    tok = getNextToken();
    
    if (tok->type != LexerToken::Eq)
        throw ParserError("Expected '=' but found " + tok->toString() + " instead");

    // consume token and parse expression
    tokens.pop();
    ASTNode *expr = parseExpr();

    // create new node
    AssignNode *node = new AssignNode(varName, expr);
    addNode(node);

    return node;
}

ASTNode *Parser::parseStatement()
{
    ASTNode *stat;

    stat = parsePrint();

    if (stat == NULL)
        stat = parseInput();

    if (stat == NULL)
        stat = parseAssign();

    if (stat == NULL)
        throw ParserError("Unexpected token: " + getNextToken()->toString());

    // make sure next token is EOL or there are no more tokens left
    if (!tokens.empty())
    {
        LexerToken *next = tokens.front();
        if (next->type == LexerToken::EOL)
            tokens.pop();
        else
            throw ParserError("Expected end-of-line but found " + next->toString() + " instead");
    }

    return stat;
}

void Parser::parse(const std::string &src)
{
    // initialize lexer
    lexer.setInput(src);

    // read token stream
    LexerToken *tok;
    while ((tok = lexer.getNextToken()))
        if (tok->type != LexerToken::Comment)
            tokens.push(tok);

    // parse statements
    ASTNode *stat;
    while (!tokens.empty())
    {
        stat = parseStatement();
        statements.push(stat);
    }
}

void Parser::compile(vector<Instr*> &program)
{
    while(!statements.empty())
    {
        statements.front()->generate(program);
        statements.pop();
    }
}