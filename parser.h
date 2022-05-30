#ifndef _PARSER_H_
#define _PARSER_H_
#include "lexer.h"
#include "translation.h"
#include "symbol.cpp"

// for return types
struct parserR
{
    bool b;
    string s;
    int line;
    int t;
    int f;
};

// for future assignments leave it as it is
class parser
{
    lexer _lexer;
    symbol _table;
    translation _t;

public:
    bool DEBUG = false;

    void printSymbols()
    {
        _table.toFile();
    }

    void debug(const string &where, const string &messages, token t);

    void syntax_error();
    token expect(TokenType expected_type);
    parser(const char filename[]);
    void readAndPrintAllInput();
    void resetPointer();
    /*Terminal functions goes here use peek and expect*/
    /*use TokenType:: for token names for example
        expect(TokenType::ASSIGN);   //example function call
      */
    // all your parser function goes here

    parserR START();

    // EXPRESSION
    parserR EXPRESSION();
    parserR T(string middle = "");
    parserR E_();
    parserR T_();
    parserR F();

    // ASSIGNMENT
    parserR ASSIGN();
    parserR ASSIGN_2();
    parserR ASSIGN_3();
    parserR ASSIGN_4();
    // FOR
    parserR FOR();
    parserR FOR_ASSIGN();

    // IN
    parserR IN();

    parserR CODE();
    parserR CODE_BODY();
    parserR CODE_BODY_END();

    parserR IF();
    parserR IF_();

    // VARIABLE
    parserR VAR(); // wrapper V()
    parserR V();
    parserR V_();

    parserR TYPE();

    // FUNCTION
    parserR FUNC();

    // COMMENT
    parserR COMMENT();
    parserR STRING();
    parserR STRING_BODY();

    // OPERATOR
    parserR ARITHEMATIC_OPERATOR();
    parserR RELATIONAL_OPERATOR();
    parserR RELATIONAL_OPERATOR_();

    // PRINT
    parserR PRINT();
    parserR PRINT_BODY();

    // RETURN
    parserR RETURN();
    parserR RETURN_BODY();

    // CONDITIONAL STATEMENT
    parserR CONDITIONAL_STATEMENT();

    // TAC
    vector<string> TAC()
    {
        return _t.getTAC();
    }

    // to file

    void toFileTAC(string filename)
    {
        _t.toFile(filename);
    }
};
#endif
