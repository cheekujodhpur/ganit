/* File : dsl.h																		 **
** Project : openMatica																 **
** Description : Data structure library												 **
** Author : Kumar Ayush; Mukund Kothari												 */

#ifndef DSL_H
#define DSL_H

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <misc.h>
using namespace std;

//Stack
template <class T>
class sElement
{
	T data;
	sElement<T>*lptr;	//pointer to next(lower) element in stack
public:
	sElement();
	void setData(T);
	T getData();
	void setPtr(sElement<T>*);
	sElement<T>* getPtr();
};

template <class T>
class Stack
{
	int count;
	sElement<T>* cptr;
public:
	Stack();
	void push(T);
	T pop();
	void fire(char*);
	void error(int);
	int size();
	char*tostring();
};

//Class definitions for data structure stack
template <class T>
sElement<T>::sElement()
{
	lptr = 0;
}

template <class T>
void sElement<T>::setData(T val)
{
	data = val;
}

template <class T>
T sElement<T>::getData()
{
	return data;
}

template <class T>
void sElement<T>::setPtr(sElement<T>* val)
{
	lptr = val;
}

template <class T>
sElement<T>* sElement<T>::getPtr()
{
	return lptr;
}

template <class T>
Stack<T>::Stack()
{
	count = 0;
	cptr = 0;
}

template <class T>
T Stack<T>::pop()
{
	if(count == 0)
	{
		error(1);
		return 0;
	}
	else
	{
		T data = (*cptr).getData();
		cptr = (*cptr).getPtr();
		count--;
		return data;
	}
}

template <class T>
void Stack<T>::push(T val)
{
	sElement<T> *dat;
	dat = new sElement<T>;
	(*dat).setData(val);
	(*dat).setPtr(cptr);
	cptr = dat;
	count++;
}

template <class T>
void Stack<T>::error(int code)
{
	switch(code)
	{
	case 1:
		cout << "Underflow Error! -> ";
		break;
	case 2:
		cout << "Invalid operation!";
		break;
	case 3:
		cout << "Division by Zero!";
	}
}

template <class T>
int Stack<T>::size()
{
	return count;
}

template <class T>
char* Stack<T>::tostring()
{
	char * p;
	p = new char[count+1];
	p[count] = '\0';	//the null terminator
	for(int i = count-1;i>=0;i--)
	{
		p[i] = this->pop();
	}
	return p;
}

void Stack<float>::fire(char*in)
{
	if(charsize(in)==0)
		return;
	float op1,op2;
	if(!allnums(in))
	{
		op2 = this->pop();
		op1 = this->pop();
		if(in[0]=='+')
			this->push(op1+op2);
		else if(in[0]=='-')
			this->push(op1-op2);
		else if(in[0]=='*')
			this->push(op1*op2);
		else if(in[0]=='/')
			this->push(op1/op2);
		else if(in[0]=='^')
			this->push(pow(op1,op2));
	}
	else
	{
		this->push(tofloat(in));
	}
}

//Function
class Function
{
	char* name;
	char* variable;
	char* expression;
public:
	Function():name(0),variable(0),expression(0){};
	Function(char* n,char* v,char* e);	//constructors
	char* getName();
	void setName(char*);
	char *getVar();
	void setVar(char*);
	char *getExp();
	void setExp(char*);
	bool fixExp();	//converts the expression string to usable form
};

Function::Function(char*n,char*v,char*e)
{
	name = n;
	variable = v;
	expression = e;
}

char*Function::getName()
{
	return name;
}

void Function::setName(char*in)
{
	name = in;
}

char*Function::getVar()
{
	return variable;
}

void Function::setVar(char*in)
{
	variable = in;
}

char*Function::getExp()
{
	return expression;
}

void Function::setExp(char*in)
{
	expression = in;
}

bool Function::fixExp()
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
	expression = tmpstr.tostring();
	return 1;
}

//Variables
class Variable
{
	char*name;
	char* value;
public:
	Variable():name(0),value(0){}
	Variable(char* n,char* inp);
	char*getName();
	void setName(char*);
	char* getVal();
	void setVal(char*);
};

Variable::Variable(char*n,char* inp)
{
	name = n;
	value = inp;
}

char* Variable::getName()
{
	return name;
}

void Variable::setName(char*input)
{
	name = input;
}

char* Variable::getVal()
{
	return value;
}

void Variable::setVal(char* input)
{
	value = input;
}

//List
template <class T>
class lElement
{
	T data;
	lElement<T> *nptr;
	lElement<T> *pptr;
public:
	lElement();
	void setData(T);
	void setNPtr(lElement<T>*);
	void setPPtr(lElement<T>*);
	T getData();
	lElement<T>* getNPtr();
	lElement<T>* getPPtr();
};

template <class T>
class List
{
	lElement<T> *iptr;
	lElement<T> *cptr;
	int count;
public:
	List();
	void append(T);
	bool inList(T);
	bool FinList(char*name);
	bool FinListandReplace(Function);
	bool VinListandReplace(Variable);
	bool FinListandDisplay(char*name);
	char* FinListandGetExp(char*name);
	char* FinListandGetVar(char*name);
	map<char*,char*,cmp_str> generateMap();
	bool remove(T);
	void print();
	int size();
};

