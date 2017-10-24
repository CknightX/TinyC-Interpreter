#include "vm.h"
#include "lexer.h"
void vm_test()
{
	Vm vm;
	int text[8] = { Vm::IMM,10,Vm::PUSH,Vm::IMM,20,Vm::ADD,Vm::PUSH,Vm::EXIT };
	vm.pc = text;
	vm.run();
	std::cout << std::endl;
}

void lexer_test()
{
	std::string src1 = "\
		void vm_test()\
	{\
		Vm vm;\
		int text[8] = { Vm::IMM,10,Vm::PUSH,Vm::IMM,20,Vm::ADD,Vm::PUSH,Vm::EXIT };\
		vm.pc = text;\
		vm.run();\
	}";

	std::string src2 = "== = >= <= < >> << !=";
	Lexer lex(src1);
	auto curr = lex.get_next_token();
	while ((curr=lex.get_next_token()) != Lexer::End)
	{
		std::cout << lex.get_token()<<std::endl;
	}
}