#define _CRT_SECURE_NO_WARNINGS
#include"Core_x86.h"
#include <stdio.h>
#include <malloc.h>
#include<time.h>
#include<stdlib.h>

typedef struct Fraction {
	int A = 0;	// 
	int B = 0;	//分子 
	int C = 1;	//分母 
}Fraction;

typedef struct TNode {
	int type;
	int data1 = 0;
	float data2 = 0;
	Fraction fenshu;
	char signal = '\0';
	struct TNode *lchild, *rchild;
}TNode;



TNode *T;
int wrong = 0;
int Siganl_type[6] = { 1,1,1,1,0,1 };//用于决定是哪几种操作符，先预设为不生成除法
int Number_type[3] = { 1,0,0 };
int two[2][2] = { { 0,-1 },{ 1,-1 } };//决定以那种方式填满树中的操作符
int three[5][4] = { { 0,0,-1,-1 },{ 0,1,-1,-1 },{ 1,0,-1,-1 },{ 1,1,-1,-1 },{ 0,2,1,-1 } };
int four[12][5] = { { 0,0,0,-1,-1 },{ 0, 0,1,-1,-1 },{ 0,1,0,-1,-1 },{ 0,1,1,-1,-1 },{ 1,0,0,-1,-1 },{ 1,0,1,-1,-1 },{ 1,1,0,-1,-1 },{ 1,1,1,-1,-1 },{ 0,0,2,1,-1 },{ 0,1,2,1,-1 },{ 0,2,1,0,-1 },{ 0,2,1,1,-1 } };
int num_num = 5, question_number = 50, end = 20;
int bktornot = 0;
void InitTree(TNode* &T, int temp)//创建5层二叉树
{

	T = (TNode*)malloc(sizeof(TNode));
	//T->fenshu=(Fraction*)malloc(sizeof(Fraction));
	T->type = -1;
	//T->data1 = 0;
	//T->signal = '\0';
	//T->data2 = 0;
	temp++;

	if (temp<5)
		InitTree(T->lchild, temp);

	if (temp<5)
		InitTree(T->rchild, temp);
	if (temp == 5) {
		T->lchild = NULL;
		T->rchild = NULL;
	}

	return;
}

void Empty(TNode *p)//置空 
{
	if (!p)
		return;
	else
	{
		Empty(p->lchild);
		Empty(p->rchild);
		p->type = -1;
		p->signal = 0;
	}
}

int random_int(int start, int end)//生成start到end范围的整数
{
	return (int)(start + (end + 1 - start)*rand() / (RAND_MAX + 1.0));
}

float random_float(float start, int end)
{
	return (float)((rand() % (100 * end))*0.01);
}

Fraction random_fenshu(int end) {
	Fraction fenshu;
	fenshu.A = rand() % end;
	fenshu.C = 2 + rand() % (end - 1);
	do {
		fenshu.B = 1 + rand() % (end);
	} while (fenshu.B >= fenshu.C);
	return fenshu;
	//printf("%d%d%d\n", fenshu->A,fenshu->B,fenshu->C);
}

int find_gcd(int n1, int n2)//找出最大公约数
{
	int temp;

	while (n1 != 0) {
		temp = n2 % n1;
		n2 = n1;
		n1 = temp;
	}

	return n2;
}

Fraction reduce_fenshu(Fraction fenshu)//化简分数
{
	int gcd = find_gcd(fenshu.B, fenshu.C);
	fenshu.B /= gcd;
	fenshu.C /= gcd;
	//printf("%d%d%d\n", fenshu->A,fenshu->B,fenshu->C);
	return fenshu;
}

int getOperOrder(char signal)	//返回操作符的优先级，“*/”为1；“+、-” 为0 
{
	if (signal == '+' || signal == '-')
		return 0;
	else if (signal == '*')
		return 2;
	else if (signal == '/')
		return 3;
	else if (signal == '^')
		return 5;

}

char getSignal(int Signal_type[])//按照SiganalType的要求随机生成操作符
{
	char signal[4] = { '+','-','*','/' };
	int x;
	do {
		x = rand() % 4;

	} while (Signal_type[x] == 0);
	return signal[x];
}

