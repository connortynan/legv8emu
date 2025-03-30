#include "decoder.hpp"

#include <exception>

namespace Decoder
{
    Operand instruction_offset(
        const Parser::Token &label_token,
        const std::unordered_map<std::string, std::size_t> &labels,
        std::size_t current_instruction)
    {
        const std::string &label = label_token.lexeme;
        auto it = labels.find(label);
        if (it == labels.end())
        {
            throw std::runtime_error("Line " + std::to_string(label_token.line) + ": Error: unknown label '" + label + "'");
        }

        int offset = static_cast<int>(it->second) - static_cast<int>(current_instruction);
        return Operand(offset); // treated as immediate
    }

    Operand decode_operand(const Parser::Token &token)
    {
        if (token.type != Parser::TOKEN_OPERAND)
            throw std::runtime_error("Line " + std::to_string(token.line) + ": Internal Error: attempted operand decoding on non operand token (" + token.lexeme + ")");
        if (token.lexeme.empty())
            throw std::runtime_error("Line " + std::to_string(token.line) + ": Internal Error: empty operand");

        if (token.lexeme[0] == '#')
        {
            return Operand{std::stoi(token.lexeme.substr(1), nullptr, 0)};
        }
        else // should be register
        {
            Register::Name reg = Register::from_string(token.lexeme);
            if (reg == Register::NONE)
                throw std::runtime_error("Line " + std::to_string(token.line) + ": Error: expected register name, got (" + token.lexeme + ")");
            return Operand{reg};
        }
    }

    // has_offset passed in determines if the token should be the immediate, and changes only in the case of an implicit #0 immediate
    // e.g., `LDUR X0, [X4]`
    Operand bracketed_operand(const Parser::Token &token, bool &has_offset)
    {
        if (has_offset) // should be immediate
        {
            if (token.lexeme[0] != '#' || token.lexeme.back() != ']')
                throw std::runtime_error("Line " + std::to_string(token.line) + ": Error: expected immediate offset, got (" + token.lexeme + ")");
            return Operand{std::stoi(token.lexeme.substr(1), nullptr, 0)};
        }
        else // should be register
        {
            const std::size_t rb = token.lexeme.find(']');
            if (rb != std::string::npos) // implicit offset
            {
                Register::Name reg = Register::from_string(token.lexeme.substr(1, rb));
                if (reg == Register::NONE)
                    throw std::runtime_error("Line " + std::to_string(token.line) + ": Error: expected register name, got (" + token.lexeme + ")");
                has_offset = true;
                return Operand{reg};
            }
            else // next token is the offset
            {
                Register::Name reg = Register::from_string(token.lexeme.substr(1));
                if (reg == Register::NONE)
                    throw std::runtime_error("Line " + std::to_string(token.line) + ": Error: expected register name, got (" + token.lexeme + ")");
                return Operand{reg};
            }
        }
    }

    Operand expect_operand(const Parser::Token &token, bool should_be_reg)
    {
        Operand op = decode_operand(token);
        if (op.is_reg && !should_be_reg)
            throw std::runtime_error("Line " + std::to_string(token.line) + ": Error: expected register operand (" + token.lexeme + ")");
        else if (!op.is_reg && should_be_reg)
            throw std::runtime_error("Line " + std::to_string(token.line) + ": Error: expected immediate operand (" + token.lexeme + ")");
        return op;
    }

