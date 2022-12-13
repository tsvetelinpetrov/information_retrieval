
// Lab 4

#include <stdio.h>

#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cctype>
#include <iostream>

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

char Char;

char Spelling[8];
int Constant;
const int MAXINTEGER = 1000000;

char fileData[10000];
int i = 0;
bool errors = false;

typedef enum TSymbolType
{
	ident, plusop,
	string, intconst, text,
	semicolon, period, quotas, othersy
}TSymbol;

TSymbol Symbol;


void DataFile(void);
char* readFile();
void GetNextSymbol();
//Scanner
void GetNextChar()
{
	Char = fileData[i];
	printf("%c", Char);
	if (Char == '.')
		printf("\n");
	i++;
	return;
}

void GetNextSymbol()
{
	int digit = 0;
	int k = 0;

	while (Char == ' ' || Char == '\n')
	{
		GetNextChar();
	}
	switch (toupper(Char))
	{
	case 65:case 66:case 67:
	case 68:
	case 69:
	case 70:
	case 71:
	case 72:
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 79:
	case 80:
	case 81:
	case 82:
	case 83:
	case 84:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 90:
	{
		strcpy(Spelling, "		");
		do
		{
			if (k < 8)
			{
				Spelling[k] = toupper(Char);
				k++;
			}
			GetNextChar();
		} while ((Char >= 48) && (Char <= 57) || (toupper(Char) >= 65) && (toupper(Char) <= 90));
		Symbol = text;
		break;
	}
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	{
		Constant = 0;
		do
		{
			digit = (unsigned int)(Char)-(unsigned int)(48);
			if ((Constant < (MAXINTEGER / 10)) || ((Constant == (MAXINTEGER / 10))
				&& (digit = (MAXINTEGER % 10))))
			{
				Constant = (10 * Constant) + digit;

			}
			else
			{
				printf(" <- error: Int constant too large");
				Constant = 0;
			}GetNextChar();
		} while ((Char >= 48) && (Char <= 57));
		Symbol = intconst;
	}break;
	case 46:
	{
		Symbol = period;
		GetNextChar();
	}break;
	case 59:
	{
		Symbol = semicolon;
		GetNextChar();
	}break;
	case '"':
	{
		Symbol = quotas;
		GetNextChar();
	}break;
	default:
	{
		Symbol = othersy;
		GetNextChar();
	}break;
	}
}
int accept(TSymbol sym)
{
	if (Symbol == sym)
	{
		GetNextSymbol();
		return 1;
	}
	return 0;
}
int expect(TSymbol sym)
{
	if (accept(sym))
		return 1;

	errors = true;
	perror(" <- expect: unexpected symbol");

	return 0;
}
void Field()
{
	if (accept(intconst))
	{
		;
	}
	else if (accept(quotas))
	{
		expect(text);
		expect(quotas);
	}
	else
	{
		errors = true;
		perror(" <- field: expects intconst or string");
	}
}
void Record()
{
	Field();
	while ((Symbol == semicolon))
	{
		accept(semicolon);
		Field();
	}
	expect(period);
}

int records = 0;
void DataFile(void)
{
	if(records == 0)
		GetNextSymbol();
	Record();
	records++;
	while ((Symbol == intconst) || (Symbol == quotas))
	{
		//GetNextSymbol();
		Record();
	}
}

char* readFile()
{
	FILE* in = fopen("file.txt", "r");
	char string[5000] = "\n";
	int i = 0;
	while (1)
	{
		if (fileData[i] == EOF)
		{
			break;
		}
		fscanf(in, "%c", &fileData[i]);
		i++;

	}

	return string;
}

int main()
{
	readFile();
	GetNextChar();
	DataFile();

	if (!errors) {
		printf("%s", "\n\nThe string is valid!\n");
	}

	return 0;
}

/* file.txt */
// 1234;631;"H2ell";5678;"course1";"end".