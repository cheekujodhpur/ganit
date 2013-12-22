/* File : vars.h																	 **
** Project : openMatica																 **
** Description : Global Variables													 **
** Author : Kumar Ayush; Mukund Kothari												 */

#ifndef VARS_H
#define VARS_H

#include <regex>
using namespace std;
#define P_EXIT "exit"
#define P_HELP "help"
#define P_LISTCMD "listcmd"

#define WELCOME_MESSAGE "Hi! This is ganit v0.1, a mathematical computing software.\nCurrent functionalities include calculator and function/variable definitions and evaluation.\nWe appreciate your feedback on cheekujodhpur@gmail.com\n\nIf you need any help, you just need to type 'help;'\n"
#define PROMPT_MESSAGE "ganit>> "

#define STR_HELP "Help\n\nThis section is under construction. For further details,\nContact : \nKumar Ayush\ncheekujodhpur@gmail.com\n"
#define STR_INVALID "Invalid Command\nFor a list of commands, try <<listcmd>>\nFor more details on a command, type <<command help>>\nAll commands must end with a semi-colon\n"
#define STR_LISTCMD "List of commands\n\nhelp\nlistcmd\nFunction\nDisplay\nEvaluate\nVariable\nexit\n"
#define STR_FUNC_CREATE_SUCCESS "Function created successfully.\n"
#define STR_FUNC_CREATE_FAIL "Function creation failed.\nPlease check your syntax.\n"
#define STR_FUNC_CREATE_FAIL_1 "Function creation failed.\nInvalid Expression.\n"
#define STR_FUNC_HELP "Usage : <<Function name(variable)=expression;>>\nExample:Function f(x) = x+2;\nUse <<^>> to denote exponentiation.\n"
#define STR_FUNC_DISPLAY_HELP "Usage : <<Display name;>>\nExample:Display f;\n"
#define STR_FUNC_DISPLAY_FAIL "Function display failed.\nPlease check your syntax.\n"
#define STR_FUNC_DISPLAY_FAIL_1 "Function display failed.\nThe given function does not exist or is invalid.\n"
#define STR_FUNC_EVAL_FAIL "Function evaluation failed.\nPlease check your syntax.\n"
#define STR_FUNC_EVAL_HELP "Usage : <<Evaluate name(value);>>\nExample:Evaluate f(3);\n"
#define STR_FUNC_EVAL_FAIL_1 "Function evaluation failed.\nThe given function does not exist.\n"
#define STR_VAR_CREATE_SUCCESS "Variable defined successfully.\n"
#define STR_VAR_CREATE_FAIL "Invalid input!\n"
#define STR_VAR_CREATE_FAIL_1 "Not a numerical value. Invalid Input!\n"
#define STR_VAR_CREATE_FAIL_2 "Keyword can't be a number. Invalid Input!\n"

regex REG_FUNC ("Function\\s+(.+?)\\((.+?)\\)\\s*=\\s*(.*)");
regex REG_FUNC_HELP ("Function help");
regex REG_FUNC_DISPLAY_HELP ("Display help");
regex REG_FUNC_DISPLAY ("Display\\s+(.+?)");
regex REG_FUNC_EVAL ("Evaluate\\s+(.+?)\\((.+?)\\)");
regex REG_FUNC_EVAL_HELP ("Evaluate help");
regex REG_POLISH ("toPolish\\((.+?)\\)");
regex REG_VAR_DEF ("Variable\\s+(.+?)\\s*=\\s*(.*)");


#endif