#include "parser.h"

void parser::syntax_error()
{
    cout << "SYNTAX ERROR\n";
    exit(1);
}
token parser::expect(TokenType expected_type)
{
    token t = _lexer.getNextToken();
    if (t.tokenType != expected_type)
        syntax_error();
    // else
    // {
    //     cout << "Accepted token: ";
    //     t.Print();
    //     cout << "\n";
    //     return t;
    // }
    return t;
}
parser::parser(const char filename[])
{
    _lexer = lexer(filename);
}
void parser::readAndPrintAllInput() // read and print allinputs (provided)
{
    token t;
    t = _lexer.getNextToken();
    while (t.tokenType != TokenType::END_OF_FILE)
    {
        t.Print();
        t = _lexer.getNextToken();
    }
}
void parser::resetPointer()
{
    _lexer.resetPointer();
}
// this function is for sample purposes only
/*
bool parser::statements()
{
    //statements-- > COLON LPAREN start RPAREN
    if (_lexer.peek(1).tokenType == TokenType::COLON)
    {
        expect(TokenType::COLON);
        if (_lexer.peek(1).tokenType == TokenType::LPAREN)
        {
            expect(TokenType::LPAREN);
            start();
            if (_lexer.peek(1).tokenType == TokenType::RPAREN)
            {
                expect(TokenType::RPAREN);
                return {true};
            }
        }
    }
    return {false};
}*/

void parser::debug(const string &where, const string &messages, token t)
{
    if (DEBUG)
    {
        cout << "DEBUG: " << where << ": " << messages << ": ";
        t.Print();
    }
}

bool strCompare(const string &a, const string &b)
{
    return a.compare(b) == 0;
}

// Entry point for CFG

// Start -> Function Start | Comment Start
parserR parser::START()
{
    if (FUNC().b)
    {
        bool a = START().b;
        if (_lexer.getCurrentToken().tokenType == TokenType::END_OF_FILE)
        {
            expect(TokenType::END_OF_FILE);
            return {true};
        }
        return {a};
    }
    else if (COMMENT().b)
    {
        bool a = START().b;
        if (_lexer.getCurrentToken().tokenType == TokenType::END_OF_FILE)
        {
            expect(TokenType::END_OF_FILE);
            return {true};
        }
        return {a};
    }
    debug(__func__, "Failed", _lexer.peek(1));
    return {false};
}

// EXPRESSION

/**
Expression -> T E` .
E` -> +TE` | -TE` | .
T -> F T` .
T` -> * F T` | / F T` | % F T | .
F -> id | num .
 */
parserR parser::EXPRESSION()
{
    parserR a = T();
    parserR b = E_();
    return {a.b && b.b, a.s + b.s};
}

// E` -> +TE` | -TE` | .
parserR parser::E_()
{
    const string _c = _lexer.peek(1).lexeme;
    if (strCompare(_c, "+") || strCompare(_c, "-"))
    {
        expect(TokenType::ARITHEMATIC_OPERATOR);
        // parserR a = T();
        // parserR b = E_();
        // if (strCompare(_c, "+"))
        // {
        //     return {a.b && b.b, a.s + " + " + b.s};
        // }
        // else
        // {
        //     return {a.b && b.b, a.s + " - " + b.s};
        // }

        if (strCompare(_c, "+"))
        {
            parserR a = T("+");
            parserR b = E_();
            return {a.b && b.b, a.s + b.s};
        }
        else
        {
            parserR a = T("-");
            parserR b = E_();
            return {a.b && b.b, a.s + b.s};
        }
    }
    else if (_c[0] == '+' || _c[0] == '-')
    {
        // parserR a = T();
        // parserR b = E_();
        // if (_c[0] == '+')
        // {
        //     return {a.b && b.b, a.s + " + " + b.s};
        // }
        // else
        // {
        //     return {a.b && b.b, a.s + " - " + b.s};
        // }

        if (_c[0] == '+')
        {
            parserR a = T("+");
            parserR b = E_();
            return {a.b && b.b, a.s + b.s};
        }
        else
        {
            parserR a = T("-");
            parserR b = E_();
            return {a.b && b.b, a.s + b.s};
        }
    }
    return {true};
}
// T -> F T` .
parserR parser::T(string middle)
{
    parserR a = F();
    parserR b = T_();
    return {a.b && b.b, middle + a.s + b.s};
}
// T` -> * F T` | / F T` | % F T | .
parserR parser::T_()
{
    const string _c = _lexer.peek(1).lexeme;
    if (strCompare(_c, "*") || strCompare(_c, "/") || strCompare(_c, "%"))
    {
        expect(TokenType::ARITHEMATIC_OPERATOR);
        parserR a = F();
        parserR b = T_();
        if (strCompare(_c, "*"))
        {
            return {a.b && b.b, a.s + " * " + b.s};
        }
        else if (strCompare(_c, "/"))
        {
            return {a.b && b.b, a.s + " / " + b.s};
        }
        else
        {
            return {a.b && b.b, a.s + " % " + b.s};
        }
    }
    return {true};
}
// F -> id | num .
parserR parser::F()
{
    if (_lexer.peek(1).tokenType == TokenType::IDENTIFIER)
    {
        const string s = _lexer.peek(1).lexeme;
        expect(TokenType::IDENTIFIER);
        return {true, s};
    }
    else if (_lexer.peek(1).tokenType == TokenType::NUMERIC_LITERAL)
    {
        const string s = _lexer.peek(1).lexeme;
        expect(TokenType::NUMERIC_LITERAL);
        return {true, s};
    }
    debug(__func__, "Failed", _lexer.peek(1));
    return {false};
}

