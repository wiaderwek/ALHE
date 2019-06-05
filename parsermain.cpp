#include "miniParser.hpp"

int main(int argc, char* argv[])
{
    if(argc < 1) return 1;

    FileSource source(argv[1]);
    Lexer lexer(source);
    MiniParser parser(lexer);

    parser.parse();

    return 0;
}