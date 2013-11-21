/* File : misc.h																	 **
** Project : openMatica																 **
** Description : Miscellaneous Functions											 **
** Author : Kumar Ayush; Mukund Kothari												 */

#ifndef MISC_H
#define MISC_H

#include <string>
#include <map>
#include <utility>
using namespace std;

bool allnums(char *in);
float tofloat(char *in);
int charsize(char*in);
int charsize(char*in,char delim);
int strsize(string in);
string chartos(char*in);
char* stochar(string in);
string substr(string,int,int);

string substr(string input,int m,int n)
{
	string x = "";
	for(int i = m;i<=n;i++)
		x = x+input[i];
	return x;
}

int charsize(char*in)
{
	int res = 0;
	while(in[res]!=0)res++;
	return res;
}

int charsize(char*in,char delim)
{
	int res = 0;
	while(in[res]!='.' && in[res]!=0)res++;
	return res;
}

int strsize(string in)
{
	int res = 0;
	while(in[res]!=0)res++;
	return res;
}


bool allnums(char *in)
{
	for(int i = 0;i<charsize(in);i++)
		if(!isdigit(in[i]))
			if(in[i]!='.')
			{
				if(i==0 && (in[i]=='+' || in[i]=='-'))
				{
					if(charsize(in)==1)
						return 0;
					else
						continue;
				}
				else
					return 0;
			}
	return 1;
}

float tofloat(char *in)
{
	float res = 0,tmpres = 0;
	if(in[0]=='+' || in[0]=='-')
	{
		char *newin = new char[charsize(in)];
		for(int i = 1;i<=charsize(in);i++)
			newin[i-1] = in[i];

		int upto_dec_len = charsize(newin,'.');
		for(int i = 0;i<upto_dec_len;i++)
			res += ((int)newin[i]-48)*pow(10.0,upto_dec_len-i-1);

		//now the decimal thingy
		int full_len = charsize(newin);
		int after_dec = full_len-upto_dec_len-1;
		for(int i = upto_dec_len+1;i<full_len;i++)
			tmpres += ((int)newin[i]-48)*pow(10.0,full_len-i-1);

		res += tmpres*pow(10.0,-after_dec);
		if(in[0]=='+')
			return +res;
		else
			return -res;
	}
	else
	{
		int upto_dec_len = charsize(in,'.');
		for(int i = 0;i<upto_dec_len;i++)
			res += ((int)in[i]-48)*pow(10.0,upto_dec_len-i-1);

		//now the decimal thingy
		int full_len = charsize(in);
		int after_dec = full_len-upto_dec_len-1;
		for(int i = upto_dec_len+1;i<full_len;i++)
			tmpres += ((int)in[i]-48)*pow(10.0,full_len-i-1);

		res += tmpres*pow(10.0,-after_dec);
		return res;
	}
}

string chartos(char*in)
{
	string res = "";
	for(int i = 0;i<charsize(in);i++)
		res = res+in[i];
	return res;
}

char* stochar(string in)
{
	char*res;
	res = new char[strsize(in)+1];
	for(int i = 0;i<=strsize(in);i++)
		res[i] = in[i];
	return res;
}

struct cmp_str
{
	bool operator()(char const*a, char const*b)
	{
		return strcmp(a,b)<0;
	}
};

#endif