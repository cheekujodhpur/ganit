/* File : mfunc.h																	 **
** Project : openMatica																 **
** Description : Key Functions														 **
** Author : Kumar Ayush; Mukund Kothari												 */

#ifndef MFUNC_H
#define MFUNC_H

#include <misc.h>
#include <string>
#include <dsl.h>
#include <regex>
#include <fstream>
#include <vars.h>
using namespace std;

//declarations and prototypes
int declare_function(char*input);
int display_function(char*input);
int evaluate_function(char*input);
int precedence(char in);
string toPolish(char*);
string *toPolish_inString(char*);
float toPolishandEval(char*,map<char*,char*,cmp_str>);
string showPolish(char*);
char* fixExpression(char*input);
void cleanUp();	//clean up variables at end of program
int defineVariable(char*);
string* simplifyPolish(string*,int,int&,bool&);
string* reducePolish(string*,int&);
void Calculator(char*);

//definitions

void Calculator(char* input)
{
	fstream file;

	file.open("var.tmp",ios::in|ios::binary); //open for reading
	if(!file)
		cout << "Crash!!!" << endl;
	List<Variable>Variables;
	file.read((char*)&Variables,sizeof(Variables));
	file.close();

	//Need to generate a variable map
	map<char*,char*,cmp_str> Varbs = Variables.generateMap();
	input = stochar(substr(chartos(input),0,charsize(input)-2));
	input = fixExpression(input);	//fix for use with next function
	float res =  toPolishandEval(input,Varbs);
	
	cout << res << endl;

	//Add result to variable '_'
	Variable temp("_",stochar(to_string(res)));
	if(!Variables.VinListandReplace(temp))
		Variables.append(temp);
	
	file.open("var.tmp",ios::out|ios::binary);
	if(!file)
		cout << "Crash!!!" << endl;
	file.write((char*)&Variables,sizeof(Variables));
	file.close();
}

int precedence(char in)
{
	if(in=='-')
		return 0;
	else if(in=='+')
		return 0;
	else if(in=='*')
		return 1;
	else if(in=='/')
		return 1;
	else if(in=='^')
		return 2;
}

char *fixExpression(char*expression)
{
	Stack<char> tmpstr; //temporary string
	tmpstr.push('(');
	char lasttmp = '('; //last character pushed
	for(int i = 0;i<charsize(expression);i++)
	{
		if(expression[i]!=32)
		{
			//check where to insert '*'
			if((isalpha(lasttmp) && isdigit(expression[i]))||(isalpha(expression[i]) && isdigit(lasttmp)))
				tmpstr.push('*');
			if( (lasttmp!='(' && lasttmp!='+' && lasttmp!='-' && lasttmp!='*' && lasttmp!='/' && lasttmp!='^' && expression[i]=='(') || 
				(lasttmp==')'  && expression[i]!='+' && expression[i]!='-' && expression[i]!='*' && expression[i]!='/' && expression[i]!='^' && expression[i]!=')') )
				tmpstr.push('*');
			tmpstr.push(expression[i]);
			lasttmp = expression[i];
		}
	}
	tmpstr.push(')');	//brackets at start and end, yeah!
	if(tmpstr.size()==2)	//just brackets
		return 0;
	return tmpstr.tostring();
}

string* reducePolish(string* input, int &n)
{
	//n is size
	int iter = 1;
	Stack<string> stred;
	stred.push(input[0]);
	int size = n;
	while(iter<size)
	{
		if(input[iter]=="*")
		{
			char* a = stochar(stred.pop());
			char* b = stochar(stred.pop());
			if(allnums(a) &&  allnums(b))
			{
				stred.push(to_string(tofloat(a)*tofloat(b)));
			}
			else if(!allnums(a) &&  allnums(b))
			{
				if(tofloat(b)==1 && (a!="+" && a!="-" && a!="*" && a!="/" && a!="^"))
					stred.push(a);
				else
				{
					stred.push(chartos(b));
					stred.push(chartos(a));
					stred.push("*");
				}
			}
			else if(allnums(a) &&  !allnums(b))
			{
				if(tofloat(a)==1 && (b!="+" && b!="-" && b!="*" && b!="/" && b!="^"))
					stred.push(b);
				else
				{
					stred.push(chartos(b));
					stred.push(chartos(a));
					stred.push("*");
				}
			}
			else
			{
				stred.push(chartos(b));
				stred.push(chartos(a));
				stred.push("*");
			}
		}
		else
			stred.push(input[iter]);
		iter++;
	}
	n = stred.size();
	return stred.toStrArray();
}

