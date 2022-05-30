#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <string>
#include <map>
#include <math.h>
using namespace std;

struct tok
{
    string lex;
    int type;
    bool end;
};

vector<vector<string>> quad;
vector<tok> arr_toks;

vector<vector<string>> sym_table;
vector<int> ds;

string type_arr[12] = {"BOOLEAN_OPERATOR",     // 0
                       "VARIABLE",             // 1
                       "NUMERIC_VALUE",        // 2
                       "INPUT",                // 3
                       "PRINT",                // 4
                       "CALCULATION_OPERATOR", // 5
                       "ASSIGN",               // 6
                       "STR_VAL",              // 7
                       "JUMP",                 // 8
                       "IF",                   // 9
                       "RETURN",               // 10
                       "END"};                 // 11

int index_finder(string a);

vector<string> tokenize_TAC_row(string row);

int tokenize(char *file);

string get_address(string id);

string new_t_var(string type, string data);

string address_of_datatype(tok tk, int t, string type_str);

void new_quad_index(string operator_code, string argument_1, string argument_2, string result);

int address_code_reader(char *file);

void execute();

string types[3] = {"INT", "CHAR", "CHAR*"};

class SymbolTable
{
private:
    char *file;
    vector<vector<string>> symbolTable;

public:
    SymbolTable();
    SymbolTable(char *f);

    string new_symbol(string id, string type, string data = "")
    {
        vector<string> temp;
        temp.resize(3);
        temp[0] = id;
        temp[1] = type;
        temp[2] = to_string(ds.size());
        symbolTable.push_back(temp);
        vector<int> toPush;
        if (data == "")
        {
            toPush.resize(toPush.size() + 1);
            toPush[toPush.size() - 1] = 0;
        }
        else
        {
            if (!(type.compare(types[0])))
            {
                toPush.resize(toPush.size() + 1);
                toPush[toPush.size() - 1] = stoi(data);
            }
            else if (!(type.compare(types[1])))
            {
                toPush.resize(toPush.size() + 1);
                toPush[toPush.size() - 1] = (int)data[0];
            }
            else if (!(type.compare(types[2])))
            {
                int temp = data.size();
                for (int i = 0; i < data.size(); i++)
                {
                    if ((i + 1) < temp && data[i] == '\\')

                        if (data[i + 1] == 'n')
                        {
                            toPush.resize(toPush.size() + 1);
                            toPush[toPush.size() - 1] = ((int)'\n');
                            i++;
                        }
                        else
                        {
                            toPush.resize(toPush.size() + 1);
                            toPush[toPush.size() - 1] = (int)data[i];
                        }
                }
                ds.push_back(0);
            }
        }
        returnSymTab();
        return id;
    }

    int read_symbol()
    {
        fstream f;
        f.open(file, ios::in);
        if (f.is_open())
        {
            string row;
            string tmp = "";
            while (getline(f, row))
            {
                vector<string> record;
                for (int i = 0; i < row.size(); i++)
                {
                    if (row[i] == ' ')
                        continue;
                    else if (row[i] == '\r')
                        continue;
                    else if (row[i] == '\n')
                        continue;

                    if (row[i] == ',' && tmp.size() > 0)
                    {
                        record.push_back(tmp);
                        tmp = "";
                    }
                    else if (row[1] != ',')
                    {
                        tmp += row[i];
                    }
                }
                if (tmp.size() > 0)
                {
                    record.push_back(tmp);
                }
                new_symbol(record[0], record[1]);
            }
            f.close();
        }
        else
        {
            return -1;
        }
        return 1;
    }

    void returnSymTab()
    {
        sym_table = symbolTable;
    }

    void loadSymTab()
    {
        symbolTable = sym_table;
    }

    ~SymbolTable();
};

SymbolTable *tem;

SymbolTable::SymbolTable()
{
}

SymbolTable::SymbolTable(char *f)
{
    file = f;
}

SymbolTable::~SymbolTable()
{
}

class TAC
{
private:
    char *file;

public:
    TAC();
    TAC(char *f);
    ~TAC();
};

TAC::TAC()
{
}

TAC::TAC(char *f)
{
    file = f;
}

TAC::~TAC()
{
}

// argc[1] -> tac.txt
// argc[2] -> symbol.txt
int main(int argv, char **argc)
{
    new_quad_index("", "", "", ""); // empty field
    ds.push_back(0);
    SymbolTable s(argc[1]);
    tem = &s;
    if (s.read_symbol() == -1)
    {
        return 1;
    }
    s.returnSymTab();
    if (address_code_reader(argc[2]) == -1)
    {
        return 1;
    }
    execute();
}