void preOrder(TNode *p, int temp)//后序遍历
{
	if (!p)
		return;
	else
	{
		temp++;
		preOrder(p->lchild, temp);
		preOrder(p->rchild, temp);
		printf("%d\t", p->type);
		printf("operator is %c\t", p->signal);
		if (p->type == 0)
			printf("number is %d\t", p->data1);
		else if (p->type == 1)
			printf("float is %0.2f\t", p->data2);
		else if (p->type == 2)
		{
			if (p->fenshu.A != 0)
				printf(" fenshu is %d '%d/%d\t", p->fenshu.A, p->fenshu.B, p->fenshu.C);
			else
				printf(" fenshu is %d/%d\t", p->fenshu.B, p->fenshu.C);
		}
		printf("layer is %d\n", temp);
	}

}


void inOrder(TNode *p, FILE *fp) //中序遍历，同时输出不带荣誉括号的中缀表达式
{

	if (p)
	{
		if (p->lchild)
		{	//如果当前节点的左子树 是运算符，且运算符优先级低于当前运算符，那么左边表达式要先计算，需加括号 
			if (p->lchild->type == 3 && getOperOrder(p->signal) - getOperOrder(p->lchild->signal)>1)
			{

				//	printf("( ");
				fprintf(fp, "(");
				inOrder(p->lchild, fp);
				//	printf(" )");
				fprintf(fp, ")");
			}
			//否则直接输出左子树 
			else
				inOrder(p->lchild, fp);
		}
		if (p->type == 0)
		{
			//	printf("%d", p->data1);
			fprintf(fp, "%d", p->data1);
		}
		else if (p->type == 1)
		{
			//	printf("%0.2f", p->data2);
			fprintf(fp, "%0.2f", p->data2);
		}
		else if (p->type == 2)
		{
			if (p->fenshu.A == 0)
			{
				//	printf("%d/%d", p->fenshu.B, p->fenshu.C);
				fprintf(fp, "%d/%d", p->fenshu.B, p->fenshu.C);
			}
			else {
				//	printf("%d'%d/%d", p->fenshu.A, p->fenshu.B, p->fenshu.C);
				fprintf(fp, "%d'%d/%d", p->fenshu.A, p->fenshu.B, p->fenshu.C);
			}
		}

		else if (p->type == 3)
		{
			//	printf(" %c ", p->signal);
			fprintf(fp, " %c ", p->signal);
		}
		if (p->rchild)
		{
			if (p->rchild->type == 3 && getOperOrder(p->rchild->signal)<getOperOrder(p->signal))
			{
				//	printf("( ");
				fprintf(fp, "(");
				inOrder(p->rchild, fp);
				//	printf(" )");
				fprintf(fp, ")");
			}
			else if (p->rchild->signal == '/'&&p->signal == '/'&&p->rchild->type == 3) {
				//	printf("( ");
				fprintf(fp, "(");
				inOrder(p->rchild, fp);
				//	printf(" )");
				fprintf(fp, ")");
			}
			else if (p->rchild->signal == '-'&&p->signal == '-'&&p->rchild->type == 3) {
				//	printf("( ");
				fprintf(fp, "(");
				inOrder(p->rchild, fp);
				//	printf(" )");
				fprintf(fp, ")");
			}
			else
				inOrder(p->rchild, fp);
		}
	}

}