string showPolish(char*input)
{
	string nullstr = "";
	smatch sm;
	string s = chartos(input);
	if(!regex_match(s,sm,REG_POLISH))
		return nullstr;

	int size = 0;
	string*out = toPolish_inString(fixExpression(stochar(sm[1])));
	//Get size of 'out'
	while(out[size]!="null")size++;
	
	bool flag = false;

	while(!flag)
		out = simplifyPolish(out,size,size,flag);
	out = reducePolish(out,size);
	string res = "";
	for(int i = 0;i<size;i++)
		res = res+out[i];
	return res;
}

string toPolish(char*input)//converts to polish string
{
	int numofterm = 1;
	for(int i = 0;i<charsize(input);i++)
		if(input[i]=='+' || input[i] =='-' || input[i] == '*' || input[i] =='/' || input[i] =='^' || input[i] == ')' || input[i] == '(')numofterm++;
	string * terms;
	numofterm = 2*numofterm-1;
	terms = new string[numofterm];
	for(int i = 0;i<numofterm;i++)
		terms[i] = "";
	int i = 0,j=0;
	while(i<numofterm)
	{
		while(input[j]!=0 && input[j]!='+' && input[j]!='-' && input[j]!='*' && input[j]!='/' && input[j]!='^' && input[j]!='(' && input[j]!=')')
		{
			terms[i] = terms[i]+input[j];
			j++;
		}
		if(j>=charsize(input))break;
		i++;
		terms[i] = terms[i] + input[j];
		j++;
		if(j>=charsize(input))break;
		i++;
	}
	Stack<string> pol1;
	string res = "";
	for(int i = 0;i<numofterm;i++)
	{
		if(terms[i]!="(" && terms[i]!=")" && terms[i]!="+" && terms[i]!="-" && terms[i]!="*" && terms[i]!="/" && terms[i]!="^")
			res = res+terms[i];
		else if(terms[i]=="(")
			pol1.push(terms[i]);
		else if(terms[i]=="+" || terms[i]=="-" || terms[i]=="*" || terms[i]=="/" || terms[i]=="^")
		{
			string tmpstr;
			while(pol1.size()>0)
			{
				tmpstr = pol1.pop();
				if(tmpstr=="+" || tmpstr=="-" || tmpstr=="*" || tmpstr=="/" || tmpstr=="^")
				{
					if(precedence(tmpstr[0])>=precedence(terms[i][0]))
					{
						res = res + tmpstr;
						continue;
					}
					else
					{
						pol1.push(tmpstr);
						break;
					}
				}
				else
				{
					pol1.push(tmpstr);
					break;
				}
			}
			pol1.push(terms[i]);
		}
		else if(terms[i]==")")
		{
			string tmpstr;
			while(pol1.size()>0)
			{
				tmpstr = pol1.pop();
				if(tmpstr=="(")
					break;
				if(tmpstr=="+" || tmpstr=="-" || tmpstr=="*" || tmpstr=="/" || tmpstr=="^")
					res = res + tmpstr;
			}
		}
	}
	return res;
}

