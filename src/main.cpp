/* File : main.cpp																	 **
** Project : openMatica																 **
** Description : Has 'main' function which holds the flow of control for the program **
** Author : Kumar Ayush; Mukund Kothari												 */

//Header Files
#include <iostream>
#include <dsl.h>
#include <mfunc.h>
#include <misc.h>
#include <vars.h>
#include <regex>
using namespace std;

int main()
{
	//variables needed to take user input and maintain the program flow
	char in_c;
	Stack<char> input_string;
	char *input;
	List<Function> Functions;
	List<Variable> Variables;
	CreateFile<Function>(Functions,"fun.tmp");
	CreateFile<Variable>(Variables,"var.tmp");

	//the following loop maintains the program
	while(1)
	{
		cout << "-> ";
		while(cin.peek()!=';')
		{
			in_c = cin.get();
			if(in_c!=10 && in_c!=13)
				input_string.push(in_c);
		}
		while(cin.get()!=10);
		input = input_string.tostring();	//empties stack
		if(!strcmp(input,P_EXIT))
			break;
		if(!strcmp(input,P_HELP))
			cout << STR_HELP;
		else if(!strcmp(input,P_LISTCMD))
			cout << STR_LISTCMD;
		else if(regex_match(input,REG_FUNC))
		{
			int func_status = declare_function(input);
			if(func_status==1)
				cout << STR_FUNC_CREATE_SUCCESS;
			else if(func_status==0)
				cout << STR_FUNC_CREATE_FAIL;
			else if(func_status==2)
				cout << STR_FUNC_CREATE_FAIL_1;
		}
		else if(regex_match(input,REG_FUNC_HELP))
			cout << STR_FUNC_HELP;
		else if(regex_match(input,REG_FUNC_DISPLAY))
		{
			int dis_status = display_function(input);
			if(dis_status==0)
				cout << STR_FUNC_DISPLAY_FAIL;
			else if(dis_status==2)
				cout << STR_FUNC_DISPLAY_FAIL_1;
		}
		else if(regex_match(input,REG_FUNC_DISPLAY_HELP))
			cout << STR_FUNC_DISPLAY_HELP;
		else if(regex_match(input,REG_FUNC_EVAL))
		{
			int eval_status = evaluate_function(input);
			if(eval_status==0)
				cout << STR_FUNC_EVAL_FAIL;
			else if(eval_status==2)
				cout << STR_FUNC_EVAL_FAIL_1;
		}
		else if(regex_match(input,REG_FUNC_EVAL_HELP))
			cout << STR_FUNC_EVAL_HELP;
		else if(regex_match(input,REG_POLISH))
			cout << showPolish(input) << endl;
		else if(regex_match(input,REG_VAR_DEF))
		{
			int vardecstat = defineVariable(input);
			if(vardecstat==1)
				cout << STR_VAR_CREATE_SUCCESS;
			else if(vardecstat==0)
				cout << STR_VAR_CREATE_FAIL;
			else if(vardecstat==2)
				cout << STR_VAR_CREATE_FAIL_1;
			else if(vardecstat==3)
				cout << STR_VAR_CREATE_FAIL_2;
		}
		else
			cout << STR_INVALID;
	}
	cleanUp();
	cout << "Bye";
	return 0;
}