void checkforbkt(TNode *p) //检测括号
{

	if (p)
	{
		if (p->lchild)
		{	//如果当前节点的左子树 是运算符，且运算符优先级低于当前运算符，那么左边表达式要先计算，需加括号 
			if (p->lchild->type == 3 && getOperOrder(p->signal) - getOperOrder(p->lchild->signal)>1)
			{
				if (Siganl_type[5] == 0)
					bktornot = 1;
				checkforbkt(p->lchild);

			}
			//否则直接输出左子树 
			else
				checkforbkt(p->lchild);
		}

		if (p->rchild)
		{
			if (p->rchild->type == 3 && getOperOrder(p->rchild->signal)<getOperOrder(p->signal))
			{
				if (Siganl_type[5] == 0)
					bktornot = 1;
				checkforbkt(p->rchild);

			}
			else if (p->rchild->signal == '/'&&p->signal == '/'&&p->rchild->type == 3) {
				if (Siganl_type[5] == 0)
					bktornot = 1;
				checkforbkt(p->rchild);

			}
			else if (p->rchild->signal == '-'&&p->signal == '-'&&p->rchild->type == 3) {
				if (Siganl_type[5] == 0)
					bktornot = 1;
				checkforbkt(p->rchild);

			}
			else
				checkforbkt(p->rchild);
		}
	}

}
void Put_Operator(int str[]) {//往数中放操作符
	int i = 0;
	TNode*p;
	//printf("str is %d %d", str[0], str[1]);
	p = T;//避免函数中的操作破坏了原来的根节点
	p->signal = getSignal(Siganl_type);//根节点必放入操作符
	p->type = 3;

	while (str[i] != -1) {
		if (str[i + 1] == -1 && Siganl_type[4] == 1 && rand() % 2 == 1)
		{
			if (str[i] == 0)
			{
				p->lchild->signal = '^';
				p->lchild->type = 3;
				p->lchild->rchild->data1 = 2 + rand() % 2;
				p->lchild->rchild->type = 0;
			}
			if (str[i] == 1)
			{
				p->rchild->signal = '^';
				p->rchild->type = 3;
				p->rchild->rchild->data1 = 2 + rand() % 2;
				p->rchild->rchild->type = 0;
			}
			i++;
		}
		else if (str[i] == 0)
		{
			//printf("put in left");
			p->lchild->signal = getSignal(Siganl_type);//p首先是根结点，此时对左子节点赋值
			p->lchild->type = 3;

			p = p->lchild; i++;
		}
		else if (str[i] == 1)
		{
			//printf("put in right");
			p->rchild->signal = getSignal(Siganl_type);
			p->rchild->type = 3;

			p = p->rchild; i++;
		}
		else if (str[i] == 2)//p退回根节点
		{
			p = T; i++;
		}
	}

}

void Put_Number(TNode* T, int Str[], int number_number, int end) {
	TNode*p;
	int number_count, x;
	p = T;

	if (p->lchild->type == -1)
	{
		do {
			x = rand() % 3;
		} while (Str[x] == 0);
		//num_num++;        //保证一定生成小数 
		//if (Str[1] == 1 && x == 1)
		//	float_num++;
		//if (Str[1] == 1 && float_num == 0 && num_num == number_number - 1)
		//	x = 1;
		p->lchild->type = x;
		switch (x) {
		case 0:p->lchild->data1 = random_int(0, end); break;
		case 1:p->lchild->data2 = random_float(0, end); break;
		case 2:p->lchild->fenshu = random_fenshu(end); p->lchild->fenshu = reduce_fenshu(p->lchild->fenshu); break;
		default:break;
		}
	}
	else if (p->lchild->type == 3)
	{
		Put_Number(p->lchild, Number_type, number_number, end);
	}
	if (p->rchild->type == -1)
	{
		do {
			x = rand() % 3;
		} while (Str[x] == 0);
		//	num_num++;
		//	if (Str[1] == 1 && x == 1)
		//		float_num++;
		//	if (Str[1] == 1 && float_num == 0 && num_num == number_number - 1)
		//		x = 1;
		p->rchild->type = x;
		switch (x) {
		case 0:p->rchild->data1 = random_int(0, end); break;
		case 1:p->rchild->data2 = random_float(0, end); break;
		case 2:p->rchild->fenshu = random_fenshu(end); p->rchild->fenshu = reduce_fenshu(p->rchild->fenshu);  break;
		default:break;
		}
	}
	else if (p->rchild->type == 3)
	{
		Put_Number(p->rchild, Number_type, number_number, end);
	}

}
void operator_number(int operator_num) {//接受参数，判断生成多少个操作符，并调用Put_Operator()

	if (operator_num == 2)
	{
		Put_Operator(two[random_int(0, 1)]);
	}
	else if (operator_num == 3)
		Put_Operator(three[random_int(0, 4)]);
	else if (operator_num == 4)
		Put_Operator(four[random_int(0, 11)]);
	else
		printf("wrong operator number \n");

}

Fraction change_to_fraction(float data2) {
	Fraction result;
	result.B = (int)(data2*100.00 + 0.1);
	//printf("\n%d\n", result.B);
	result.C = 100;
	result.A = result.B / result.C;
	result.B = result.B - result.C*result.A;
	//printf("%f\t%d'%d/%d\n", data2, result.A, result.B, result.C);
	return reduce_fenshu(result);
}