string * toPolish_inString(char*input)//converts to polish string
{
	int numofterm = 1;
	for(int i = 0;i<charsize(input);i++)
		if(input[i]=='+' || input[i] =='-' || input[i] == '*' || input[i] =='/' || input[i] =='^' || input[i] == ')' || input[i] == '(')numofterm++;
	string * terms;
	numofterm = 2*numofterm-1;
	terms = new string[numofterm];
	for(int i = 0;i<numofterm;i++)
		terms[i] = "";
	int i = 0,j=0;
	while(i<numofterm)
	{
		while(input[j]!=0 && input[j]!='+' && input[j]!='-' && input[j]!='*' && input[j]!='/' && input[j]!='^' && input[j]!='(' && input[j]!=')')
		{
			terms[i] = terms[i]+input[j];
			j++;
		}
		if(j>=charsize(input))break;
		i++;
		terms[i] = terms[i] + input[j];
		j++;
		if(j>=charsize(input))break;
		i++;
	}
	Stack<string> pol1;
	Stack<string> res;
	for(int i = 0;i<numofterm;i++)
	{
		if(terms[i]!="(" && terms[i]!=")" && terms[i]!="+" && terms[i]!="-" && terms[i]!="*" && terms[i]!="/" && terms[i]!="^")
		{
			if(terms[i]!="")res.push(terms[i]);
		}
		else if(terms[i]=="(")
			pol1.push(terms[i]);
		else if(terms[i]=="+" || terms[i]=="-" || terms[i]=="*" || terms[i]=="/" || terms[i]=="^")
		{
			string tmpstr;
			while(pol1.size()>0)
			{
				tmpstr = pol1.pop();
				if(tmpstr=="+" || tmpstr=="-" || tmpstr=="*" || tmpstr=="/" || tmpstr=="^")
				{
					if(precedence(tmpstr[0])>=precedence(terms[i][0]))
					{
						if(tmpstr!="")
							res.push(tmpstr);
						continue;
					}
					else
					{
						pol1.push(tmpstr);
						break;
					}
				}
				else
				{
					pol1.push(tmpstr);
					break;
				}
			}
			pol1.push(terms[i]);
		}
		else if(terms[i]==")")
		{
			string tmpstr;
			while(pol1.size()>0)
			{
				tmpstr = pol1.pop();
				if(tmpstr=="(")
					break;
				if(tmpstr=="+" || tmpstr=="-" || tmpstr=="*" || tmpstr=="/" || tmpstr=="^")
					if(tmpstr!="")
						res.push(tmpstr);
			}
		}
	}
	res.push("null");
	return res.toStrArray();
}

float toPolishandEval(char*input,map<char*,char*,cmp_str> Varbs)
{
	Stack<float> result_stack;
	int numofterm = 1;
	for(int i = 0;i<charsize(input);i++)
		if(input[i]=='+' || input[i] =='-' || input[i] == '*' || input[i] =='/' || input[i] =='^' || input[i] == ')' || input[i] == '(')numofterm++;
	string * terms;
	numofterm = 2*numofterm-1;
	terms = new string[numofterm];
	for(int i = 0;i<numofterm;i++)
		terms[i] = "";
	int i = 0,j=0;
	while(i<numofterm)
	{
		while(input[j]!=0 && input[j]!='+' && input[j]!='-' && input[j]!='*' && input[j]!='/' && input[j]!='^' && input[j]!='(' && input[j]!=')')
		{
			terms[i] = terms[i]+input[j];
			j++;
		}
		if(j>=charsize(input))break;
		i++;
		terms[i] = terms[i] + input[j];
		j++;
		if(j>=charsize(input))break;
		i++;
	}
	Stack<string> pol1;
	for(int i = 0;i<numofterm;i++)
	{
		if(terms[i]!="(" && terms[i]!=")" && terms[i]!="+" && terms[i]!="-" && terms[i]!="*" && terms[i]!="/" && terms[i]!="^")
		{
			if(allnums(stochar(terms[i])))
				result_stack.fire(stochar(terms[i]));
			else
				result_stack.fire(Varbs[stochar(terms[i])]);
		}
		else if(terms[i]=="(")
			pol1.push(terms[i]);
		else if(terms[i]=="+" || terms[i]=="-" || terms[i]=="*" || terms[i]=="/" || terms[i]=="^")
		{
			string tmpstr;
			while(pol1.size()>0)
			{
				tmpstr = pol1.pop();
				if(tmpstr=="+" || tmpstr=="-" || tmpstr=="*" || tmpstr=="/" || tmpstr=="^")
				{
					if(precedence(tmpstr[0])>=precedence(terms[i][0]))
					{
						{
							result_stack.fire(stochar(tmpstr));
						}
						continue;
					}
					else
					{
						pol1.push(tmpstr);
						break;
					}
				}
				else
				{
					pol1.push(tmpstr);
					break;
				}
			}
			pol1.push(terms[i]);
		}
		else if(terms[i]==")")
		{
			string tmpstr;
			while(pol1.size()>0)
			{
				tmpstr = pol1.pop();
				if(tmpstr=="(")
					break;
				if(tmpstr=="+" || tmpstr=="-" || tmpstr=="*" || tmpstr=="/" || tmpstr=="^")
				{
					result_stack.fire(stochar(tmpstr));
				}
			}
		}
	}
	return result_stack.pop();
}

