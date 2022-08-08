
#ifndef __LEXER_HPP__
#define __LEXER_HPP__

#include <string>
#include <vector>
#include <map>

#include "../error.hpp"

namespace rbasic::lexer
{
    class LexerError : public BaseError
    {
    public:
        LexerError(const std::string &msg);
    };

    class LexerToken
    {
    public:
        enum Type
        {
            EOL,
            Identifier,
            String,
            Comment,
            Number,
            Lt,
            Gt,
            Lte,
            Gte,
            Eq,
            Ne,
            Semi,
            ParenL,
            ParenR,
            BracketL,
            BracketR,
            Comma,
            Add,
            Sub,
            Mul,
            Div,
        } type;

        std::string strValue;
        double numValue;
        int lineNum;

        LexerToken();
        LexerToken(Type t);
        LexerToken(Type t, const std::string &str);
        LexerToken(Type t, double num);
        ~LexerToken();

        std::string toString();
    };

    class Lexer
    {
    private:
        std::string input;
        std::vector<LexerToken*> tokens;

        void clearTokens();

        char getch();
        void eatWhitespace();
        bool commentRule();
        bool identifierRule();
        bool numberRule();
        bool stringRule();
        bool symbolRule(const std::string &str, LexerToken::Type type);

    public:
        Lexer();
        Lexer(const std::string &src);
        ~Lexer();

        void setInput(const std::string &src);
        LexerToken *getNextToken();
    };
}

#endif