#ifndef _SYMBOL_H_
#define _SYMBOL_H_
#include "lexer.h"
#include "string"
#include "vector"
#include "fstream"
#include "iostream"
#include <iterator>
#include <map>

using namespace std;

class symbol
{

    const string STRING_TYPE = "CHAR";
    const string INT_TYPE = "INT";

    struct Symbol_Detail
    {
        string name;
        string type;
        vector<string> scope;
        int intVal;
        string strVal;
    };

public:
    // it should be map, for simplecity using list
    vector<Symbol_Detail> _symbols;
    vector<string> _scope;
    map<string, Symbol_Detail> _symbolTable;

    symbol() {}

    bool compareSdtype(const Symbol_Detail &a, const Symbol_Detail &b)
    {
        if (a.name != b.name)
            return false;
        if (a.type != b.type)
            return false;

        if (a.type == STRING_TYPE && a.strVal != b.strVal)
            return false;
        if (a.type == INT_TYPE && a.intVal != b.intVal)
            return false;

        return true;
    }

    void addSymbol(string name, string value)
    {
        _symbolTable.insert(pair<string, Symbol_Detail>(name, {name, STRING_TYPE, _scope, 0, value}));
    }
    void addSymbol(string name, int value)
    {
        _symbolTable.insert(pair<string, Symbol_Detail>(name, {name, INT_TYPE, _scope, value, ""}));
    }

    void toFile()
    {
        fstream f;

        // opening file "Gfg.txt"
        // in out(write) mode
        // ios::out Open for output operations.
        f.open("symbol.txt", ios::out);

        // If no file is created, then
        // show the error message.
        if (!f)
        {
            cout << "Error in creating file!!!";
            return;
        }

        cout << "File created.\n";

        cout << "symbol table is in symbol.txt\n";

        int count = 0;
        map<string, Symbol_Detail>::iterator itr;
        for (itr = _symbolTable.begin(); itr != _symbolTable.end(); ++itr)
        {
            f << itr->second.name;
            f << ",";
            f << itr->second.type;
            f << ",";
            f << count;
            if (itr->second.type.compare("INT") == 0)
            {
                count += 4;
            }
            else
            {
                count += 1;
            }
            f << endl;
        }
        f.close();
    }
};
#endif