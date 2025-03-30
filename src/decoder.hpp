#pragma once

#include <vector>

#include "parser.hpp"
#include "opcodes.hpp"
#include "registers.hpp"

namespace Decoder
{
    struct Operand
    {
        union
        {
            Register::Name reg;
            int imm;
        };
        bool is_reg;

        Operand(Register::Name r) : reg(r), is_reg(true) {}
        Operand(int i) : imm(i), is_reg(false) {}
    };

    struct Instruction
    {
        Opcode::Type opcode;
        Opcode::Format format;

        union
        {
            struct
            {
                Operand Rm, shamt, Rn, Rd;
            } R;
            struct
            {
                Operand imm, Rn, Rd;
            } I;
            struct
            {
                Operand Rt, Rn, offset;
            } D;
            struct
            {
                Operand label;
            } B;
            struct
            {
                Operand Rt, label;
            } CB;
            struct
            {
                Operand imm, Rd, shift;
            } IW;
        };

        Instruction(Opcode::Type op) : opcode(op), format(Opcode::format(op)) {}
    };

    std::vector<Instruction> decode(const std::vector<Parser::Token> &tokens);

    std::ostream &operator<<(std::ostream &os, const Operand &operand);
    std::ostream &operator<<(std::ostream &os, const Instruction &instruction);
}
