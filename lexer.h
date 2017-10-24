#pragma once
#include<string>
#include<array>
#include<vector>
#include<algorithm>
#include<map>
class LexException
{
public:
	size_t index;
};


class Lexer final
{
public:
	enum Token{
		Num = 128, Fun, Sys, Glo, Loc, Id, Str,Ident,Symbol,End,
		Char, Else, Enum, If, Int, Return, Sizeof, While,
		Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
	};
public:


	explicit Lexer(const std::string src) :src(src) { _init(); }
	explicit Lexer(std::string&& src) :src(std::move(src)) { _init(); }
	Token get_next_token();
	std::string get_token() const { return curr; }
private:
	static bool is_num(char c) { return c >= '0'&&c <= '9'; }
	static bool is_char(char c) { return ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z')||c=='_'); }
	static bool is_white(char c) { return (c == ' ' || c == '\r' || c == '\n'||c=='\t'); }
	static bool is_op(char c);
	static Token is_legalop(std::string op);
	void eat_whitespace();
	char get_c() { if (index >= src.size())return -1; return src[index++]; }
	void _init();

private:
	size_t index=0,line=0;
	std::string src;
	std::string curr;
	static std::vector<char> op;
	static std::map <std::string, Token > op_map;
};

std::vector<char> Lexer::op;
std::map <std::string, Lexer::Token > Lexer::op_map;


void Lexer::_init()
{
	decltype(op) _op= { '+','-','*','/','!','%','^','&','|','<','>','=','~'};
	op.swap(_op);
	decltype(op_map) _op_map= {
		{"+",And},{"-",Sub},{"*",Mul},{"/",Div},{">",Gt},{">=",Ge},{"<",Lt},{"<=",Le},{"=",Assign},{"==",Eq},
		{"^",Xor},{"|",Or},{"&",And},{"<<",Shl},{">>",Shr},{"!=",Ne}
	};
	op_map.swap(_op_map);
}

bool Lexer::is_op(char c)
{
	if (std::find(op.begin(), op.end(), c) != op.end())
		return true;
	return false;
}

Lexer::Token Lexer::is_legalop(std::string op)
{
	if (op_map.find(op) != op_map.end())
		return op_map[op];
	throw;
}

void Lexer::eat_whitespace()
{
	while (1)
	{
		char c = get_c();
		if (c == ' ' || c == '\r'||c=='\t')
			continue;
		else if (c == '\n')
			++line;
		else
		{
			--index;
			break;
		}
	}
}

Lexer::Token Lexer::get_next_token()
{
	curr = "";
	enum State{BEGIN,CHAR,STRING,NUMBER,IDENT,SYMBOL,OP};
	bool is_add = true, is_end = false;
	State state=BEGIN;

	while (char c = get_c())
	{
		if (static_cast<int>(c) < 0)
			break;

		is_add = true; is_end = false;

		switch (state)
		{
		case BEGIN:
			if (is_char(c))
				state = IDENT;
			else if (is_num(c))
				state = NUMBER;
			else if (is_white(c))
			{
				is_add = false;
				eat_whitespace();
			}
			else if (is_op(c))
			{
				state = OP;
			}
			else if (c == '\'')
			{
				state = CHAR;
				is_add = false;
			}
			else if (c == '\"')
			{
				state = STRING;
				is_add = false;
			}
			else
				state = SYMBOL;
			break;
		case CHAR:
			if (c == '\'')
			{
				is_add = false;
				is_end = true;
			}
			break;
		case STRING:
			if (c == '\"')
			{
				is_add = false;
				is_end = true;
			}
			break;
		case NUMBER:
			if (!is_num(c)&&c!='.')
			{
				is_add = false;
				is_end = true;
				--index;
			}
			break;
		case IDENT:
			if (is_char(c)||is_num(c)){}
			else
			{
				is_add = false;
				is_end = true;
				--index;
			}

			break;
		case SYMBOL:
			is_add = false;
			is_end = true;
			--index;
			break;
		case OP:
			if (!is_op(c))
			{
				is_add = false;
				is_end = true;
				--index;
			}
		}


		if (is_add)
			curr += c;
		if (is_end)
			break;
	}

	switch (state)
	{
	case BEGIN:
		return End;
	case CHAR:
		return Char;
	case STRING:
		return Str;
	case NUMBER:
		return Num;
	case IDENT:
		return Ident;
	case SYMBOL:
		return Symbol;
	case OP:
		return is_legalop(curr);
	}
}
