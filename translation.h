#ifndef _TRANSLATION_H_
#define _TRANSLATION_H_
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class translation
{
private:
  vector<string> _code;
  int _line = 0;

public:
  translation() : _line(0) {}
  ~translation() {}

  void emit(string);

  void backpatch(int, int);

  void toFile(string);

  // getter setters
  int getLine();
  vector<string> getTAC()
  {
    return _code;
  }

  void incLine();
};

#endif