Fraction Operate(Fraction a, char op, Fraction b)//分数的运算
{
	Fraction result;
	//	printf("start to operate \t");
	switch (op) {
	case'+':result.B = a.B*b.C + b.B*a.C;
		if (result.B<0)
			wrong = 1;
		result.C = a.C*b.C;
		if (result.C<0)
			wrong = 1;
		if (result.B >= result.C)
		{
			result.A = a.A + b.A + 1;
			result.B = result.B - result.C;
		}
		else
			result.A = a.A + b.A;
		break;
	case'-':if (((a.A*a.C + a.B)*b.C - (b.A*b.C + b.B)*a.C) < 0)
	{
		wrong = 1;
		result.A = 1;
		result.B = 0;
		result.C = 1;
	}
			else {
				result.B = (a.A*a.C + a.B)*b.C - (b.A*b.C + b.B)*a.C;
				if (result.B<0)
					wrong = 1;
				result.C = a.C*b.C;
				if (result.C<0)
					wrong = 1;
				result.A = result.B / result.C;
				result.B = result.B - result.C*result.A;
			}
			break;
	case'*':result.C = a.C*b.C;
		if (result.C<0)
			wrong = 1;
		result.B = (a.A*a.C + a.B)*(b.A*b.C + b.B);
		if (result.B<0)
			wrong = 1;
		result.A = result.B / result.C;
		result.B = result.B - result.C*result.A;
		break;
	case'/':if (b.A == 0 && b.B == 0)
		wrong = 1;
			else
			{
				result.C = a.C*(b.A*b.C + b.B);
				if (result.C<0)
					wrong = 1;
				result.B = b.C*(a.A*a.C + a.B);
				if (result.B<0)
					wrong = 1;
				result.A = result.B / result.C;
				result.B = result.B - result.C*result.A;


			}
			if (Number_type[0] == 1 && Number_type[1] == 0 && Number_type[2] == 0 && result.B != 0)
				wrong = 1;
			break;
	case'^':if (b.A == 2)
	{
		result.C = a.C*a.C;
		if (result.C<0)
			wrong = 1;
		result.B = (a.A*a.C + a.B)*(a.A*a.C + a.B);
		if (result.B<0)
			wrong = 1;
		result.A = result.B / result.C;
		result.B = result.B - result.C*result.A;
	}
			else if (b.A == 3)
			{
				result.C = a.C*a.C*a.C;
				if (result.C<0)
					wrong = 1;
				result.B = (a.A*a.C + a.B)*(a.A*a.C + a.B)*(a.A*a.C + a.B);
				if (result.B<0)
					wrong = 1;
				result.A = result.B / result.C;
				result.B = result.B - result.C*result.A;
			}

			break;


	}
	//if (wrong == 1)//有wrong说明计算过程中出现了负数或分母为0
	//printf("wrong exp\n");
	//printf("%d'%d/%d  %c %d'%d/%d \n= %d'%d/%d\n", a.A, a.B, a.C, op, b.A, b.B, b.C, result.A, result.B, result.C);

	return reduce_fenshu(result);

}

Fraction Calculate(TNode *T)//后序遍历计算
{
	TNode *p = T;
	Fraction int_f, a, b, result;
	if (p != NULL && p->type == 0)
	{
		int_f.A = p->data1;
		int_f.B = 0;
		int_f.C = 1;
		return int_f;
	}
	else if (p != NULL && p->type == 2)
		return p->fenshu;
	else if (p != NULL && p->type == 1)
		return change_to_fraction(p->data2);
	else
	{
		a = Calculate(p->lchild);
		b = Calculate(p->rchild);
		result = Operate(a, p->signal, b);
		return result;
	}

}

