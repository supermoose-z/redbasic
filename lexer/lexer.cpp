
#include <regex>
#include <sstream>

#include "lexer.hpp"

using namespace std;
using namespace rbasic;
using namespace rbasic::lexer;

LexerError::LexerError(const string &msg) : BaseError(msg)
{

}

// ----------------

LexerToken::LexerToken()
{

}

LexerToken::LexerToken(LexerToken::Type t)
{
    type = t;
    numValue = 0;
    strValue = "";
}

LexerToken::LexerToken(LexerToken::Type t, const string &str) : strValue(str)
{
    type = t;
    numValue = 0;
}

LexerToken::LexerToken(LexerToken::Type t, double num)
{
    type = t;
    numValue = num;
    strValue = "";
}

LexerToken::~LexerToken()
{
}

string LexerToken::toString()
{
    ostringstream oss;

    switch (type)
    {
    case LexerToken::EOL:
        oss << "EOL";
        break;
        
    case LexerToken::Identifier:
        oss << "Identifier(" << strValue << ")";
        break;
        
    case LexerToken::String:
        oss << "String(" << strValue << ")";
        break;
        
    case LexerToken::Comment:
        oss << "Comment(" << strValue << ")";
        break;
        
    case LexerToken::Number:
        oss << "Number(" << numValue << ")";
        break;
        
    case LexerToken::Lt:
        oss << "<";
        break;
        
    case LexerToken::Gt:
        oss << ">";
        break;
        
    case LexerToken::Lte:
        oss << "<=";
        break;
        
    case LexerToken::Gte:
        oss << ">=";
        break;
        
    case LexerToken::Eq:
        oss << "=";
        break;
        
    case LexerToken::Ne:
        oss << "<>";
        break;
        
    case LexerToken::Semi:
        oss << ";";
        break;
        
    case LexerToken::ParenL:
        oss << "(";
        break;
        
    case LexerToken::ParenR:
        oss << ")";
        break;
        
    case LexerToken::BracketL:
        oss << "[";
        break;
        
    case LexerToken::BracketR:
        oss << "]";
        break;
        
    case LexerToken::Comma:
        oss << ",";
        break;
        
    case LexerToken::Add:
        oss << "+";
        break;
        
    case LexerToken::Sub:
        oss << "-";
        break;
        
    case LexerToken::Mul:
        oss << "*";
        break;
        
    case LexerToken::Div:
        oss << "/";
        break;
    }

    return oss.str();
}

// ----------------

Lexer::Lexer()
{

}

Lexer::Lexer(const string &src)
{
    setInput(src);
}

Lexer::~Lexer()
{
    clearTokens();
}

void Lexer::setInput(const string &src)
{
    input = src;
}

void Lexer::clearTokens()
{
    while (tokens.size() > 0)
    {
        LexerToken *tok = *(tokens.begin());
        
        delete tok;

        tokens.erase(tokens.begin());
    }
}

char Lexer::getch()
{
    if (input.size() == 0)
        throw LexerError("Unexpected end of input reached");

    char ch = input[0];

    input.erase(0, 1);

    return ch;
}

void Lexer::eatWhitespace()
{
    regex ws("\\s");

    while (regex_match(input.substr(0, 1), ws))
        input.erase(0, 1);
}

bool Lexer::identifierRule()
{
    if (!regex_match(input.substr(0, 1), regex("[a-zA-Z_]")))
        return false;

    regex re("[a-zA-Z0-9_]");
    string tok = "";
    char ch[] = { getch(), 0 };

    while (regex_match(ch, re))
    {
        tok.push_back(ch[0]);
        ch[0] = getch();        
    }

    input.insert(input.begin(), ch[0]);

    if (tok.size() > 0)
    {
        tokens.push_back(new LexerToken(LexerToken::Identifier, tok));
        return true;
    }
    else
    {
        return false;
    }
}

