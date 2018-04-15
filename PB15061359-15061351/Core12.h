#pragma once
__declspec(dllexport) typedef struct  Parameter
{
	//该结构体用于设置用户要求的参数，传给Setting函数用于生成对应要求的题目
    int integer = 1;  //支持 整数 生成则为1，不支持则为0，默认为支持
	int decimal = 0;  //支持 小数 生成则为1，不支持则为0，默认为不支持
	int fraction = 0; //支持 分数 生成则为1，不支持则为0，默认为不支持
	int add = 1;      //支持 加法 符号生成则为1，不支持则为0，默认为支持
	int sub = 1;      //支持 减法 符号生成则为1，不支持则为0，默认为支持
	int multiply = 1; //支持 乘法 符号生成则为1，不支持则为0，默认为支持
	int division = 1; //支持 除法 符号生成则为1，不支持则为0，默认为支持
	int pow = 0;      //支持 乘方 符号生成则为1，不支持则为0，默认为不支持
	int bracktet = 1; //支持 括号 符号生成则为1，不支持则为0，默认为支持

}Parameter;

__declspec(dllexport) int Setting_Parameter(int question_num, int sup,Parameter argument);
//   该函数接受用户设定参数
//   question_num  对应生成的题目数量（>0）
//   sup 对应生成的数的上界（>=1）
//   argument 对应上述结构体

__declspec(dllexport) int Generate(int setting_error);
//   生成对应要求的题目
//   题目存于result.txt 答案存于key.txt
