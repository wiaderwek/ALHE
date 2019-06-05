#include <iostream>
#include <string>
#include "miniParser.hpp"

char FileSource::nextChar()
{
    if(file.eof()) return character = std::char_traits<char>::eof();//EOF;

    file.get(character);

    return character;
}

char Lexer::skipWhitespaces()
{
    while(!source.isEOF() && isspace(source.getCharacter())) source.nextChar();
    return source.getCharacter();
}

std::string Lexer::getString()
{
    std::string ret;

    skipWhitespaces();

    while(!source.isEOF() && !isspace(source.getCharacter())) 
    {
        ret = ret + source.getCharacter();
        source.nextChar();
    }

    if(source.isEOF()) return "\0";

    return ret;
}

double Lexer::getNumber()
{
    char c = source.getCharacter();

    int i = c - '0';
    while(isDigit(c = source.nextChar()))
    {
        i *= 10;
        i += (c - '0');
    }

    if(c != '.') return (double)i;

    double d = 0, j = 10;
    while(isDigit(c = source.nextChar()))
    {
        double temp = (c - '0') / j;
        d += temp;
        j *= 10;
    }

    return i+d;
}

void MiniParser::parse()
{
    std::string token = lex.getString();

    if(token != "NODES") return;

    lex.getString();

    std::ofstream verts;
    verts.open("ver.txt", std::ios::trunc);

    int j = 0;
    while((token = lex.getString()) != "\0" && (token) != ")")
    {
        std::cout << token << " = " << j << std::endl;

        verts << j << '\n';
        vert_map[token] = j++;
        
        for(int i=0;i<4;++i) lex.getString();
    }
    verts.close();
//-----------------------------------------------------
    token = lex.getString();
    if(token != "LINKS") return;

    lex.getString();

    std::ofstream links;
    links.open("edges.txt", std::ios::trunc);

    while((token = lex.getString()) != "\0" && token != ")")
    {
        for(int i=0;i<2;++i) token = lex.getString();

        std::cout << "Link: "<< token << " ";

        links << vert_map[token] << ' ';
        links << vert_map[token = lex.getString()] << '\n';

        std::cout << token << std::endl;

        for(int i=0;i<11;++i) lex.getString();
    }
    links.close();
//-----------------------------------------------------
    token = lex.getString();
    if(token != "DEMANDS") return;

    lex.getString();

    std::ofstream demands;
    demands.open("demands.txt", std::ios::trunc);

    while((token = lex.getString()) != "\0" && token != ")")
    {
        for(int i=0;i<2;++i) token = lex.getString();

        std::cout << "Demand: "<< token << " ";

        demands << vert_map[token] << ' ';
        demands << vert_map[token = lex.getString()] << ' ';

        std::cout << token << " ";

        for(int i=0;i<2;++i) lex.getString();

        token = lex.getString();
        token.erase(token.end()-3, token.end());

        demands << token << '\n';
        std::cout << token << std::endl;

        for(int i=0;i<1;++i) lex.getString();
    }
    demands.close();
}