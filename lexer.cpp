#include "lexer.h"

using namespace std;
// for printing tokens names, Same as the enum defined in lexer.h
string reserved[] = {
    "END_OF_FILE",
    "ERROR",
    "IDENTIFIER",
    "FUNC",
    "INT",
    "CHAR",
    "CALL",
    "IF",
    "ELIF",
    "ELSE",
    "FOR",
    "PRINT",
    "PRINTLN",
    "RETURN",
    "IN",
    "RO",
    "NE",
    "ASSIGNMENT_OPERATOR",
    "ARITHEMATIC_OPERATOR",
    "NUMERIC_LITERAL",
    "CHARACTER_LITERAL",
    "COMMA",
    "SEMICOLON",
    "COLON",
    "BEGIN",
    "END",
    "COMMENT",
    "STRING"};
token::token()
{
    this->lexeme = "";
    this->tokenType = TokenType::ERROR;
}
token::token(string lexeme, TokenType tokenType)
{
    this->lexeme = lexeme;
    this->tokenType = tokenType;
}
void token::Print()
{
    cout << "{" << lexeme << " , "
         << reserved[(int)tokenType] << "}\n";
}
int lexer::getCurrentPointer()
{
    return index;
}
void lexer::setCurrentPointer(int pos)
{
    if (pos >= 0 && pos < tokens.size())
        index = pos;
    else
        index = 0;
}
void lexer::Tokenize() // function that tokenizes your input stream
{
    vector<char>::iterator iterator;
    iterator = stream.end();
    iterator = stream.begin();
    int state = 0;
    // your implementation goes here
    // push_back EOF token at end to identify End of File
    string letterStream = "";
    for (; iterator != stream.end(); iterator++)
    {
        bool run = true;
        bool fkflag = true;
        switch (state)
        {
        case 0: // Starting state
            if (*iterator == 'r' && run && fkflag)
            {
                state = 44;
                run = false;
                fkflag = false;
            }
            else if (*iterator == 'p' && run && fkflag)
            {
                state = 36;
                run = false;
                fkflag = false;
            }
            else if (*iterator == 'e' && run && fkflag)
            {
                state = 25;
                run = false;
                fkflag = false;
            }
            else if (*iterator == 'c' && run && fkflag)
            {
                state = 20;
                run = false;
                fkflag = false;
            }
            else if (*iterator == 'f' && run && fkflag)
            {
                state = 15;
                run = false;
                fkflag = false;
            }
            else if (*iterator == 'i' && run && fkflag)
            {
                state = 10;
                run = false;
                fkflag = false;
            }
            else if (*iterator == 'b' && run && fkflag)
            {
                state = 65;
                run = false;
                fkflag = false;
            }
            else if (*iterator == '<' && run && fkflag)
            {
                state = 1;
                run = false;
                fkflag = false;
            }
            else if (*iterator == '>' && run && fkflag)
            {
                state = 2;
                run = false;
                fkflag = false;
            }
            else if (*iterator == '~' && run && fkflag)
            {
                state = 7;
                run = false;
                fkflag = false;
            }
            else if (*iterator == '#' && run && fkflag)
            {
                state = 84;
                run = false;
                fkflag = false;
            }
            else if (*iterator >= '0' && *iterator <= '9' && run && fkflag)
            {

                run = false;
                fkflag = false;
                if (*iterator != '\t' && *iterator != '\n')
                    state = 86;
            }
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z' && run && fkflag)
            {
                state = 51;
                run = false;
                fkflag = false;
            }
            else if (*iterator == '"' && run && fkflag)
            {
                state = 62;
                run = false;
                fkflag = false;
            }
            else if (*iterator == ',' && run && fkflag)
            {
                state = 101;
                run = false;
                fkflag = false;
            }
            else if (*iterator == ':' && run && fkflag)
            {
                run = false;
                fkflag = false;
                state = 102;
            }
            else if (*iterator == ';' && run && fkflag)
            {
                state = 103;
                run = false;
                fkflag = false;
            }
            else if (*iterator == '+' || *iterator == '-' || *iterator == '/' || *iterator == '%' || *iterator == '*' && run && fkflag)
            {
                state = 24;
                run = false;
                fkflag = false;
            }
            if (*iterator != '\n' && *iterator != '\t' && *iterator != ' ')
            {
                letterStream.push_back(*iterator);
                run = false;
                fkflag = false;
            }
            break;

        case 1:
            if (*iterator == '=' && run && fkflag)
            {
                state = 4;
                run = false;
                fkflag = false;
            }
            else if (*iterator == '-' && run && fkflag)
            {
                state = 16;
                run = false;
                fkflag = false;
            }
            else
                state = 5;
            letterStream.push_back(*iterator);
            break;

        case 2:
            if (*iterator == '=' && run && fkflag)
            {
                state = 3;
                run = false;
                fkflag = false;
            }
            else
            {
                state = 6;
            }
            letterStream.push_back(*iterator);
            break;

        case 3: // Final State
        {
            state = 0;
            tokens.push_back(token("GE", TokenType::RO));
            letterStream = "";
        }
        break;

        case 4: // Final State
        {
            state = 0;
            tokens.push_back(token("LE", TokenType::RO));
            letterStream = "";
        }
        break;

        case 5: // Final State
        {
            state = 0;
            --iterator;
            --iterator;
            tokens.push_back(token("LT", TokenType::RO));
            letterStream = "";
        }
        break;

        case 6: // Final State
        {
            state = 0;
            --iterator;
            tokens.push_back(token("GT", TokenType::RO));
            letterStream = "";
        }
        break;

        case 7:
            if (*iterator == '=')
                state = 8;
            else
                state = 150;
            letterStream.push_back(*iterator);
            break;

        case 8: // Final State
            state = 0;
            tokens.push_back(token("NE", TokenType::NE));
            letterStream = "";
            break;

        case 10:
            if (*iterator == 'f' && fkflag)
            {
                state = 11;
            }
            else if (*iterator == 'n' && fkflag)
            {
                state = 12;
            }
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
            {
                state = 51;
            }
            else if (*iterator >= '0' && *iterator <= '9' && fkflag)
            {
                state = 52;
            }
            else
            {
                state = 89;
            }
            if (*iterator == ' ')
            {
                letterStream.push_back(*iterator);
            }
            break;

        case 11:
            if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
            {
                fkflag = false;
                state = 51;
            }
            else if (*iterator >= '0' && *iterator <= '9' && fkflag)
            {
                fkflag = false;
                state = 52;
            }
            else
            {
                fkflag = false;
                state = 96;
            }
            letterStream.push_back(*iterator);
            break;

        case 12:
            if (*iterator == 't')
            {
                fkflag = false;
                state = 13;
            }
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
            {
                fkflag = false;
                state = 51;
            }
            else if (*iterator >= '0' && *iterator <= '9' && fkflag)
            {
                fkflag = false;
                state = 52;
            }
            else
            {
                fkflag = false;
                state = 14;
            }
            letterStream.push_back(*iterator);
            break;

        case 13: // FUNC
            if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
            {
                fkflag = false;
                state = 51;
            }
            else if (*iterator >= '0' && *iterator <= '9' && fkflag)
            {
                fkflag = false;
                state = 52;
            }
            else
            {
                fkflag = false;
                state = 98;
            }
            letterStream.push_back(*iterator);
            break;

        case 14: // Final State
        {
            state = 0;
            --iterator;
            tokens.push_back(token(string(""), TokenType::IN));
            letterStream = "";
        }
        break;

        case 15:
            if (*iterator == 'u' && fkflag)
            {
                fkflag = false;
                state = 17;
            }
            else if (*iterator == 'o' && fkflag)
            {
                fkflag = false;
                state = 34;
            }
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
            {
                fkflag = false;
                state = 51;
            }
            else if (*iterator >= '0' && *iterator <= '9' && fkflag)
            {
                fkflag = false;
                state = 52;
            }
            else
            {
                fkflag = false;
                state = 89;
            }
            letterStream.push_back(*iterator);
            break;

        case 16: // Final State
            state = 0;
            --iterator;
            tokens.push_back(token(string(""), TokenType::ASSIGNMENT_OPERATOR));
            letterStream = "";
            break;

        case 17:
            if (*iterator == 'n' && fkflag)
            {
                fkflag = false;
                state = 18;
            }
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
            {
                fkflag = false;
                state = 51;
            }
            else if (*iterator >= '0' && *iterator <= '9' && fkflag)
            {
                fkflag = false;
                state = 52;
            }
            else
            {
                fkflag = false;
                state = 89;
            }
            letterStream.push_back(*iterator);
            break;

        case 18:
            if (*iterator == 'c' && fkflag)
            {
                fkflag = true;
                state = 19;
            }
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
            {
                fkflag = true;
                state = 51;
            }
            else if (*iterator >= '0' && *iterator <= '9' && fkflag)
            {
                fkflag = true;
                state = 52;
            }
            else
            {
                fkflag = true;
                state = 89;
            }
            letterStream.push_back(*iterator);
            break;

        case 19: // FUNC
            if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
            {
                fkflag = true;
                state = 51;
            }
            else if (*iterator >= '0' && *iterator <= '9' && fkflag)
            {
                fkflag = true;
                state = 52;
            }
            else
            {
                fkflag = true;
                state = 100;
            }
            letterStream.push_back(*iterator);
            break;

        case 20:
            if (*iterator == 'a' && fkflag)
            {
                fkflag = true;
                state = 31;
            }
            else if (*iterator == 'h' && fkflag)
            {
                fkflag = true;
                state = 21;
            }
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
            {
                fkflag = true;
                state = 51;
            }
            else if (*iterator >= '0' && *iterator <= '9' && fkflag)
            {
                fkflag = true;
                state = 52;
            }
            else
            {
                fkflag = true;
                state = 89;
            }
            letterStream.push_back(*iterator);
            break;

        case 21:
            if (*iterator == 'a' && fkflag)
            {
                fkflag = true;
                state = 22;
            }
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
            {
                fkflag = true;
                state = 51;
            }
            else if (*iterator >= '0' && *iterator <= '9' && fkflag)
            {
                fkflag = true;
                state = 52;
            }
            else
            {
                fkflag = true;
                state = 89;
            }
            letterStream.push_back(*iterator);
            break;

        case 22:
            if (*iterator == 'r' && fkflag)
            {
                fkflag = true;
                state = 23;
            }
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
            {
                fkflag = true;
                state = 51;
            }
            else if (*iterator >= '0' && *iterator <= '9' && fkflag)
            {
                fkflag = true;
                state = 52;
            }
            else
            {
                fkflag = true;
                state = 89;
            }
            letterStream.push_back(*iterator);
            break;

        case 23: // CHAR
            if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
            {
                fkflag = true;
                state = 51;
            }
            else if (*iterator >= '0' && *iterator <= '9' && fkflag && run)
            {
                fkflag = true;
                state = 52;
            }
            else
            {
                fkflag = true;
                state = 93;
            }
            letterStream.push_back(*iterator);
            break;

        case 24:
        {
            state = 0;
            tokens.push_back(token(letterStream, TokenType::ARITHEMATIC_OPERATOR));
            letterStream = "";
            --iterator;
        }
        break;

        case 25:
        {
            if (*iterator == 'n' && fkflag && run)
                state = 78;
            else if (*iterator == 'l' && fkflag && run)
                state = 26;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9' && fkflag && run)
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 26:
        {
            if (*iterator == 'i' && fkflag && run)
                state = 28;
            else if (*iterator == 's' && fkflag && run)
                state = 27;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9' && fkflag && run)
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 27:
        {
            if (*iterator == 'e' && fkflag && run)
                state = 30;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9' && fkflag && run)
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 28:
        {
            if (*iterator == 'f' && fkflag && run)
                state = 29;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9' && fkflag && run)
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 29:
        {
            if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9' && fkflag && run)
                state = 52;
            else
                state = 61;
            letterStream.push_back(*iterator);
        }
        break;

        case 30:
        {
            if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 90;
            letterStream.push_back(*iterator);
        }
        break;

        case 31:
        {
            if (*iterator == 'l')
                state = 32;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 32:
        {
            if (*iterator == 'l')
                state = 33;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 33:
        {
            if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 92;
            letterStream.push_back(*iterator);
        }
        break;

        case 34:
        {
            if (*iterator == 'r')
                state = 35;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 35:
        {
            if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 99;
            letterStream.push_back(*iterator);
        }
        break;

        case 36:
        {
            if (*iterator == 'r')
                state = 37;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 37:
        {
            if (*iterator == 'i')
                state = 38;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 38:
        {
            if (*iterator == 'n')
                state = 39;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 39:
        {
            if (*iterator == 't')
                state = 40;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 40:
        {
            if (*iterator == 'l')
                state = 41;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 43; // PRINT
            letterStream.push_back(*iterator);
        }
        break;

        case 41:
        {
            if (*iterator == 'n')
                state = 49;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 42: // RETURN
        {
            if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 56;
            letterStream.push_back(*iterator);
        }
        break;

        case 43: // Final State
        {
            state = 0;
            --iterator;
            tokens.push_back(token(string(""), TokenType::PRINT));
            letterStream = "";
        }
        break;

        case 44:
        {
            if (*iterator == 'e')
                state = 45;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 45:
        {
            if (*iterator == 't')
                state = 46;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 46:
        {
            if (*iterator == 'u')
                state = 47;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;
        case 47:
        {
            if (*iterator == 'r')
                state = 48;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;
        case 48:
        {
            if (*iterator == 'n')
                state = 42;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 49: // PRINTLN
        {
            if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 91;
            letterStream.push_back(*iterator);
        }
        break;

        case 51:
        {
            if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
            {
                state = 51;
                letterStream.push_back(*iterator);
            }
            else if (*iterator >= '0' && *iterator <= '9')
            {
                letterStream.push_back(*iterator);
                state = 52;
            }
            else
                state = 89;
        }
        break;

        case 52: // For identifiers having digits at end
        {
            if (*iterator >= '0' && *iterator <= '9')
            {
                letterStream.push_back(*iterator);
                state = 52;
            }
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 150;
            else
                state = 89;
        }
        break;

        case 54:
        {
            if (*iterator == 'n')
                state = 76;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 55:
        {
            if (*iterator == 'g')
                state = 68;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 56:
        {
            state = 0;
            letterStream = "";
            --iterator;
            tokens.push_back(token(string(""), TokenType::RETURN));
        }
        break;

        case 61: // Final State
        {
            state = 0;
            --iterator;
            tokens.push_back(token(string(""), TokenType::ELIF));
            letterStream = "";
        }
        break;

        case 62:
        {
            state = 63;
            letterStream.push_back(*iterator);
        }
        break;

        case 63:
        { // check for string
            if (*iterator == '"')
                state = 64;
            else
                state = 63;
            letterStream.push_back(*iterator);
        }
        break;

        case 64:
        {
            state = 0;
            --iterator;
            tokens.push_back(token(letterStream, TokenType::STRING));
            letterStream = "";
        }
        break;

        case 65:
        {
            if (*iterator == 'e')
                state = 55;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 68:
        {
            if (*iterator == 'i')
                state = 54;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 75: // Final State
        {
            state = 0;
            letterStream = "";
            --iterator;
            tokens.push_back(token(string(""), TokenType::BEGIN));
        }
        break;

        case 76:
        {
            if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 75;
            letterStream.push_back(*iterator);
        }
        break;

        case 78:
        {
            if (*iterator == 'd')
                state = 79;
            else if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*iterator);
        }
        break;

        case 79:
        {
            if (*iterator >= 'a' && *iterator <= 'z' || *iterator >= 'A' && *iterator <= 'Z')
                state = 51;
            else if (*iterator >= '0' && *iterator <= '9')
                state = 52;
            else
                state = 81;
            letterStream.push_back(*iterator);
        }
        break;

        case 81: // Final State
        {
            state = 0;
            --iterator;
            tokens.push_back(token(string(""), TokenType::END));
            letterStream = "";
        }
        break;

        case 84:
        {
            if ((int)*iterator != 10)
                letterStream.push_back(*iterator);
            else
                state = 85;
        }
        break;
        case 85: // Final State
        {
            state = 0;
            --iterator;
            tokens.push_back(token(letterStream, TokenType::COMMENT));
            letterStream = "";
        }
        break;
        case 86: // Only for numeric literals
        {
            if (*iterator >= '0' && *iterator <= '9')
            {
                state = 86;
                letterStream.push_back(*iterator);
            }
            else
            {
                if (*iterator != ' ')
                    --iterator;
                state = 88;
            }
        }
        break;
        case 88: // Final State: Numeric Literal
        {
            state = 0;
            --iterator;
            tokens.push_back(token(letterStream, TokenType::NUMERIC_LITERAL));
            letterStream = "";
        }
        break;
        case 89: // Final State: Identifier
        {        // In DFA, there are more than 1 states for identifers
                 // that was done to make dfa more neat
                 // while coding the DFA, I only used one final state from DFA
            state = 0;
            --iterator;
            --iterator;
            tokens.push_back(token(letterStream, TokenType::IDENTIFIER));
            letterStream = "";
        }
        break;

        case 90: // Final State
        {
            state = 0;
            --iterator;
            tokens.push_back(token(string(""), TokenType::ELSE));
            letterStream = "";
        }
        break;

        case 91: // Final State
        {
            state = 0;
            --iterator;
            letterStream = "";
            tokens.push_back(token(string(""), TokenType::PRINTLN));
        }
        break;

        case 92: // Final State
        {
            state = 0;
            --iterator;
            tokens.push_back(token(string(""), TokenType::CALL));
            letterStream = "";
        }
        break;

        case 93: // Final State
        {
            state = 0;
            --iterator;
            letterStream = "";
            tokens.push_back(token(string(""), TokenType::CHAR));
        }
        break;

        case 96: // Final State
        {
            state = 0;
            --iterator;
            tokens.push_back(token(string(""), TokenType::IF));
            letterStream = "";
        }
        break;

        case 98: // Final State
        {
            state = 0;
            letterStream = "";
            --iterator;
            --iterator;
            tokens.push_back(token(letterStream, TokenType::INT));
        }
        break;

        case 99: // Final State
        {
            state = 0;
            letterStream = "";
            --iterator;
            tokens.push_back(token(letterStream, TokenType::FOR));
        }
        break;

        case 100: // Final State
        {
            state = 0;
            letterStream = "";
            --iterator;
            tokens.push_back(token(string(""), TokenType::FUNC));
        }
        break;

        case 101: // Final State
        {
            state = 0;
            letterStream = "";
            --iterator;
            tokens.push_back(token(string(""), TokenType::COMMA));
        }
        break;

        case 102: // Final State
        {
            state = 0;
            letterStream = "";
            --iterator;
            tokens.push_back(token(string(""), TokenType::COLON));
        }
        break;

        case 103: // Final State
        {
            state = 0;
            letterStream = "";
            --iterator;
            tokens.push_back(token(string(""), TokenType::SEMICOLON));
        }
        break;

        case 150: // Trap State
        {
            state = 0;
            letterStream = "";
            --iterator;
            tokens.push_back(token());
        }
        }
        fkflag = true;
    }
    tokens.push_back(token(string(""), TokenType::END_OF_FILE));
}
lexer::lexer(const char filename[])
{
    // constructors: takes file name as an argument and store all
    // the contents of file into the class varible stream
    ifstream fin(filename);
    if (!fin) // if file not found
    {
        cout << "file not found" << endl;
    }
    else
    {
        char byte = 0;
        while (fin.get(byte))
        { // store file contents into stream
            if (byte != '\r')
                stream.push_back(byte);
        }
        stream.push_back(' '); // dummy spaces appended at the end
        stream.push_back(' ');
        // Tokenize function to get all the (token,lexeme) pairs
        Tokenize();
        // assigning tokens to iterator::pointer
        index = 0;
    }
}
lexer::lexer()
{
    index = -1;
}
void lexer::printRaw()
{
    // helper function to print the content of file
    vector<char>::iterator it = stream.begin();
    for (it = stream.begin(); it != stream.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
}
token lexer::getNextToken()
{
    // this function will return single (token,lexeme) pair on each call
    // this is an iterator which is pointing to the beginning of vector of tokens
    token _token;
    if (index == tokens.size())
    {                       // return end of file if
        _token.lexeme = ""; // index is too large
        _token.tokenType = TokenType::END_OF_FILE;
    }
    else
    {
        _token = tokens[index];
        index = index + 1;
    }
    return _token;
}
token lexer::getCurrentToken()
{
    // this function will return single (token,lexeme) pair on each call
    // this is an iterator which is pointing to the beginning of vector of tokens
    token _token;
    if (index == tokens.size())
    {                       // return end of file if
        _token.lexeme = ""; // index is too large
        _token.tokenType = TokenType::END_OF_FILE;
    }
    else
    {
        _token = tokens[index];
    }
    return _token;
}
void lexer::resetPointer()
{
    index = 0;
}
token lexer::peek(int howFar)
{
    if (howFar <= 0)
    { // peeking backward or in place is not allowed
        cout << "LexicalAnalyzer:peek:Error: non positive argument\n";
        exit(-1);
    }

    int peekIndex = index + howFar - 1;
    if (peekIndex > (tokens.size() - 1))
    {                                             // if peeking too far
        return token("", TokenType::END_OF_FILE); // return END_OF_FILE
    }
    else
        return tokens[peekIndex];
}