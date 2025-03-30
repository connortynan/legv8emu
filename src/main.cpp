#include "parser.hpp"
#include "decoder.hpp"

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[])
{
    std::string filepath;
    if (argc < 2)
    {
        filepath = "tests/heapsort.legv8asm";
    }
    else
    {
        filepath = argv[1];
    }
    std::ifstream infile(filepath);
    if (!infile)
    {
        std::cerr << "Failed to open " << filepath << std::endl;
        return 1;
    }

    try
    {
        std::vector<Parser::Token> tokens = Parser::parse(infile);

        std::cout << "--- TOKENS ---\n";
        for (const auto &token : tokens)
        {
            std::cout << token << '\n';
        }

        std::vector<Decoder::Instruction> instructions = Decoder::decode(tokens);

        std::cout << "\n--- INSTRUCTIONS ---\n";
        for (const auto &instr : instructions)
        {
            std::cout << instr << '\n';
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
