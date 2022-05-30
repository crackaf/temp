#include <iostream>
#include "parser.h"
using namespace std;

int main(int argc, char *argv[])
{
	// asking for file name as command line arguments
	if (argc == 2)
	{
		// lexer _lexer(argv[1]);
		// token t;
		// t = _lexer.getNextToken();
		// while (t.tokenType != TokenType::END_OF_FILE)
		// {
		// 	t.Print();
		// 	t = _lexer.getNextToken();
		// }
		parser _parser(argv[1]);
		try
		{
			bool const result = _parser.START().b;
			cout << endl
					 << result << endl;

			vector<string> ret = _parser.TAC();
			for (size_t i = 0; i < ret.size(); i++)
			{
				cout << i << ". " << ret[i] << endl;
			}
		}
		catch (const std::exception &e)
		{
			vector<string> ret = _parser.TAC();
			for (auto s : ret)
			{
				cout << s << endl;
			}

			std::cerr << e.what() << '\n';
		}
		_parser.toFileTAC("tac.txt");
		_parser.printSymbols();
	}
	else if (argc > 2)
	{ // argument limit exceeds
		cout << "Too many arguments" << endl;
	}
	else // if file name is'nt given
	{
		cout << "Please provide a file name" << endl;
	}
	return 0;
}
