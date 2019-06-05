#ifndef MINIPARSER_HPP
#define MINIPARSER_HPP

#include <map>
#include <fstream>

class FileSource
{
    private:
        std::ifstream file;

        char character = '\0';

    public:
        bool isEOF() const { return file.eof(); }

        char getCharacter() const { return character; }
        char nextChar();

        FileSource(std::string filename)
        {
            file.open(filename);
            if(!file.good()) throw std::runtime_error("Bad file!");
        }

        ~FileSource() { file.close(); }
};

class Lexer
{
    private:
        FileSource &source;

        bool isDigit(char c) const
        {
            return c >= '0' && c <= '9';
        }

        bool isLetter(char c) const
        {
            return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_');
        }

        char skipWhitespaces();

    public:
        std::string getString();
        double getNumber();

        Lexer(FileSource &s):
            source(s) { source.nextChar(); }
};

class MiniParser
{
    private:
        Lexer& lex;

        std::map<std::string, int> vert_map;

        void parseNodes();
        void parseEdges();
        void parseDemands();

    public:
        void parse();

        MiniParser(Lexer &l) : lex(l)
        {}
};

#endif //MINIPARSER_HPP