template <class T>
lElement<T>::lElement()
{
	nptr = NULL;
	pptr = NULL;
}

template <class T>
void lElement<T>::setData(T input)
{
	data = input;
}

template <class T>
void lElement<T>::setNPtr(lElement<T>*ptr)
{
	nptr = ptr;
}

template <class T>
void lElement<T>::setPPtr(lElement<T>*ptr)
{
	pptr = ptr;
}

template <class T>
T lElement<T>::getData()
{
	return data;
}

template <class T>
lElement<T>* lElement<T>::getNPtr()
{
	return nptr;
}

template <class T>
lElement<T>* lElement<T>::getPPtr()
{
	return pptr;
}

template <class T>
List<T>::List()
{
	count = 0;
	iptr = NULL;
}

template <class T>
void List<T>::append(T input)
{
	if(count==0)
	{
		lElement<T>*f;
		f = new lElement<T>;
		(*f).setData(input);
		iptr = f;
		cptr = iptr;
		count++;
	}
	else
	{
		lElement<T>*f;
		f = new lElement<T>;
		(*f).setData(input);
		(*f).setPPtr(cptr);
		(*cptr).setNPtr(f);
		cptr = f;
		count++;
	}
}

template <class T>
void List<T>::print()
{
	int i = 0;
	lElement<T>*iter = iptr;
	printf("[");
	while(i<count)
	{
		printf("%d,",(*iter).getData());
		iter = (*iter).getNPtr();
		i++;
	}
	printf("\b]\n");
}

template <class T>
bool List<T>::inList(T n)
{
	int i = 0;
	lElement<T>*iter = iptr;
	while(i<count)
	{
		if((*iter).getData()==n)
			return 1;
		iter = (*iter).getNPtr();
		i++;
	}
	return 0;
}

bool List<Function>::FinList(char*name)//checks if the function already exists or not
{
	int i = 0;
	lElement<Function>*iter = iptr;
	while(i<count)
	{
		if(!strcmp((*iter).getData().getName(),name))
			return 1;
		iter = (*iter).getNPtr();
		i++;
	}
	return 0;
}

bool List<Function>::FinListandDisplay(char*name)
{
	int i = 0;
	lElement<Function>*iter = iptr;
	while(i<count)
	{
		if(!strcmp((*iter).getData().getName(),name))
		{
			cout << (iter->getData()).getName() 
				<< "(" << (iter->getData()).getVar()
				<< ") = " << (iter->getData()).getExp()
				<<"\n";
			return 1;
		}
		iter = (*iter).getNPtr();
		i++;
	}
	return 0;
}

bool List<Function>::FinListandReplace(Function f)
{
	int i = 0;
	lElement<Function>*iter = iptr;
	while(i<count)
	{
		if(!strcmp((*iter).getData().getName(),f.getName()))
		{
			Function temp(f.getName(),f.getVar(),f.getExp());
			iter->setData(temp);
			return 1;
		}
		iter = (*iter).getNPtr();
		i++;
	}
	return 0;
}

bool List<Variable>::VinListandReplace(Variable f)
{
	int i = 0;
	lElement<Variable>*iter = iptr;
	while(i<count)
	{
		if(!strcmp((*iter).getData().getName(),f.getName()))
		{
			Variable temp(f.getName(),f.getVal());
			iter->setData(temp);
			return 1;
		}
		iter = iter->getNPtr();
		i++;
	}
	return 0;
}

char*List<Function>::FinListandGetExp(char*name)
{
	char*result=NULL;
	int i = 0;
	lElement<Function>*iter = iptr;
	while(i<count)
	{
		if(!strcmp((*iter).getData().getName(),name))
		{
			result = (iter->getData()).getExp();
			return result;
		}
		iter = (*iter).getNPtr();
		i++;
	}
	return result;
}

char*List<Function>::FinListandGetVar(char*name)
{
	char*result=NULL;
	int i = 0;
	lElement<Function>*iter = iptr;
	while(i<count)
	{
		if(!strcmp((*iter).getData().getName(),name))
		{
			result = (iter->getData()).getVar();
			return result;
		}
		iter = (*iter).getNPtr();
		i++;
	}
	return result;
}

template <class T>
bool List<T>::remove(T n)
{
	int i = 0;
	lElement<T>*iter = iptr;
	while(i<count)
	{
		if((*iter).getData()==n)
		{
			(*((*iter).getPPtr())).setNPtr((*iter).getNPtr());
			if(iter==cptr)
				cptr = (*iter).getPPtr();	//failsafe if last element deleted
			count--;	//required by rest of the functions
			return 1;
		}
		iter = (*iter).getNPtr();
		i++;
	}
	return 0;
}

template <class T>
int List<T>::size()
{
	return count;
}

map<char*,char*,cmp_str> List<Variable>::generateMap()
{
	map<char*,char*,cmp_str> Varbs;
	int i = 0;
	lElement<Variable>*iter = iptr;
	while(i<count)
	{
		Varbs[(iter->getData()).getName()] = (iter->getData()).getVal();
		iter = (*iter).getNPtr();
		i++;
	}
	return Varbs;
}

#endif