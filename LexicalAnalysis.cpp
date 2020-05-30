#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cctype>
using namespace std;
string keywords[34] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "inline", "int", "long", "register", "restrict", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
enum DFA_STATE
{
    INITIAL,
    CONST,
    LU_CONST,
    DIGIT,
    OX_DIGIT,
    EQUAL,
    ADD,
    MINUS,
    MUL,
    DIV,
    MOD,
    NOT,
    AND,
    OR,
    XOR,
    GREATER,
    GREATER2,
    LESS,
    LESS2,
    IDENTIFIER,
    LU_STRING,
    STRING
};
int lIndex = 0;
int cIndex = 0;
int totalcIndex = 0;
vector<string> srcLines;
char getNextChar()
{
    char c = SCHAR_MAX;
    while (true)
    {
        if (lIndex < srcLines.size())
        {
            string line = srcLines.at(lIndex);
            if (cIndex < line.length())
            {
                c = line.at(cIndex);
                cIndex++;
                totalcIndex++;
                break;
            }
            else
            {
                lIndex++;
                totalcIndex += 2;
                cIndex = 0;
            }
        }
        else
        {
            break;
        }
    }
    return c;
}
bool isKeyword(string s)
{
    for (int i = 0; i < 34; i++)
    {
        if (keywords[i] == s)
            return true;
    }
    return false;
}
string generatorToken3(int num, string lexeme, string type, int cIndex, int lIndex, int totalcIndex)
{
    string strToken = "";

    strToken += "[@" + to_string(num) + "," + to_string(totalcIndex - lexeme.length() + 1) + ":" + to_string(totalcIndex);
    strToken += "='" + lexeme + "',<" + type + ">," + to_string(lIndex + 1) + ":" + to_string(cIndex - lexeme.length() + 1) + "]\n";

    return strToken;
}
string generatorToken(int num, string lexeme, string type)
{
    return generatorToken3(num, lexeme, type, cIndex - 1, lIndex, totalcIndex - 1);
}
string generatorToken2(int num, string lexeme, string type)
{
    return generatorToken3(num, lexeme, type, cIndex - 2, lIndex, totalcIndex - 2);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Input error!" << endl;
        return 0;
    }
    ifstream in(argv[1]); //打开文件
    if (!in)
    {
        cerr << "Can't open file inputed!" << endl;
        return 0;
    }
    string line;
    while (getline(in, line)) //从文件中读取一行
    {
        srcLines.push_back(line); //添加到vector中
    }
    in.close();
    cout << "Scanning..." << endl;
    string tokenStream = ""; //属性字流
    int tokenNum = 0;        //属性字序号
    DFA_STATE state = INITIAL;
    string lexeme = ""; //token
    char c = ' ';       //暂存字符
    bool keep = false;  //超前搜索标识
    bool end = false;
    while (!end)
    {
        if (!keep)
        {
            c = getNextChar();
        }
        keep = false;
        switch (state)
        {
        case INITIAL:
            lexeme = "";

            if (c == '_' || (c != 'L' && c != 'U' && c != 'l' && c != 'u' && isalpha(c)))
            {
                state = IDENTIFIER;
                lexeme = lexeme + c;
            }
            else if (c == 'u' || c == 'U' || c == 'l' || c == 'L') //U'a'
            {
                state = LU_CONST;
                lexeme = lexeme + c;
            }
            else if (c == '0') //0x00
            {
                state = OX_DIGIT;
                lexeme = lexeme + c;
            }
            else if (c != '0' && isdigit(c))
            {
                state = DIGIT;
                lexeme = lexeme + c;
            }
            else if (c == '+')
            {
                state = ADD;
                lexeme = lexeme + c;
            }
            else if (c == '-')
            {
                state = MINUS;
                lexeme = lexeme + c;
            }
            else if (c == '*')
            {
                state = MUL;
                lexeme = lexeme + c;
            }
            else if (c == '/')
            {
                state = DIV;
                lexeme = lexeme + c;
            }
            else if (c == '%')
            {
                state = MOD;
                lexeme = lexeme + c;
            }
            else if (c == '=')
            {
                state = EQUAL;
                lexeme = lexeme + c;
            }
            else if (c == '{')
            {
                tokenStream += generatorToken(tokenNum, "{", "'{'");
                tokenNum++;
                state = INITIAL;
            }
            else if (c == '}')
            {
                tokenStream += generatorToken(tokenNum, "}", "'}'");
                tokenNum++;
                state = INITIAL;
            }
            else if (c == '[')
            {
                tokenStream += generatorToken(tokenNum, "[", "'['");
                tokenNum++;
                state = INITIAL;
            }
            else if (c == ']')
            {
                tokenStream += generatorToken(tokenNum, "]", "']'");
                tokenNum++;
                state = INITIAL;
            }
            else if (c == '(')
            {
                tokenStream += generatorToken(tokenNum, "(", "'('");
                tokenNum++;
                state = INITIAL;
            }
            else if (c == ')')
            {
                tokenStream += generatorToken(tokenNum, ")", "')'");
                tokenNum++;
                state = INITIAL;
            }
            else if (c == ';')
            {
                tokenStream += generatorToken(tokenNum, ";", "';'");
                tokenNum++;
                state = INITIAL;
            }
            else if (c == ',')
            {
                tokenStream += generatorToken(tokenNum, ",", "','");
                tokenNum++;
                state = INITIAL;
            }
            else if (c == ':')
            {
                tokenStream += generatorToken(tokenNum, ":", "':'");
                tokenNum++;
                state = INITIAL;
            }
            else if (c == '?')
            {
                tokenStream += generatorToken(tokenNum, "?", "'?'");
                tokenNum++;
                state = INITIAL;
            }
            else if (c == '.')
            {
                tokenStream += generatorToken(tokenNum, ".", "'.'");
                tokenNum++;
                state = INITIAL;
            }
            else if (c == '!')
            {
                state = NOT;
                lexeme = lexeme + c;
            }
            else if (c == '&')
            {
                state = AND;
                lexeme = lexeme + c;
            }
            else if (c == '|')
            {
                state = OR;
                lexeme = lexeme + c;
            }
            else if (c == '^')
            {
                state = XOR;
                lexeme = lexeme + c;
            }
            else if (c == '>')
            {
                state = GREATER;
                lexeme = lexeme + c;
            }
            else if (c == '<')
            {
                state = LESS;
                lexeme = lexeme + c;
            }
            else if (c == '"')
            {
                state = STRING;
                lexeme = lexeme + c;
            }
            else if (c == '\'')
            {
                state = CONST;
                lexeme = lexeme + c;
            }
            else if (c == ' ')
            {
            }
            else if (c == SCHAR_MAX)
            {
                cIndex = 5;
                totalcIndex += cIndex;
                tokenStream += generatorToken(tokenNum, "<EOF>", "EOF");
                end = true;
            }
            break;
        case IDENTIFIER:
            if (isalpha(c) || isdigit(c) || c == '_')
            {
                state = IDENTIFIER;
                lexeme = lexeme + c;
            }
            else
            {
                if (lexeme == "do" || lexeme == "else")
                {
                    tokenStream += generatorToken3(tokenNum, lexeme, "'" + lexeme + "'", lexeme.length(), lIndex - 1, totalcIndex - 2);
                }
                else if (isKeyword(lexeme))
                {
                    tokenStream += generatorToken2(tokenNum, lexeme, "'" + lexeme + "'");
                }
                else
                {
                    tokenStream += generatorToken2(tokenNum, lexeme, "Identifier");
                }
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            break;
        case LU_CONST:
            if (c == '\'')
            {
                state = CONST;
                lexeme = lexeme + c;
            }
            else if (c == '"')
            {
                state = STRING;
                lexeme = lexeme + c;
            }
            else if (c == '8')
            {
                state = LU_STRING;
                lexeme = lexeme + c;
            }
            else
            {
                state = IDENTIFIER;
                lexeme = lexeme + c;
            }
            break;
        case CONST:
            if (c != '\'')
            {
                state = CONST;
                lexeme = lexeme + c;
            }
            else
            {
                lexeme = lexeme + c;
                tokenStream += generatorToken(tokenNum, lexeme, "Constant");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            break;
        case OX_DIGIT:
            if (c == 'X' || c == 'x' || c == '.')
            {
                state = DIGIT;
                lexeme = lexeme + c;
            }
            else if (!isdigit(c))
            {
                tokenStream += generatorToken2(tokenNum, lexeme, "Constant");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            else
            {
                state = DIGIT;
                lexeme = lexeme + c;
            }
            break;
        case DIGIT:
            if (isdigit(c) || c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F' || c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == '.' || c == 'L' || c == 'l' || c == 'U' || c == 'u' || c == 'p' || c == 'P')
            {
                state = DIGIT;
                lexeme = lexeme + c;
            }
            else
            {
                tokenStream += generatorToken2(tokenNum, lexeme, "Constant");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            break;
        case EQUAL:
            if (c == '=')
            {
                tokenStream += generatorToken(tokenNum, "==", "'=='");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else
            {
                tokenStream += generatorToken2(tokenNum, "=", "'='");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            state = INITIAL;
            break;
        case ADD:
            if (c == '+') //说明是++
            {
                tokenStream += generatorToken(tokenNum, "++", "'++'");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else if (c == '=') //+=
            {
                tokenStream += generatorToken(tokenNum, "+=", "'+='");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else //+
            {
                tokenStream += generatorToken2(tokenNum, "+", "'+'");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            state = INITIAL;
            break;
        case MINUS:
            if (c == '-') //说明是--
            {
                tokenStream += generatorToken(tokenNum, "--", "'--'");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else if (c == '=') //-=
            {
                tokenStream += generatorToken(tokenNum, "-=", "'-='");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else if (c == '>') //->
            {
                tokenStream += generatorToken(tokenNum, "->", "'->'");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else //-
            {
                tokenStream += generatorToken2(tokenNum, "-", "'-'");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            state = INITIAL;
            break;
        case MUL:
            if (c == '=') //*=
            {
                tokenStream += generatorToken(tokenNum, "*=", "'*='");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else //*
            {
                tokenStream += generatorToken2(tokenNum, "*", "'*'");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            state = INITIAL;
            break;
        case DIV:
            if (c == '=') // /=
            {
                tokenStream += generatorToken(tokenNum, "/=", "'/='");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else // /
            {
                tokenStream += generatorToken2(tokenNum, "/", "'/'");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            state = INITIAL;
            break;
        case MOD:
            if (c == '=') //%=
            {
                tokenStream += generatorToken(tokenNum, "%=", "'%='");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else //%
            {
                tokenStream += generatorToken2(tokenNum, "%", "'%'");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            state = INITIAL;
            break;
        case NOT:
            if (c == '=') //!=
            {
                tokenStream += generatorToken(tokenNum, "!=", "'!='");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else //!
            {
                tokenStream += generatorToken2(tokenNum, "!", "'!'");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            state = INITIAL;
            break;
        case AND:
            if (c == '&') //说明是&&
            {
                tokenStream += generatorToken(tokenNum, "&&", "'&&'");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else if (c == '=') //&=
            {
                tokenStream += generatorToken(tokenNum, "&=", "'&='");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else //&
            {
                tokenStream += generatorToken2(tokenNum, "&", "'&'");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            state = INITIAL;
            break;
        case OR:
            if (c == '|')
            {
                tokenStream += generatorToken(tokenNum, "||", "'||'");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else if (c == '=')
            {
                tokenStream += generatorToken(tokenNum, "|=", "'|='");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else
            {
                tokenStream += generatorToken2(tokenNum, "|", "'|'");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            state = INITIAL;
            break;
        case XOR:
            if (c == '=')
            {
                tokenStream += generatorToken(tokenNum, "^=", "'^='");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else
            {
                tokenStream += generatorToken2(tokenNum, "^", "'^'");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            state = INITIAL;
            break;
        case GREATER:
            if (c == '>')
            {
                state = GREATER2;
                lexeme = lexeme + c;
            }
            else if (c == '=')
            {
                tokenStream += generatorToken(tokenNum, ">=", "'>='");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else
            {
                tokenStream += generatorToken2(tokenNum, ">", "'>'");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            break;
        case GREATER2:
            if (c == '=')
            {
                tokenStream += generatorToken(tokenNum, ">>=", "'>>='");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else
            {
                tokenStream += generatorToken2(tokenNum, ">>", "'>>'");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            break;
        case LESS:
            if (c == '<')
            {
                state = LESS2;
                lexeme = lexeme + c;
            }
            else if (c == '=')
            {
                tokenStream += generatorToken(tokenNum, "<=", "'<='");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else
            {
                tokenStream += generatorToken2(tokenNum, "<", "'<'");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            break;
        case LESS2:
            if (c == '=')
            {
                tokenStream += generatorToken(tokenNum, "<<=", "'<<='");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            else
            {
                tokenStream += generatorToken2(tokenNum, "<<", "'<<'");
                tokenNum++;
                state = INITIAL;
                keep = true;
            }
            break;
        case LU_STRING:
            if (c == '"')
            {
                state = STRING;
                lexeme = lexeme + c;
            }
            else
            {
                state = IDENTIFIER;
                lexeme = lexeme + c;
            }
            break;
        case STRING:
            if (c != '"')
            {
                state = STRING;
                lexeme = lexeme + c;
            }
            else
            {
                lexeme = lexeme + c;
                tokenStream += generatorToken(tokenNum, lexeme, "StringLiteral");
                tokenNum++;
                state = INITIAL;
                keep = false;
            }
            break;
        default:
            cout << ("[ERROR]Scanner:line " + to_string(lIndex) + ", column=" + to_string(cIndex) + ", unreachable state!") << endl;
            break;
        }
    }
    string fileName = argv[1];
    string outFile(fileName, 0, fileName.rfind('.'));
    outFile = outFile + ".tokens";
    ofstream out(outFile.c_str());
    out << tokenStream;
    out.close();
    cout << "Completed" << endl;
    return 0;
}