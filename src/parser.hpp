#pragma once

#include <istream>
#include <vector>
#include <string>

namespace Parser
{
    enum TokenType
    {
        TOKEN_LABEL,
        TOKEN_INSTRUCTION,
        TOKEN_OPERAND,
        TOKEN_COMMENTS,
    };

    struct Token
    {
        TokenType type;
        std::string lexeme;
        int line;
    };

    inline const char *to_string(TokenType type)
    {
        switch (type)
        {
        case TOKEN_LABEL:
            return "LABEL";
        case TOKEN_INSTRUCTION:
            return "INSTRUCTION";
        case TOKEN_OPERAND:
            return "OPERAND";
        case TOKEN_COMMENTS:
            return "COMMENT";
        default:
            return "UNKNOWN";
        }
    }

    inline std::ostream &operator<<(std::ostream &os, const Token &token)
    {
        return os << "line " << token.line << ": " << to_string(token.type) << ", " << token.lexeme;
    }

    std::vector<Token> parse(std::istream &in);
} // namespace Parser