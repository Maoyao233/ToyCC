/// @file lexer.cpp
/// A OI-style lexer
/// todo: code refactoring

#include <iostream>
#include <cstring>
#include "token.h"
#include "lexer.h"
using namespace std;

/**
 * @brief Add a token to the Trie
 * @param rt Root of the Trie
 * @param s The string to insert
 * @param kind of the token
*/
void Lexer::add(int rt, string s, TokenKind kind)
{
	int pos = rt;
	int m = s.length();
	for (int i = 0; i < m; i++)
	{
		if (!tr[pos][s[i]])
		{
			tr[pos][s[i]] = ++cnt;
		}
		pos = tr[pos][s[i]];
	}
	f[pos] = kind;
}
void Lexer::spsolve_root(int rt, TokenKind flag)
{
	f[rt] = flag;
}
int Lexer::build_alpha()
{
	int rt = ++cnt;
	add(rt, "int", TokenKind::kw_int);
	add(rt, "void", TokenKind::kw_void);
	add(rt, "if", TokenKind::kw_if);
	add(rt, "else", TokenKind::kw_else);
	add(rt, "while", TokenKind::kw_while);
	add(rt, "return", TokenKind::kw_return);
	spsolve_root(rt, TokenKind::identifier);
	return rt;
}
int Lexer::build_number()
{
	int rt = ++cnt;
	spsolve_root(rt, TokenKind::numeric_constant);
	return rt;
}
int Lexer::build_sign()
{
	int rt = ++cnt;
	add(rt, "=", TokenKind::equal);
	add(rt, "+", TokenKind::plus);
	add(rt, "+=", TokenKind::plusequal);
	add(rt, "-", TokenKind::minus);
	add(rt, "-=", TokenKind::minusequal);
	add(rt, "*", TokenKind::star);
	add(rt, "*=", TokenKind::starequal);
	add(rt, "/", TokenKind::slash);
	add(rt, "/=", TokenKind::slashequal);
	add(rt, "%", TokenKind::percent);
	add(rt, "%=", TokenKind::percentequal);
	add(rt, "==", TokenKind::equalequal);
	add(rt, ">", TokenKind::greater);
	add(rt, ">>", TokenKind::greatergreater);
	add(rt, ">>=", TokenKind::greatergreaterequal);
	add(rt, ">=", TokenKind::greaterequal);
	add(rt, "<", TokenKind::less);
	add(rt, "<<", TokenKind::lessless);
	add(rt, "<<=", TokenKind::lesslessequal);
	add(rt, "<=", TokenKind::lessequal);
	add(rt, "!=", TokenKind::exclaimequal);
	add(rt, "^", TokenKind::caret);
	add(rt, "^=", TokenKind::caretequal);
	add(rt, "&", TokenKind::amp);
	add(rt, "&=", TokenKind::ampequal);
	add(rt, "&&", TokenKind::ampamp);
	add(rt, "|", TokenKind::pipe);
	add(rt, "||", TokenKind::pipepipe);
	add(rt, "|=", TokenKind::pipeequal);
	add(rt, ";", TokenKind::semi);
	add(rt, ",", TokenKind::comma);
	add(rt, "#", TokenKind::eof);
	add(rt, "(", TokenKind::l_paren);
	add(rt, ")", TokenKind::r_paren);
	add(rt, "{", TokenKind::l_brace);
	add(rt, "}", TokenKind::r_brace);
	add(rt, "//", TokenKind::s_comment);
	add(rt, "/*", TokenKind::lm_comment);
	add(rt, "*/", TokenKind::rm_comment);
	return rt;
}
std::vector<Token> Lexer::Lex()
{
	char s[1025];
	while (in.getline(s, 1024))
	{
		row++;
		int max_col = strlen(s);
		for (col = 0; col < max_col; col++)
		{
			int length = 0;
			TokenKind flag = TokenKind::unknown;
			switch (check_classify(s[col]))
			{
				case 1:
					solve_alpha(root_alpha, s, max_col, col, length, flag);
					break;
				case 2:
					solve_number(root_number, s, max_col, col, length, flag);
					break;
				case 3:
					solve_sign(root_sign, s, max_col, col, length, flag);
					break;
				case 4:
					flag = TokenKind::unknown;
					break;
			}
			if (flag == TokenKind::unknown)
			{
				continue;
			}
			else if (flag == TokenKind::s_comment)
			{
				break;
			}
			else if (flag == TokenKind::lm_comment)
			{
				readin = 0;
			}
			else if (flag == TokenKind::rm_comment)
			{
				readin = 1;
			}
			else
			{
				if (!readin)
				{
					col += length - 1;
					continue;
				}
				if (flag == TokenKind::eof)
				{
					stop = 1;
				}
				
				//tokens[tot].kind = flag;
				string tmp;
				for (int i = col; i <= col + length - 1; i++)
					tmp = tmp + s[i];
				Location loc{ row,col+1 };
				tokens.emplace_back(Token{ flag,tmp,loc });
			}
			col += length - 1;
		}
		if (stop)
		{
			break;
		}
	}
	if (tokens.rbegin()->kind != TokenKind::eof)
	{
		tokens.push_back({ TokenKind::eof, "", Location{row + 1, 0} });
	}
	return tokens;
}
int Lexer::check_classify(char ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return 1;
	else if (ch >= '0' && ch <= '9')
		return 2;
	else
	{
		switch (ch)
		{
			case '=':
			case '+':
			case '-':
			case '*':
			case '/':
			case '%':
			case '>':
			case '<':
			case '!':
			case ';':
			case ',':
			case '#':
			case '(':
			case ')':
			case '{':
			case '}':
			case '^':
			case '|':
			case '&':
				return 3;
				break;
			default:
				return 4;
				break;
		}
	}
	return 0;
}
void Lexer::solve_alpha(int rt, char* s, int m, int l, int& len, TokenKind& flag)
{
	int pos = rt;
	bool sg = 0;
	len = 0;
	for (int i = l; i < m; i++)
	{
		if (check_classify(s[i]) < 3)
		{
			len++;
			if (tr[pos][s[i]] && !sg)
			{
				pos = tr[pos][s[i]];
			}
			else
			{
				sg = 1;
			}
		}
		else
		{
			break;
		}
	}
	if (!sg && f[pos]!=TokenKind::unknown)
	{
		flag = f[pos];
	}
	else
	{
		flag = f[rt];
	}
}
void Lexer::solve_number(int rt, char* s, int m, int l, int& len, TokenKind& flag)
{
	len = 0;
	for (int i = l; i < m; i++)
	{
		if (check_classify(s[i]) == 2)
		{
			len++;

		}
		else
		{
			break;
		}
	}
	flag = f[rt];
}
void Lexer::solve_sign(int rt, char* s, int m, int l, int& len, TokenKind& flag)
{
	int pos = rt;
	bool sg = 0;
	len = 0;
	for (int i = l; i < m; i++)
	{
		if (check_classify(s[i]) == 3)
		{
			if (tr[pos][s[i]])
			{
				pos = tr[pos][s[i]];
				len++;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	if (f[pos]!=TokenKind::unknown)
	{
		flag = f[pos];
	}
}

