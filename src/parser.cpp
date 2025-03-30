#include "parser.hpp"

#include <exception>
#include <sstream>

namespace Parser
{
    std::vector<Token> parse(std::istream &in)
    {
        std::vector<Token> tokens;

        std::string line;
        int line_number = 0;

        while (std::getline(in, line))
        {
            ++line_number;

            std::size_t comment_pos = line.find('/');
            if (comment_pos == std::string::npos)
                comment_pos = line.size();

            std::string lexeme;
            bool label_valid = true;
            bool instruction_valid = true;

            std::size_t i;
            char ch;
            for (i = 0; i < comment_pos; i++)
            {
                ch = line[i];
                if (ch == ':')
                {
                    if (lexeme.empty())
                    {
                        throw std::runtime_error("Line " + std::to_string(line_number) + ": Error: empty label");
                    }
                    if (!label_valid)
                    {
                        throw std::runtime_error("Line " + std::to_string(line_number) + ": Error: multiple labels on the same line");
                    }
                    tokens.push_back({TOKEN_LABEL, lexeme, line_number});
                    lexeme.clear();
                    label_valid = false;
                }
                else if (std::isspace(ch) && instruction_valid)
                {
                    if (!lexeme.empty())
                    {
                        tokens.push_back({TOKEN_INSTRUCTION, lexeme, line_number});
                        lexeme.clear();
                        instruction_valid = false;
                        label_valid = false;
                    }
                }
                else if (std::isspace(ch) || ch == ',')
                {
                    if (!lexeme.empty())
                    {
                        if (std::isspace(ch))
                        {
                            std::size_t next_nonspace = i;
                            while (next_nonspace < comment_pos && std::isspace(line[next_nonspace]))
                                next_nonspace++;
                            if (next_nonspace < comment_pos && line[next_nonspace] != ',')
                            {
                                throw std::runtime_error("Line " + std::to_string(line_number) + ": Error: Expected comma-separated operands");
                            }
                            tokens.push_back({TOKEN_OPERAND, lexeme, line_number});
                            lexeme.clear();
                            i = next_nonspace - 1; // since i will be incremented from for loop
                        }
                        else // ','
                        {
                            tokens.push_back({TOKEN_OPERAND, lexeme, line_number});
                            lexeme.clear();
                        }
                    }
                }
                else
                {
                    lexeme += ch;
                }
            }

            if (!lexeme.empty())
            {
                tokens.push_back({TOKEN_OPERAND, lexeme, line_number});
            }
        }

        return tokens;
    }
} // namespace Parser
