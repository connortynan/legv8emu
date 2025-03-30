#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>

namespace Opcode
{
    enum Type
    {
        B,
        FMULS,
        FDIVS,
        FCMPS,
        FADDS,
        FSUBS,
        FMULD,
        FDIVD,
        FCMPD,
        FADDD,
        FSUBD,
        STURB,
        LDURB,
        B_EQ,
        B_NE,
        B_LT,
        B_LE,
        B_GT,
        B_GE,
        B_LO,
        B_LS,
        B_HI,
        B_HS,
        B_MI,
        B_VS,
        STURH,
        LDURH,
        AND,
        ADD,
        ADDI,
        ANDI,
        BL,
        SDIV,
        UDIV,
        MUL,
        SMULH,
        UMULH,
        ORR,
        ADDS,
        ADDIS,
        ORRI,
        CBZ,
        CBNZ,
        STURW,
        LDURSW,
        STURS,
        LDURS,
        STXR,
        LDXR,
        EOR,
        SUB,
        SUBI,
        EORI,
        MOVZ,
        LSR,
        LSL,
        BR,
        ANDS,
        SUBS,
        SUBIS,
        ANDIS,
        MOVK,
        STUR,
        LDUR,
        STURD,
        LDURD,
        NONE
    };

    enum class Format
    {
        R,
        I,
        D,
        B,
        CB,
        IW,
        NONE
    };

    inline Type from_string(const std::string &str)
    {
        static const std::unordered_map<std::string, Type> lookup = {
            {"B", B},
            {"FMULS", FMULS},
            {"FDIVS", FDIVS},
            {"FCMPS", FCMPS},
            {"FADDS", FADDS},
            {"FSUBS", FSUBS},
            {"FMULD", FMULD},
            {"FDIVD", FDIVD},
            {"FCMPD", FCMPD},
            {"FADDD", FADDD},
            {"FSUBD", FSUBD},
            {"STURB", STURB},
            {"LDURB", LDURB},
            {"B.EQ", B_EQ},
            {"B.NE", B_NE},
            {"B.LT", B_LT},
            {"B.LE", B_LE},
            {"B.GT", B_GT},
            {"B.GE", B_GE},
            {"B.LO", B_LO},
            {"B.LS", B_LS},
            {"B.HI", B_HI},
            {"B.HS", B_HS},
            {"B.MI", B_MI},
            {"B.VS", B_VS},
            {"STURH", STURH},
            {"LDURH", LDURH},
            {"AND", AND},
            {"ADD", ADD},
            {"ADDI", ADDI},
            {"ANDI", ANDI},
            {"BL", BL},
            {"SDIV", SDIV},
            {"UDIV", UDIV},
            {"MUL", MUL},
            {"SMULH", SMULH},
            {"UMULH", UMULH},
            {"ORR", ORR},
            {"ADDS", ADDS},
            {"ADDIS", ADDIS},
            {"ORRI", ORRI},
            {"CBZ", CBZ},
            {"CBNZ", CBNZ},
            {"STURW", STURW},
            {"LDURSW", LDURSW},
            {"STURS", STURS},
            {"LDURS", LDURS},
            {"STXR", STXR},
            {"LDXR", LDXR},
            {"EOR", EOR},
            {"SUB", SUB},
            {"SUBI", SUBI},
            {"EORI", EORI},
            {"MOVZ", MOVZ},
            {"LSR", LSR},
            {"LSL", LSL},
            {"BR", BR},
            {"ANDS", ANDS},
            {"SUBS", SUBS},
            {"SUBIS", SUBIS},
            {"ANDIS", ANDIS},
            {"MOVK", MOVK},
            {"STUR", STUR},
            {"LDUR", LDUR},
            {"STURD", STURD},
            {"LDURD", LDURD}};

        auto it = lookup.find(str);
        if (it != lookup.end())
            return it->second;

        return NONE;
    }

    inline Format format(const Type &op)
    {
        switch (op)
        {
        // R format
        case Opcode::FMULS:
        case Opcode::FDIVS:
        case Opcode::FCMPS:
        case Opcode::FADDS:
        case Opcode::FSUBS:
        case Opcode::FMULD:
        case Opcode::FDIVD:
        case Opcode::FCMPD:
        case Opcode::FADDD:
        case Opcode::FSUBD:
        case Opcode::AND:
        case Opcode::ADD:
        case Opcode::SDIV:
        case Opcode::UDIV:
        case Opcode::MUL:
        case Opcode::SMULH:
        case Opcode::UMULH:
        case Opcode::ORR:
        case Opcode::ADDS:
        case Opcode::STURS:
        case Opcode::LDURS:
        case Opcode::EOR:
        case Opcode::SUB:
        case Opcode::LSR:
        case Opcode::LSL:
        case Opcode::BR:
        case Opcode::ANDS:
        case Opcode::SUBS:
        case Opcode::STURD:
        case Opcode::LDURD:
            return Format::R;

        // I format
        case Opcode::ADDI:
        case Opcode::ANDI:
        case Opcode::ADDIS:
        case Opcode::ORRI:
        case Opcode::SUBI:
        case Opcode::EORI:
        case Opcode::SUBIS:
        case Opcode::ANDIS:
            return Format::I;

        // D format
        case Opcode::STURB:
        case Opcode::LDURB:
        case Opcode::STURH:
        case Opcode::LDURH:
        case Opcode::STURW:
        case Opcode::LDURSW:
        case Opcode::STXR:
        case Opcode::LDXR:
        case Opcode::STUR:
        case Opcode::LDUR:
            return Format::D;

        // B format
        case Opcode::B:
        case Opcode::BL:
            return Format::B;

        // CB format
        case Opcode::B_EQ:
        case Opcode::B_NE:
        case Opcode::B_LT:
        case Opcode::B_LE:
        case Opcode::B_GT:
        case Opcode::B_GE:
        case Opcode::B_LO:
        case Opcode::B_LS:
        case Opcode::B_HI:
        case Opcode::B_HS:
        case Opcode::B_MI:
        case Opcode::B_VS:
        case Opcode::CBZ:
        case Opcode::CBNZ:
            return Format::CB;

        // IW format
        case Opcode::MOVZ:
        case Opcode::MOVK:
            return Format::IW;
        default:
            return Format::NONE;
        }
    }

    inline std::string to_string(Type op)
    {
        static const char *names[] = {
            "B", "FMULS", "FDIVS", "FCMPS", "FADDS", "FSUBS",
            "FMULD", "FDIVD", "FCMPD", "FADDD", "FSUBD",
            "STURB", "LDURB", "B.EQ", "B.NE", "B.LT", "B.LE", "B.GT", "B.GE",
            "B.LO", "B.LS", "B.HI", "B.HS", "B.MI", "B.VS",
            "STURH", "LDURH", "AND", "ADD", "ADDI", "ANDI", "BL", "SDIV",
            "UDIV", "MUL", "SMULH", "UMULH", "ORR", "ADDS", "ADDIS", "ORRI",
            "CBZ", "CBNZ", "STURW", "LDURSW", "STURS", "LDURS", "STXR", "LDXR",
            "EOR", "SUB", "SUBI", "EORI", "MOVZ", "LSR", "LSL", "BR",
            "ANDS", "SUBS", "SUBIS", "ANDIS", "MOVK", "STUR", "LDUR",
            "STURD", "LDURD", "NONE"};

        if (op < 0 || op > NONE)
            return "UNKNOWN";

        return names[static_cast<int>(op)];
    }

} // namespace Opcode