int Setting_Parameter(int question_num, int sup, Parameter argument) {
	if (question_num != 0)
		question_number = question_num;
	else if (question_num < 0)
	{
		printf("error question number\n");
		return -1;
	}
	if (sup != 0 && sup >= 1)
		end = sup;
	else if (sup <= 1)
		printf("wrong sup!\n");
	/*
	if (argument.Data_Number != 0 && argument.Data_Number >= 3 && argument.Data_Number <= 5)
	num_num = argument.Data_Number;

	else if (argument.Data_Number < 3 || argument.Data_Number>5)
	{
	printf("error count of number\n");
	return -1;
	}*/
	if (argument.integer == 1 || argument.integer == 0)
		Number_type[0] = argument.integer;

	else
	{
		printf("error integer input!\n");
		return -1;
	}
	if (argument.fraction == 1 || argument.fraction == 0)
		Number_type[2] = argument.fraction;
	else
	{
		printf("error fraction input!\n");
		return -1;
	}
	if (argument.decimal == 1 || argument.decimal == 0)
		Number_type[1] = argument.decimal;
	else
	{
		printf("error decimal input!\n");
		return -1;
	}
	if (argument.integer == 0 && argument.decimal == 0 && argument.fraction == 0)
	{
		printf("至少支持一种类型的数\n");
		return -1;
	}
	if (argument.add == 1 || argument.add == 0)
		Siganl_type[0] = argument.add;
	else
	{
		printf("error add permission!\n");
		return -1;
	}
	if (argument.sub == 1 || argument.sub == 0)
		Siganl_type[1] = argument.sub;
	else
	{
		printf("error subtraction permission!\n");
		return -1;
	}
	if (argument.multiply == 1 || argument.multiply == 0)
		Siganl_type[2] = argument.multiply;
	else
	{
		printf("error multiple permission!\n");
		return -1;
	}
	if (argument.division == 1 || argument.division == 0)
		Siganl_type[3] = argument.division;
	else
	{
		printf("error division permission!\n");
		return -1;
	}
	if (argument.pow == 1 || argument.pow == 0)
		Siganl_type[4] = argument.pow;
	else
	{
		printf("error mtp2 permission!\n");
		return -1;
	}

	if (argument.bracktet == 1 || argument.bracktet == 0)
		Siganl_type[5] = argument.bracktet;
	else
	{
		printf("error plus permission!\n");
		return -1;
	}
	if (argument.add == 0 && argument.sub == 0 && argument.multiply == 0 && argument.division == 0)
	{
		printf("error operator permission!\n");
		return -1;
	}
	return 1;
}

int Generate(int setting_error) {
	float result_float = 0;
	int i, x_Signaltype = 0;;
	Fraction result, test;
	FILE* fp, *fp_key;
	srand((unsigned)time(NULL));
	InitTree(T, 0);
	//printf("dffd");
	//printf("%d",T->type);
	//printf("two is %d %d ", two[1][0], two[1][1]);
	//random_fenshu(end, T->fenshu);
	//printf("%d%d%d\n", T->fenshu->A,T->fenshu->B,T->fenshu->C);
	//reduce_fenshu(T->fenshu);
	if ((fp = fopen("result.txt", "w")) == NULL)
		printf("error open file !!");
	if ((fp_key = fopen("key.txt", "w")) == NULL)
		printf("error open file !!");
	if (setting_error == -1)
	{
		printf("error setting\n");
		return -1;
	}

	for (i = 1; i<question_number + 1; i++)
	{
		num_num = 3 + rand() % 3;
		if (num_num == 5)
		{
			x_Signaltype = rand() % 12;
			Put_Operator(four[x_Signaltype]);
		}
		else if (num_num == 4)
		{
			x_Signaltype = rand() % 5;
			Put_Operator(three[x_Signaltype]);
		}
		else if (num_num == 3)
		{
			x_Signaltype = rand() % 2;
			Put_Operator(two[x_Signaltype]);
		}
		Put_Number(T, Number_type, num_num, end);
		//preOrder(T,0);
		//printf("\n\n");
		result = Calculate(T);
		result = reduce_fenshu(result);

		if (Number_type[1] == 1)
		{
			result_float = (float)result.A + ((float)result.B) / ((float)result.C);
			//printf("\n result is %0.2f\n", result_float);
		}
		if (result_float >=20)
			wrong = 1;
		//else
		//rintf("\nfinal result is %d'%d/%d \n", result.A, result.B, result.C);
		//	printf("++++++%d",end);
		if (result.A>=end||result.C>=end)
			wrong=1;

		if (wrong == 1)
			i--;
		else
		{
			checkforbkt(T);
			if (bktornot == 0) {
				//fprintf(fp, "%d.\t", i);
				inOrder(T, fp);
				fprintf(fp, " =\n");
				if (Number_type[1] == 1)
				{

					fprintf(fp_key, "%0.2f\n", result_float);

				}
				else if (Number_type[2] == 1 && result.B != 0) {

					if(result.A!=0)
					    fprintf(fp_key, "%d'%d/%d\n", result.A, result.B, result.C);
					else
						fprintf(fp_key, "%d/%d\n",  result.B, result.C);

				}
				else
					fprintf(fp_key, "%d\n", result.A);
			}
			else
				i--;
		}
		wrong = 0; bktornot = 0;
		Empty(T);
	}




	fclose(fp);
	fclose(fp_key);
	printf("success!\n");
	return 1;
}