int index_finder(string a)
{
    for (int i = 0; i < 12; i++)
    {
        if (a == type_arr[i])
        {
            return i;
        }
    }
    return -1;
}

vector<string> tokenize_TAC_row(string row)
{
    vector<string> vs;
    int count = 0;
    row += " ";
    int len = row.size();
    bool isStr = false;
    string tmp;
    for (int i = 0; i < len; i++)
    {
        if (row[i] == '\n' || row[i] == '\r')
        {
            continue;
        }
        if (row[i] == ' ')
        {
            if (isStr)
            {
            }
            else
            {
                if (tmp.size() > 0)
                {
                    vs.push_back(tmp);
                    tmp = "";
                    count = 0;
                }
            }
        }
        else
        {
            tmp.push_back(row[i]);
            if (row[i] == '"')
            {
                isStr = (!isStr);
                if (isStr == false)
                {
                    vs.push_back(tmp);
                    tmp = "";
                }
            }
        }
    }
    if (tmp.size() > 0)
    {
        vs.push_back(tmp);
    }
    return vs;
}

int tokenize(char *file)
{
    fstream f;
    f.open(file, ios::in);
    if (f.is_open())
    {
        string row;
        while (getline(f, row))
        {
            bool condition = false;
            bool isStr = false;
            string tmp_str;
            vector<string> vs = tokenize_TAC_row(row);
            for (int i = 0; i < vs.size(); i++)
            {
                tok tk;
                tk.lex = vs[i];
                if (!(vs[i].compare("=")) && condition)
                {
                    tk.type = index_finder("BOOLEAN_OPERATOR");
                }
                else if (!(vs[i].compare("=")))
                {
                    tk.type = index_finder("ASSIGN");
                }
                else if (!vs[i].compare("+"))
                {
                    tk.type = index_finder("CALCULATION_OPERATOR");
                }
                else if (!vs[i].compare("+"))
                {
                    tk.type = index_finder("CALCULATION_OPERATOR");
                }
                else if (!vs[i].compare("-"))
                {
                    tk.type = index_finder("CALCULATION_OPERATOR");
                }
                else if (!vs[i].compare("*"))
                {
                    tk.type = index_finder("CALCULATION_OPERATOR");
                }
                else if (!vs[i].compare("/"))
                {
                    tk.type = index_finder("CALCULATION_OPERATOR");
                }
                else if (!vs[i].compare(">"))
                {
                    tk.type = index_finder("BOOLEAN_OPERATOR");
                }
                else if (!vs[i].compare("<"))
                {
                    tk.type = index_finder("BOOLEAN_OPERATOR");
                }
                else if (!vs[i].compare(">="))
                {
                    tk.type = index_finder("BOOLEAN_OPERATOR");
                }
                else if (!vs[i].compare("<="))
                {
                    tk.type = index_finder("BOOLEAN_OPERATOR");
                }
                else if (!vs[i].compare("if"))
                {
                    condition = true;
                    tk.type = index_finder("IF");
                }
                else if (!vs[i].compare("in"))
                {
                    tk.type = index_finder("INPUT");
                }
                else if (!vs[i].compare("out"))
                {
                    tk.type = index_finder("PRINT");
                }
                else if (!vs[i].compare("ret"))
                {
                    tk.type = index_finder("RETURN");
                }
                else if (!vs[i].compare("goto"))
                {
                    tk.type = index_finder("JUMP");
                }
                else
                {
                    if (vs[i][0] == '"')
                    {
                        if (vs[i].size() > 1)
                        {
                            if (vs[i][vs[i].size() - 1] == '"')
                            {
                                isStr = false;
                                tk.type = index_finder("STR_VAL");
                            }

                            if (!isStr)
                            {
                                isStr = !isStr;
                                tmp_str = vs[i];
                            }
                            else
                            {
                                isStr = !isStr;
                                tmp_str += vs[i];
                                tk.lex = tmp_str;
                                tk.type = index_finder("STR_VAL");
                            }
                        }
                    }
                    else if (vs[i][vs[i].size() - 1] == '"')
                    {
                        if (!isStr)
                        {
                            isStr = !isStr;
                            tmp_str = vs[i];
                        }
                        else
                        {
                            isStr = !isStr;
                            tmp_str += vs[i];
                            tk.lex = tmp_str;
                            tk.type = index_finder("STR_VAL");
                        }
                    }
                    else if (isStr)
                    {
                        tmp_str += vs[i];
                    }
                    else if (vs[i][0] >= '0')
                    {
                        if (vs[i][0] <= '9')
                            tk.type = 2;
                    }
                    else
                    {
                        tk.type = 1;
                    }
                }
                if (!isStr)
                    arr_toks.push_back(tk);
            }
            tok tk;
            tk.lex = "";
            tk.type = index_finder("END");
            arr_toks.push_back(tk);
        }
    }
    else
    {
        return -1;
    }
    f.close();
    return 1;
}

