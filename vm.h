// ��������-ģ��ִ�л��ָ��

#pragma once

#include<cstdlib>
#include<iostream>

#define private public



class Vm final
{
public:
	enum  IR{
		LEA, IMM, JMP, CALL, JZ, JNZ, ENT, ADJ, LEV, LI, LC, SI, SC, PUSH,
		OR, XOR, AND, EQ, NE, LT, GT, LE, GE, SHL, SHR, ADD, SUB, MUL, DIV, MOD,
		OPEN, READ, CLOS, PRTF, MALC, MSET, MCMP, EXIT,RET
	};

public:
	Vm();
	~Vm();
	void run();
private:
	static const int poolsize = 1000;

	int *text, *old_text, *stack; //����Σ���ջ��
	char* data; //���ݶ�
	int *pc, *bp, *sp, ax, cycle; //�Ĵ���
private:
	void eval_one(IR ir); //ִ��һ�����
	void eval();

};
Vm::~Vm()
{
	delete[] text, data, stack;
}
void Vm::run()
{
	eval();
}
void Vm::eval()
{
	while (true)
	{
		IR op = static_cast<IR>(*pc++);
		eval_one(op);
		if (op == EXIT)
			return;
	}
}

void Vm::eval_one(IR op)
{
	int* tmp = nullptr;
	switch (op)
	{
	case IMM: //IMM <num> ������num->ax
		ax = *pc++;
		break;
	case LC:  //[ax] char -> ax 
		ax = *(char*)ax;
		break;
	case LI:  //[ax] int -> ax
		ax = *(int*)ax;
		break;
	case SC:  //ax char -> [sp], sp++
		*(char*)(*sp++) = ax;
		break;
	case SI:  //ax int -> [sp], sp++
		*(int*)(*sp++) = ax;
		break;
	case PUSH:
		*--sp = ax;
		break;
	case JMP:
		pc = (int*)*pc;
		break;
	case JZ:
		pc = ax ? pc + 1 : (int*)*pc;
		break;
	case JNZ:
		pc = ax ? (int*)*pc : pc + 1;
		break;
	case CALL:
		*--sp = (int)(pc + 1); //���浱ǰָ�����һ��ָ���ַ
		pc = (int*)*pc; //��ת
		break;
	case RET:
		pc = (int*)*sp++;
		break;
	case ENT: //make new call frame ���浱ǰ��ջָ�룬ͬʱ��ջ�ϱ���һ���ռ�
		*--sp = (int)bp; // �����ַ�Ĵ���
		bp = sp;
		sp = sp - *pc++; // *pc��Ϊ��Ҫ����ռ�Ĵ�С
		break;
	case ADJ: //remove arguments from frame�������Ӻ���ʱѹ��ջ�е��������
		sp = sp + *pc++;
		break;
	case LEV: // POP RET
		sp = bp; 
		bp = (int*)*sp++;
		pc = (int*)*sp++;
		break;
	case LEA: // LEA <offset> ȡ����
		ax = (int)(bp + *pc++);
		break;

	//------����Ϊ�����ָ��-------
	case OR:
		ax = *sp++ | ax;
		break;
	case XOR:
		ax = *sp++ ^ ax;
		break;
	case AND:
		ax = *sp++ & ax;
		break;
	case EQ:
		ax = *sp++ == ax;
		break;
	case NE:
		ax = *sp++ != ax;
		break;
	case LT:
		ax = *sp++ < ax;
		break;
	case LE:
		ax = *sp++ <= ax;
		break;
	case GT:
		ax = *sp++ > ax;
		break;
	case GE:
		ax = *sp++ >= ax;
		break;
	case SHL:
		ax = *sp++ << ax;
		break;
	case SHR:
		ax = *sp++ >> ax;
		break;
	case ADD:
		ax = *sp++ + ax;
		break;
	case SUB:
		ax = *sp++ - ax;
		break;
	case MUL:
		ax = *sp++ * ax;
		break;
	case DIV:
		ax = *sp++ / ax;
		break;
	case MOD:
		ax = *sp++ % ax;
		break;

	//-----����Ϊ���ú���-----
	case EXIT:
		std::cout << "exit " << *sp;
		break;
	case OPEN:
		break;
	case CLOS:
		break;
	case READ:
		break;
	case PRTF:
		tmp = sp + pc[1];
		for (int i = 1; i <= 6; ++i)
			std::cout << tmp[-i];
		ax = 6;
		break;
	case MALC:
		ax = (int)std::malloc(*sp);
		break;
	case MSET:
		break;
	case MCMP:
		break;
	default:
		std::cerr << "unknown instruction:" << op;
		break;
	}
}
Vm::Vm()
{
	// alloc
	if (!(text = old_text = static_cast<int*>(std::malloc(poolsize))))
	{
		std::cerr << "alloc for text error";
		exit(0);
	}
	if (!(data = static_cast<char*>(std::malloc(poolsize))))
	{
		std::cerr << "alloc for data error";
		exit(0);
	}
	if (!(stack = static_cast<int*>(std::malloc(poolsize))))
	{
		std::cerr << "alloc for data error";
		exit(0);
	}

	// set 0
	memset(text, 0, poolsize);
	memset(data, 0, poolsize);
	memset(stack, 0, poolsize);

	bp = sp = static_cast<int*>(stack + poolsize); //Ĭ��ָ��ջ��
	ax = 0;
}