bool Lexer::stringRule()
{
    if (input[0] != '"')
        return false;

    char ch;
    string tok = "";

    // eat opening string character
    getch();

    // get string contents
    while (true)
    {
        if (input.size() == 0)
            throw LexerError("Unclosed string literal");

        ch = getch();

        if (ch == '\\')
        {
            tok.push_back(ch);
            tok.push_back(getch());
            tok.push_back(getch());
        }
        else if (ch == '"')
        {
            tokens.push_back(new LexerToken(LexerToken::String, tok));
            return true;
        }
        else
        {
            tok.push_back(ch);
        }
    }
}

bool Lexer::numberRule()
{
    regex re("[0-9.]");
    string tok = "";
    char ch[2] = { getch(), 0 };
    bool point = false;

    while (regex_match(ch, re))
    {
        if (ch[0] == '.')
        {
            if (point)
                throw LexerError("Unexpected . in number token");

            point = true;
        }

        tok.push_back(ch[0]);
        ch[0] = getch();
    }

    input.insert(input.begin(), ch[0]);

    if (tok.size() > 0)
    {
        double n = stod(tok);
        tokens.push_back(new LexerToken(LexerToken::Number, n));
        return true;
    }
    else
    {
        return false;
    }
}

bool Lexer::commentRule()
{
    if (input.size() < 3 || input.substr(0, 3).compare("REM") != 0)
        return false;

    string tok = "";
    char ch;

    input = input.substr(3);

    do
    {
        ch = getch();
        if (ch != '\n')
            tok.push_back(ch);

    } while (ch != '\n');

    LexerToken *lexerToken = new LexerToken(LexerToken::Comment, tok);
    tokens.push_back(lexerToken);

    return true;
}

bool Lexer::symbolRule(const std::string &str, LexerToken::Type type)
{
    int size = str.size();

    if (input.size() >= size && input.substr(0, size).compare(str) == 0)
    {
        string tok = input.substr(0, size);
        input = input.substr(size);
        tokens.push_back(new LexerToken(type, tok));
        return true;
    }
    else
    {
        return false;
    }
}

LexerToken *Lexer::getNextToken()
{
    bool done = false;

    // skip whitespace
    eatWhitespace();

    // if there are no characters left then finish
    if (input.size() == 0)
        return NULL;

    // run rules
    if (commentRule()) return tokens.back();
    if (identifierRule()) return tokens.back();
    if (numberRule()) return tokens.back();
    if (stringRule()) return tokens.back();

    if (symbolRule(";", LexerToken::EOL)) return tokens.back();
    if (symbolRule("<>", LexerToken::Ne)) return tokens.back();
    if (symbolRule("<=", LexerToken::Lte)) return tokens.back();
    if (symbolRule(">=", LexerToken::Gte)) return tokens.back();
    if (symbolRule("+", LexerToken::Add)) return tokens.back();
    if (symbolRule("-", LexerToken::Sub)) return tokens.back();
    if (symbolRule("*", LexerToken::Mul)) return tokens.back();
    if (symbolRule("/", LexerToken::Div)) return tokens.back();
    if (symbolRule(",", LexerToken::Comma)) return tokens.back();
    if (symbolRule("<", LexerToken::Lt)) return tokens.back();
    if (symbolRule(">", LexerToken::Gt)) return tokens.back();
    if (symbolRule("=", LexerToken::Eq)) return tokens.back();
    if (symbolRule(";", LexerToken::Semi)) return tokens.back();
    if (symbolRule("(", LexerToken::ParenL)) return tokens.back();
    if (symbolRule(")", LexerToken::ParenR)) return tokens.back();
    if (symbolRule("[", LexerToken::BracketL)) return tokens.back();
    if (symbolRule("]", LexerToken::BracketR)) return tokens.back();

    // if no rules matched then raise an error
    throw LexerError("Unexpected character: " + input.substr(0, 1));
}