// PRINT
/*
Print -> print PrintBody ; .
PrintBody -> Expression | String .
*/

parserR parser::PRINT()
{
    if (_lexer.peek(1).tokenType == TokenType::PRINT)
    {
        expect(TokenType::PRINT);
        parserR ret = PRINT_BODY();
        if (ret.b)
        {
            if (_lexer.peek(1).tokenType == TokenType::SEMICOLON)
            {
                expect(TokenType::SEMICOLON);
                _t.emit("out " + ret.s + ";");
                return {true};
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}

parserR parser::PRINT_BODY()
{
    parserR a = EXPRESSION();
    if (a.b)
        return a;
    return STRING();
}

// COMMENT

//  COMMENT -> # StringBody EOF .
parserR parser::COMMENT()
{
    if (_lexer.peek(1).tokenType == TokenType::COMMENT)
    {
        const string _c = _lexer.peek(1).lexeme;
        if (_c[0] == '#')
        {
            expect(TokenType::COMMENT);
            return {true};
        }
    }
    debug(__func__, "COMMENT FAILED", _lexer.peek(1));
    return {false};
}

// STRING

/*
String -> " StringBody " .
StringBody -> char StringBody | num StringBody | .
*/
parserR parser::STRING()
{
    const string _c = _lexer.peek(1).lexeme;

    if (_c[0] == '"' && _c[_c.size() - 1] == '"')
    {
        expect(TokenType::STRING);
        return {true, _c};
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}

// ASSIGNMENT

/**
Assignment -> id Assignment2 .
Assignment2 -> , Assignment | <- Assignment3 | Type ; .
Assignment3 -> Expression Assignment4 .
Assignment4 -> , Assignment | Type ; .
 */

// Assignment -> id Assignment2 .
parserR parser::ASSIGN()
{
    if (_lexer.peek(1).tokenType == TokenType::IDENTIFIER)
    {
        const string _name = _lexer.peek(1).lexeme;
        expect(TokenType::IDENTIFIER);
        parserR ret = ASSIGN_2();
        if (ret.b)
        {
            if (ret.s.compare("char") == 0)
            {
                _table.addSymbol(_name, "char");
            }
            else
                _table.addSymbol(_name, 0);
            _t.emit(_name + " = " + ret.s + ";");
            return {true};
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}

// Assignment2 -> , Assignment | <- Assignment3 | Type ; .
parserR parser::ASSIGN_2()
{
    if (_lexer.peek(1).tokenType == TokenType::COMMA)
    {
        expect(TokenType::COMMA);
        return ASSIGN();
    }
    else if (_lexer.peek(1).tokenType == TokenType::ASSIGNMENT_OPERATOR)
    {
        expect(TokenType::ASSIGNMENT_OPERATOR);
        return ASSIGN_3();
    }
    parserR tt = TYPE();
    if (tt.b)
    {
        if (_lexer.peek(1).tokenType == TokenType::SEMICOLON)
        {
            expect(TokenType::SEMICOLON);
            return tt;
        }
    }
    return {false};
}

// Assignment3 -> Expression Assignment4 .
parserR parser::ASSIGN_3()
{
    parserR ret = EXPRESSION();
    if (ret.b)
    {
        parserR ret2 = ASSIGN_4();
        return {ret2.b, ret.s + ret2.s};
    }
    return ret;
}

// Assignment4 -> , Assignment | Type ; | ; .
parserR parser::ASSIGN_4()
{
    if (_lexer.peek(1).tokenType == TokenType::COMMA)
    {
        expect(TokenType::COMMA);
        return ASSIGN();
    }
    else if (TYPE().b)
    {
        if (_lexer.peek(1).tokenType == TokenType::SEMICOLON)
        {
            expect(TokenType::SEMICOLON);
            return {true};
        }
    }
    else if (_lexer.peek(1).tokenType == TokenType::SEMICOLON)
    {
        expect(TokenType::SEMICOLON);
        return {true};
    }
    return {false};
}

// TYPE

// Type -> char | int .
parserR parser::TYPE()
{
    if (_lexer.peek(1).tokenType == TokenType::CHAR)
    {
        expect(TokenType::CHAR);
        return {true, "char"};
    }

    else if (_lexer.peek(1).tokenType == TokenType::INT)
    {
        expect(TokenType::INT);
        return {true, "int"};
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}

// RETURN

/*
Return -> return ReturnBody ; .
ReturnBody -> Expression | String | .
*/

// Return -> return ReturnBody ; .
parserR parser::RETURN()
{
    if (_lexer.peek(1).tokenType == TokenType::RETURN)
    {
        expect(TokenType::RETURN);
        parserR ret = RETURN_BODY();
        if (ret.b)
        {
            if (_lexer.peek(1).tokenType == TokenType::SEMICOLON)
            {
                expect(TokenType::SEMICOLON);
                _t.emit("ret " + ret.s + ";");
                return {true};
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}

// ReturnBody -> Expression | String | .
parserR parser::RETURN_BODY()
{
    parserR ret = EXPRESSION();
    if (ret.b)
        return ret;
    ret = STRING();
    if (ret.b)
        return ret;
    return {true};
}

// FUNCTION

/*
Function -> func Type id : CodeBody .
*/

// Function -> func Type id : CodeBody .
parserR parser::FUNC()
{
    if (_lexer.peek(1).tokenType == TokenType::FUNC)
    {
        expect(TokenType::FUNC);
        if (TYPE().b)
        {
            if (_lexer.peek(1).tokenType == TokenType::IDENTIFIER)
            {
                expect(TokenType::IDENTIFIER);
                if (_lexer.peek(1).tokenType == TokenType::COLON)
                {
                    expect(TokenType::COLON);
                    CODE();
                    {
                        if (_lexer.peek(1).tokenType == TokenType::SEMICOLON)
                        {
                            expect(TokenType::SEMICOLON);
                            return {true};
                        }
                    }
                }
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}

// IN

// In -> in id ;
parserR parser::IN()
{
    if (_lexer.peek(1).tokenType == TokenType::IN)
    {
        expect(TokenType::IN);
        if (_lexer.peek(1).tokenType == TokenType::IDENTIFIER)
        {
            string id = _lexer.peek(1).lexeme;
            expect(TokenType::IDENTIFIER);
            if (_lexer.peek(1).tokenType == TokenType::SEMICOLON)
            {
                expect(TokenType::SEMICOLON);
                _t.emit("in " + id + ";");
                return {true};
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}

// CODE
//  Code -> begin CodeBody | Comment Code .
//  CodeBody -> If CodeBody CodeBodyEnd | For CodeBody CodeBodyEnd | Print CodeBody CodeBodyEnd | Comment CodeBody CodeBodyEnd | Assignment CodeBody CodeBodyEnd | In CodeBody CodeBodyEnd | Return CodeBody CodeBodyEnd .
//  CodeBodyEnd -> end .

// Code -> begin CodeBody .
parserR parser::CODE()
{
    if (_lexer.peek(1).tokenType == TokenType::BEGIN)
    {
        expect(TokenType::BEGIN);
        if (CODE_BODY().b)
        {

            return {true};
        }
    }
    else if (COMMENT().b)
    {
        CODE();
        {
            return {true};
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}

// CodeBody -> If CodeBody CodeBodyEnd | For CodeBody CodeBodyEnd | Print CodeBody CodeBodyEnd | Comment CodeBody CodeBodyEnd | Assignment CodeBody CodeBodyEnd | In CodeBody CodeBodyEnd | Return CodeBody CodeBodyEnd .
parserR parser::CODE_BODY()
{
    bool code_body = false, code_body_end = false;
    if (IF().b)
    {
        code_body = CODE_BODY().b;
        code_body_end = CODE_BODY_END().b;
        return {code_body && code_body_end};
    }
    else if (FOR().b)
    {
        code_body = CODE_BODY().b;
        code_body_end = CODE_BODY_END().b;
        return {code_body && code_body_end};
    }
    else if (PRINT().b)
    {
        code_body = CODE_BODY().b;
        code_body_end = CODE_BODY_END().b;
        return {code_body && code_body_end};
    }
    else if (COMMENT().b)
    {
        code_body = CODE_BODY().b;
        code_body_end = CODE_BODY_END().b;
        return {code_body && code_body_end};
    }
    else if (ASSIGN().b)
    {
        code_body = CODE_BODY().b;
        code_body_end = CODE_BODY_END().b;
        return {code_body && code_body_end};
    }
    else if (IN().b)
    {
        code_body = CODE_BODY().b;
        code_body_end = CODE_BODY_END().b;
        return {code_body && code_body_end};
    }
    else if (RETURN().b)
    {
        code_body = CODE_BODY().b;
        code_body_end = CODE_BODY_END().b;
        return {code_body && code_body_end};
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}

// CodeBodyEnd -> end .
parserR parser::CODE_BODY_END()
{
    if (_lexer.peek(1).tokenType == TokenType::END)
    {
        expect(TokenType::END);
        return {true};
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}

// IF
/*
IF -> if CondionalStatement : CodeBody IF_ .
IF_ -> elif ConditionalStatement : CodeBody IF_ | else : CodeBody | .
*/

// IF -> if CondionalStatement : CodeBody IF_ .
parserR parser::IF()
{
    if (_lexer.peek(1).tokenType == TokenType::IF)
    {
        expect(TokenType::IF);
        parserR cond = CONDITIONAL_STATEMENT();
        if (cond.b)
        {
            _t.backpatch(cond.t, cond.f + 1);
            if (_lexer.peek(1).tokenType == TokenType::COLON)
            {
                expect(TokenType::COLON);
                CODE();
                {
                    int pos = _t.getLine();
                    _t.emit("goto ");
                    _t.backpatch(cond.f, _t.getLine());
                    parserR if_ = IF_();
                    if (if_.b)
                    {
                        _t.backpatch(pos, if_.t);
                        return {true};
                    }
                }
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}

// IF_ -> elif ConditionalStatement : CodeBody IF_ | else : CodeBody | .
parserR parser::IF_()
{
    if (_lexer.peek(1).tokenType == TokenType::ELIF)
    {
        expect(TokenType::ELIF);
        parserR cond = CONDITIONAL_STATEMENT();
        if (cond.b)
        {
            _t.backpatch(cond.t, _t.getLine());
            if (_lexer.peek(1).tokenType == TokenType::COLON)
            {
                expect(TokenType::COLON);
                CODE();
                {
                    int pos = _t.getLine();
                    _t.emit("goto ");
                    _t.backpatch(cond.f, _t.getLine());
                    parserR if_ = IF_();
                    if (if_.b)
                    {
                        _t.backpatch(pos, if_.t);
                        return {true, "", 0, if_.t};
                    }
                }
            }
        }
    }
    else if (_lexer.peek(1).tokenType == TokenType::ELSE)
    {
        expect(TokenType::ELSE);
        if (_lexer.peek(1).tokenType == TokenType::COLON)
        {
            expect(TokenType::COLON);
            CODE();
            {
                return {true, "", 0, _t.getLine()};
            }
        }
    }
    return {true, "", 0, _t.getLine()};
}

// CONDITIONAL STATEMENT

// ConditionalStatement -> Expression RelationalOP Expression .
parserR parser::CONDITIONAL_STATEMENT()
{
    parserR a, b, c;
    a = EXPRESSION();
    if (a.b)
    {
        b = RELATIONAL_OPERATOR();
        if (b.b)
        {
            c = EXPRESSION();
            if (c.b)
            {
                int t = _t.getLine();
                _t.emit("if " + a.s + " " + b.s + " " + c.s + " goto ");
                int f = _t.getLine();
                _t.emit("goto ");
                return {true, "", 0, t, f};
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}

// FOR

// For -> for Assignment, ConditionalStatement, Assignment : Code .
parserR parser::FOR()
{
    if (_lexer.peek(1).tokenType == TokenType::FOR)
    {
        expect(TokenType::FOR);
        parserR for_assign = FOR_ASSIGN();
        if (for_assign.b)
        {
            if (_lexer.peek(1).tokenType == TokenType::COMMA)
            {
                expect(TokenType::COMMA);
                _t.emit(for_assign.s);
                int start = _t.getLine();
                parserR for_conditional_statement = CONDITIONAL_STATEMENT();
                if (for_conditional_statement.b)
                {
                    _t.backpatch(for_conditional_statement.t, _t.getLine());
                    if (_lexer.peek(1).tokenType == TokenType::COMMA)
                    {
                        expect(TokenType::COMMA);
                        parserR for_assign_2 = FOR_ASSIGN();
                        if (for_assign_2.b)
                        {
                            if (_lexer.peek(1).tokenType == TokenType::COLON)
                            {
                                expect(TokenType::COLON);
                                CODE();
                                {
                                    _t.emit(for_assign_2.s);
                                    int pos = _t.getLine();
                                    _t.emit("goto ");
                                    _t.backpatch(pos, start);
                                    _t.backpatch(for_conditional_statement.f, _t.getLine());

                                    return {true};
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}

// FOR_ASSIGN -> id <- Expression .
parserR parser::FOR_ASSIGN()
{
    if (_lexer.peek(1).tokenType == TokenType::IDENTIFIER)
    {
        string id = _lexer.peek(1).lexeme;
        expect(TokenType::IDENTIFIER);
        if (_lexer.peek(1).tokenType == TokenType::ASSIGNMENT_OPERATOR)
        {
            expect(TokenType::ASSIGNMENT_OPERATOR);
            parserR expression = EXPRESSION();
            if (expression.b)
            {
                return {true, id + " = " + expression.s + ";"};
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}

// ARTHIMATIC OPERATORS

// ArthimaticOP -> + | - | * | / | % .
parserR parser::ARITHEMATIC_OPERATOR()
{
    const string _c = _lexer.peek(1).lexeme;
    if (strCompare(_c, "+") || strCompare(_c, "-") || strCompare(_c, "*") || strCompare(_c, "/") || strCompare(_c, "%"))
    {
        expect(TokenType::ARITHEMATIC_OPERATOR);
        return {true};
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}

// REALTIONAL OPERATORS

// RelationalOP -> = | ~ = | < RelationalOP' | > RelationalOP' .
// RelationalOP' -> = | .

// RelationalOP -> = | ~ = | < RelationalOP' | > RelationalOP' .
parserR parser::RELATIONAL_OPERATOR()
{
    const string _c = _lexer.peek(1).lexeme;
    if (strCompare(_c, "=") || strCompare(_c, "~=") || strCompare(_c, "NE") || strCompare(_c, "LT") || strCompare(_c, "GT") || strCompare(_c, "GE") || strCompare(_c, "LE"))
    {
        expect(TokenType::RO);
        string s = _c;
        if (strCompare(_c, "NE"))
        {
            s = "<>";
        }
        else if (strCompare(_c, "LT"))
        {
            s = "<";
        }
        else if (strCompare(_c, "GT"))
        {
            s = ">";
        }
        else if (strCompare(_c, "GE"))
        {
            s = ">=";
        }
        else if (strCompare(_c, "LE"))
        {
            s = "<=";
        }
        return {true, s};
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return {false};
}