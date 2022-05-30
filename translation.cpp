#include "translation.h"
#include "fstream"

void translation::emit(string s)
{
  _code.push_back(s);
  cout << s << endl;

  incLine();
}

void translation::backpatch(int line, int gotoLine)
{
  _code[line].append(to_string(gotoLine));
}

int translation::getLine()
{
  return _line;
}

void translation::incLine()
{
  _line++;
}

void translation::toFile(string filename)
{
  ofstream outfile;
  outfile.open(filename);

  for (int i = 0; i < _code.size(); i++)
  {
    outfile << _code[i] << endl;
  }

  outfile.close();
}