int evaluate_function(char*input)
{
	ifstream file;
	file.open("fun.tmp",ios::in|ios::binary); //open for reading
	if(!file)
		cout << "Crash!!!" << endl;
	List<Function>Functions;
	file.read((char*)&Functions,sizeof(Functions));
	file.close();
	
	smatch sm;
	string s = chartos(input);
	if(!regex_match(s,sm,REG_FUNC_EVAL))
		return 0;

	char*variable = Functions.FinListandGetVar(stochar(sm[1]));
	if(!variable)
		return 2;

	file.open("var.tmp",ios::in|ios::binary); //open for reading
	if(!file)
		cout << "Crash!!!" << endl;
	List<Variable>Variables;
	file.read((char*)&Variables,sizeof(Variables));
	file.close();

	//Need to generate a variable map
	map<char*,char*,cmp_str> Varbs = Variables.generateMap();
	Varbs[variable] = stochar(sm[2]);

	char*expressionstr = Functions.FinListandGetExp(stochar(sm[1]));
	if(expressionstr)
	{
		float res =  toPolishandEval(expressionstr,Varbs);
		cout << res << endl;
	}
	return 1;
}

int declare_function(char*input)
{
	ifstream file;
	file.open("fun.tmp",ios::in|ios::binary); //open for reading
	if(!file)
		cout << "Crash!!!" << endl;
	List<Function>Functions;
	file.read((char*)&Functions,sizeof(Functions));
	file.close();
	
	smatch sm;
	string s = chartos(input);
	if(!regex_match(s,sm,REG_FUNC))
		return 0;

	Function temp(stochar(sm[1]),stochar(sm[2]),stochar(sm[3]));
	if(!temp.fixExp())	//converts the expression of the function to usable form
		return 2;
	if(!Functions.FinListandReplace(temp))
		Functions.append(temp);

	ofstream fileout;
	fileout.open("fun.tmp",ios::out|ios::binary); //open for writing
	if(!fileout)
		cout << "Crash!!!" << endl;
	fileout.write((char*)&Functions,sizeof(Functions));
	fileout.close();
	return 1;
}

int display_function(char*input)
{
	ifstream file;
	file.open("fun.tmp",ios::in|ios::binary); //open for reading
	if(!file)
		cout << "Crash!!!" << endl;
	List<Function>Functions;
	file.read((char*)&Functions,sizeof(Functions));
	file.close();
	
	smatch sm;
	string s = chartos(input);
	if(!regex_match(s,sm,REG_FUNC_DISPLAY))
		return 0;

	if(!Functions.FinListandDisplay(stochar(sm[1])))
		return 2;
	return 1;
}

