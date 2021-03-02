#include <iostream>
#include <fstream>
#include <cstring>

#pragma warning(push, 0)
#include "llvm/Support/CommandLine.h"
#pragma warning(pop)

#include "lexer.h"
#include "Parser.h"
#include "CodeGenerator.h"
using namespace std;

static cl::opt<string> inputFile(cl::Positional, cl::desc("<input file>"), cl::Required);
static cl::opt<string> outputFile("o", cl::desc("Output file name"), cl::value_desc("filename"), cl::init("a.out"));
static cl::opt<bool> dumpTokens("dump-tokens", cl::desc("Run preprocessor, dump internal rep of tokens"));
static cl::opt<bool> dumpAST("dump-ast", cl::desc("Run parser, dump AST"));

void usage(const char* exeName)
{
    cout << "Usage : " << exeName << " <inputfile> <outputfile>" << endl;
}

int main(int argc, char* argv[])
{
    cl::ParseCommandLineOptions(argc, argv);
    ifstream fin(inputFile);
    if (!fin.is_open())
    {
        cout << "Can't open " << inputFile << endl;
        return 0;
    }

    Lexer* lexer = new Lexer(fin);
    auto tokens = lexer->Lex();
    fin.close();

    if (dumpTokens)
    {
        json res = json::array();
        for (size_t i = 0; i < tokens.size(); i++)
        {
            res.push_back(json({
                    {"id",i + 1},
                    {"content",tokens[i].content},
                    {"prop",tokens[i].getKindName()},
                    {"loc",to_string(tokens[i].loc.row) + "," + to_string(tokens[i].loc.col)}
                }));
        }
        std::cout << res.dump(4) << std::endl;
        return 0;
    }
    Parser* p = new Parser(tokens, inputFile);
    auto res = p->parse();
    if (!res)
    {
        std::cerr << "Error";
        return 0;
    }
    if (dumpAST)
    {
        std::cout << res->toJson().dump(4) << std::endl;
        return 0;
    }
    CodeGenerator* generator = new CodeGenerator();
    res->genCode(*generator);
    generator->print(outputFile.getValue().c_str());
    return 0;
}