string get_address(string id)
{

    int i = 0;
    bool found = false;
    string ret = "";
    while (i < sym_table.size())
    {
        if (!sym_table[i][0].compare(id))
        {
            ret = sym_table[i][2];
            found = true;
        }
        i++;
    }

    if (found)
        return ret;
    else
        return "-1";
}

string address_of_datatype(tok tk, int t, string type_str)
{
    if (tk.type == t)
    {
        return get_address(new_t_var(type_str, tk.lex));
    }
    else
        return get_address(tk.lex);
}

void new_quad_index(string operator_code = "", string argument_1 = "", string argument_2 = "", string result = "")
{
    vector<string> temp;
    temp.resize(4);
    temp[0] = operator_code;
    temp[1] = argument_1;
    temp[2] = argument_2;
    temp[3] = result;
    quad.push_back(temp);
}

int address_code_reader(char *file)
{
    int r = tokenize(file);
    if (r == -1)
        return -1;
    for (int i = 0; i < arr_toks.size(); i++)
    {
        tok tk = arr_toks[i];
        int j = i;
        bool num_flag = false;
        if (tk.type == 1)
        {
            i++;
            if (arr_toks[i].type == index_finder("ASSIGN"))
            {
                i++;
                if (arr_toks[i].type == 1 || arr_toks[i].type == 2)
                {
                    i++;
                    if (arr_toks[i].type == index_finder("END"))
                    {
                        tok t1 = arr_toks[j], t2 = arr_toks[j + 2];
                        string a, b;
                        a = get_address(t1.lex);
                        b = address_of_datatype(t2, index_finder("NUMERIC_VALUE"), types[0]);
                        new_quad_index("=", b, "", a);
                    }
                    else if (arr_toks[i].type == index_finder("CALCULATION_OPERATOR"))
                    {
                        i++;
                        if (arr_toks[i].type == 2 || arr_toks[i].type == 1)
                        {
                            i++;
                            if (arr_toks[i].type == index_finder("END"))
                            {
                                tok t1 = arr_toks[j],
                                    t2 = arr_toks[j + 2],
                                    t3 = arr_toks[j + 4];
                                string a[3];
                                a[0] = get_address(t1.lex);
                                a[1] = address_of_datatype(t2, index_finder("NUMERIC_VALUE"), types[0]);
                                a[2] = address_of_datatype(t3, index_finder("NUMERIC_VALUE"), types[0]);
                                new_quad_index("+", a[1], a[2], a[0]);
                            }
                        }
                    }
                    else
                    {
                        return -1;
                    }
                }
            }
            else
            {
                return -1;
            }
        }
        else if (tk.type == 3)
        {
            i++;
            if ((arr_toks[i].type == index_finder("VARIABLE")))
            {
                i++;
                if (arr_toks[i].type == index_finder("END"))
                {
                    tok tk = arr_toks[j + 1];
                    string a = get_address(tk.lex);
                    new_quad_index("IN", a);
                }
            }
            else
            {
                return -1;
            }
        }
        else if (tk.type == 4)
        {
            i++;
            if ((arr_toks[i].type == 1 || arr_toks[i].type == index_finder("STR_VAL")))
            {
                i++;
                if (arr_toks[i].type == index_finder("END"))
                {
                    tok tk = arr_toks[j + 1];
                    string a = address_of_datatype(tk, index_finder("STR_VAL"), "CHAR*");
                    if (tk.type != index_finder("STR_VAL"))
                    {
                        new_quad_index("OUT", a);
                    }
                    else
                    {
                        new_quad_index("OUT_S", a);
                    }
                }
            }
            else
            {
                return -1;
            }
        }
        else if (tk.type == 8)
        {
            i++;
            if ((arr_toks[i].type == index_finder("NUMERIC_VALUE")) && arr_toks[++i].type == index_finder("END"))
            {
                i++;
                if (arr_toks[i].type == index_finder("END"))
                {
                    tok tk = arr_toks[j + 1];
                    string a = address_of_datatype(tk, index_finder("NUMERIC_VALUE"), types[0]);
                    new_quad_index("GOTO", a);
                }
            }
            else
            {
                return -1;
            }
        }
        else if (tk.type == 9)
        {
            i++;
            if ((arr_toks[i].type == 1 || arr_toks[i].type == 2))
            {
                i++;
                if (arr_toks[i].type == index_finder("BOOLEAN_OPERATOR"))
                {
                    i++;
                    if (arr_toks[i].type == 1 || arr_toks[i].type == 2)
                    {
                        i++;
                        if (arr_toks[i].type == index_finder("JUMP"))
                        {
                            i++;
                            if (arr_toks[i].type == 2)
                            {
                                i++;
                                if (arr_toks[i].type == index_finder("END"))
                                {
                                    tok tmp = arr_toks[j + 1],
                                        roperator = arr_toks[j + 2], // relational operator
                                        tmp2 = arr_toks[j + 3],
                                        tmp3 = arr_toks[j + 5];
                                    string temp1 = address_of_datatype(tmp, index_finder("NUMERIC_VALUE"), types[0]);
                                    string temp2 = address_of_datatype(tmp2, index_finder("NUMERIC_VALUE"), types[0]);
                                    string temp3 = address_of_datatype(tmp3, index_finder("NUMERIC_VALUE"), types[0]);
                                    new_quad_index(roperator.lex, temp1, temp2, temp3);
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                return -1;
            }
        }
        else if (tk.type == 10)
        {
            i++;
            if ((arr_toks[i].type == 2) && arr_toks[++i].type == index_finder("END"))
            {
                tok tk = arr_toks[j + 1];
                string a = address_of_datatype(tk, index_finder("NUMERIC_VALUE"), types[0]);
                new_quad_index("RET", a);
            }
            else
            {
                return -1;
            }
        }
    }
    return 1;
}

string new_t_var(string type, string data = "")
{
    string t = "t";
    string temp_var;
    bool flag = true;
    for (int i = 1; flag == true; i++)
    {
        temp_var = t + to_string(i);
        for (int j = 0; j < sym_table.size(); ++j)
        {
            if ((sym_table[j][0].compare(t + to_string(i))))
            { // if doesnt match
                flag = false;
            }
        }
    }
    tem->loadSymTab();
    return tem->new_symbol(temp_var, type, data);
}

void execute()
{
    int len = quad.size();

    int pc = 1;
    while (pc < len)
    {
        string opcode = quad[pc][0];
        int argument_1;
        int argument_2;
        int result;
        if (quad[pc][1].size() > 0)
            argument_1 = stoi(quad[pc][1]);
        else
            argument_1 = -1;

        if (quad[pc][2].size() > 0)
            argument_2 = stoi(quad[pc][2]);
        else
            argument_2 = -1;

        if (quad[pc][3].size() > 0)
            result = stoi(quad[pc][3]);
        else
            result = -1;

        if ((!opcode.compare("=")))
        {
            if (argument_2 == -1)
            {
                ds[result] = ds[argument_1];
            }
            else
            {
                if (ds[argument_1] == ds[argument_2])
                {
                    pc = ds[result] - 1;
                }
            }
        }
        else if ((!opcode.compare("OUT")))
        {
            cout << ds[argument_1];
        }
        else if ((!opcode.compare("IN")))
        {
            int n;
            cin >> n;
            ds[argument_1] = n;
        }
        else if ((!opcode.compare("GOTO")))
        {
            pc = ds[argument_1] - 1;
        }
        else if ((!opcode.compare("OUT_S")))
        {
            for (int i = argument_1 + 1; ds[i] != 34; i++)
            {
                cout << (char)ds[i];
            }
        }
        else if ((!opcode.compare("RET")))
        {
            break;
        }
        else if ((!opcode.compare("-")))
        {
            ds[result] = ds[argument_1] - ds[argument_2];
        }
        else if ((!opcode.compare("+")))
        {
            ds[result] = ds[argument_1] + ds[argument_2];
        }
        else if ((!opcode.compare("/")))
        {
            ds[result] = ds[argument_1] / ds[argument_2];
        }
        else if ((!opcode.compare("*")))
        {
            ds[result] = ds[argument_1] * ds[argument_2];
        }
        else if ((!opcode.compare(">")))
        {
            if (ds[argument_1] > ds[argument_2])
            {
                pc = ds[result] - 1;
            }
        }
        else if ((!opcode.compare("<")))
        {
            if (ds[argument_1] < ds[argument_2])
            {
                pc = ds[result] - 1;
            }
        }
        else if ((!opcode.compare("<=")))
        {
            if (ds[argument_1] <= ds[argument_2])
            {
                pc = ds[result] - 1;
            }
        }
        else if ((!opcode.compare(">=")))
        {
            if (ds[argument_1] >= ds[argument_2])
            {
                pc = ds[result] - 1;
            }
        }
        pc++;
    }
}