template <class T>
void CreateFile(List<T>data, char* fname)
{
	ofstream file;
	file.open(fname,ios::out|ios::binary); //create if doesn't exist
	file.write((char*)&data,sizeof(data));
	file.close();
}

int defineVariable(char* input)
{
	smatch sm;
	string s = chartos(input);
	if(!regex_match(s,sm,REG_VAR_DEF))
	{
		cout << "Invalid input!" << endl;
		return 0;
	}

	if(!allnums(stochar(sm[2])))
	{
		cout << "Invalid input!" << endl;
		return 2;
	}

	fstream file;
	file.open("var.tmp",ios::in|ios::binary); //open for reading
	if(!file)
		cout << "Crash!!!" << endl;
	List<Variable>Variables;
	file.read((char*)&Variables,sizeof(Variables));
	file.close();

	//Error if variable is a number
	if(allnums(stochar(sm[1])))
		return 3;
	Variable temp(stochar(sm[1]),stochar(sm[2]));
	if(!Variables.VinListandReplace(temp))
		Variables.append(temp);
	
	file.open("var.tmp",ios::out|ios::binary);
	if(!file)
		cout << "Crash!!!" << endl;
	file.write((char*)&Variables,sizeof(Variables));
	file.close();

	return 1;
}

string*simplifyPolish(string * exp,int n,int &size,bool &statusflag)
{
	int insertToOutput(string*,string*,int,int,int);

	int operand_ctr=0,
		operator_ctr=0,
		i=n-1,
		pos[6],
		posctr=0,
		expctr[3],
		ctr=0,
		l=0,
		stop=0,
		flag=0;

    size = n;
  
	string*ar=exp,
        *temp;

    while(i>0)
    {
        if(ar[i]=="*" && (ar[i-1]=="+" || ar[i-1]=="-"))
		{
            posctr=0;
            pos[posctr++]=i-2;

            operand_ctr=0;
            operator_ctr=0;

            ctr=0;
            flag=0;
            for(int iter=0;iter<3;iter++)
			{
				expctr[iter]=0;
			}

            for(int j=i-2;j>=0;j--)
            {
                expctr[ctr]++;

                if(ar[j]=="+"||ar[j]=="-"||ar[j]=="*")
                    operator_ctr++;
                else
                    operand_ctr++;
                if(operand_ctr==operator_ctr+1)
                {
                      flag++;

                      operand_ctr=0;
                      operator_ctr=0;

                      pos[posctr++]=j;

                      if(flag!=3)
                        pos[posctr++]=j-1;

                      ctr++;
                }
                if(flag==3)
                    break;
            }
            temp=ar;
            ar=new string[size+expctr[2]+1];
            if(pos[5])
                stop=insertToOutput(ar,temp,0,0,pos[5]-1);

            stop=insertToOutput(ar,temp,stop,pos[0],pos[1]);
            stop=insertToOutput(ar,temp,stop,pos[4],pos[5]);

            ar[stop++]="*";

            stop=insertToOutput(ar,temp,stop,pos[2],pos[3]);
            stop=insertToOutput(ar,temp,stop,pos[4],pos[5]);

            ar[stop++]="*";

            ar[stop++]="+";

            if(pos[0]+2!=size-1)
                stop=insertToOutput(ar,temp,stop,pos[0]+3,size-1);

            temp=0;
            size += expctr[2] + 1;
            return ar;
        }
        i--;
    }
	statusflag = true;
	return ar;
}

int insertToOutput(string*ar,string*temp,int start,int _end,int _beg)
{
	int beg;
	int end;
	beg = _end>_beg?_beg:_end;
	end = _end>_beg?_end:_beg;
	int i=start,j=beg;

	if(j!=-1)
	{
		for(i=start,j=beg;j<=end;j++,i++)
		{
			ar[i]=temp[j];
		}
		return i;
	}
	return 0;

}

void cleanUp()
{
	remove("fun.tmp");
	remove("var.tmp");
}

#endif