    std::vector<Instruction> decode(const std::vector<Parser::Token> &tokens)
    {
        // first pass: create label table
        std::unordered_map<std::string, std::size_t> labels;
        std::size_t instruction_num = 0;
        for (const auto &t : tokens)
        {
            if (t.type == Parser::TOKEN_LABEL)
            {
                labels[t.lexeme] = instruction_num;
            }
            else if (t.type == Parser::TOKEN_INSTRUCTION)
            {
                instruction_num++;
            }
        }

        // second pass: validate and structure instructions
        std::vector<Instruction> instructions;
        instruction_num = 0;
        for (std::size_t i = 0; i < tokens.size(); i++)
        {
            if (tokens[i].type == Parser::TOKEN_INSTRUCTION)
            {
                Opcode::Type opcode = Opcode::from_string(tokens[i].lexeme);
                if (opcode == Opcode::NONE)
                    throw std::runtime_error("Line " + std::to_string(tokens[i].line) + ": Error: unexpected opcode (" + tokens[i].lexeme + ")");

                Instruction instruction(opcode);

                switch (instruction.format)
                {
                case Opcode::Format::R:
                {
                    if (opcode == Opcode::LSL || opcode == Opcode::LSR)
                    {
                        instruction.R.Rd = decode_operand(tokens[++i]);
                        instruction.R.Rn = decode_operand(tokens[++i]);
                        instruction.R.shamt = decode_operand(tokens[++i]);
                        instruction.R.Rm = Operand(Register::XZR); // unused
                    }
                    else if (opcode == Opcode::BR)
                    {
                        // Only 1 operand: target register (treated like Rn)
                        instruction.R.Rd = Operand(Register::XZR);      // unused
                        instruction.R.Rm = Operand(Register::XZR);      // unused
                        instruction.R.shamt = Operand(0);               // unused
                        instruction.R.Rn = decode_operand(tokens[++i]); // actual register
                    }
                    else
                    {
                        instruction.R.Rd = decode_operand(tokens[++i]);
                        instruction.R.Rn = decode_operand(tokens[++i]);
                        instruction.R.Rm = decode_operand(tokens[++i]);
                        instruction.R.shamt = Operand(0); // unused
                    }
                }
                break;

                case Opcode::Format::I:
                {
                    instruction.I.Rd = decode_operand(tokens[++i]);
                    instruction.I.Rn = decode_operand(tokens[++i]);
                    instruction.I.imm = decode_operand(tokens[++i]);
                }
                break;

                case Opcode::Format::D:
                {
                    instruction.D.Rt = decode_operand(tokens[++i]);

                    bool has_offset = false;
                    instruction.D.Rn = bracketed_operand(tokens[++i], has_offset);

                    if (has_offset)
                        instruction.D.offset = Operand{0};
                    else
                    {
                        has_offset = true;
                        instruction.D.offset = bracketed_operand(tokens[++i], has_offset);
                    }
                }
                break;

                case Opcode::Format::B:
                {
                    instruction.B.label = instruction_offset(tokens[++i], labels, instruction_num);
                }
                break;

                case Opcode::Format::CB:
                {
                    if (opcode == Opcode::CBZ || opcode == Opcode::CBNZ)
                    {
                        instruction.CB.Rt = decode_operand(tokens[++i]);
                        instruction.CB.label = instruction_offset(tokens[++i], labels, instruction_num);
                    }
                    else // B.cond types like B.EQ, B.GT, etc.
                    {
                        instruction.CB.Rt = Operand(Register::XZR);
                        instruction.CB.label = instruction_offset(tokens[++i], labels, instruction_num);
                    }
                }
                break;

                case Opcode::Format::IW:
                {
                    instruction.IW.Rd = decode_operand(tokens[++i]);
                    instruction.IW.imm = decode_operand(tokens[++i]);
                    instruction.IW.shift = decode_operand(tokens[++i]);
                }
                break;

                default:
                    throw std::runtime_error("Line " + std::to_string(tokens[i].line) + ": Internal Error: unexpected opcode format (" + tokens[i].lexeme + ")");
                }

                instructions.push_back(instruction);
                instruction_num++;
            }
            else if (tokens[i].type == Parser::TOKEN_OPERAND)
                throw std::runtime_error("Line " + std::to_string(tokens[i].line) + ": Error: unexpected operand (" + tokens[i].lexeme + ")");
        }
        return instructions;
    }

    std::ostream &operator<<(std::ostream &os, const Operand &operand)
    {
        if (operand.is_reg)
            os << Register::to_string(operand.reg); // You need to implement this too
        else
            os << "#" << operand.imm;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const Instruction &inst)
    {
        os << Opcode::to_string(inst.opcode); // You need a to_string for opcodes

        switch (inst.format)
        {
        case Opcode::Format::R:
            os << " " << inst.R.Rd << ", " << inst.R.Rn << ", ";
            if (inst.opcode == Opcode::LSL || inst.opcode == Opcode::LSR)
                os << inst.R.shamt;
            else
                os << inst.R.Rm;
            break;

        case Opcode::Format::I:
            os << " " << inst.I.Rd << ", " << inst.I.Rn << ", " << inst.I.imm;
            break;

        case Opcode::Format::D:
            os << " " << inst.D.Rt << ", [" << inst.D.Rn << ", " << inst.D.offset << "]";
            break;

        case Opcode::Format::B:
            os << " " << inst.B.label;
            break;

        case Opcode::Format::CB:
            os << " " << inst.CB.Rt << ", " << inst.CB.label;
            break;

        case Opcode::Format::IW:
            os << " " << inst.IW.Rd << ", " << inst.IW.imm << ", LSL " << inst.IW.shift;
            break;

        default:
            os << " UNKNOWN FORMAT ";
            break;
        }

        return os;
    }

} // namespace Decoder
