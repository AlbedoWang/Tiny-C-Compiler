#define _CRT_SECURE_NO_WARNINGS
#include "convert_lex.h"

char TokenText[256];

Token token;
FILE* sFile;

TokenList list ;

int isLetter(char ch)
{
	if (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z')
	{
		return 1;
	}
	return 0;
}
int isDigit(char ch)
{
	if (ch >= '0' && ch <= '9')
	{
		return 1;
	}
	return 0;
}
int cmpstring(char a[], char b[])
{
	int la = strlen(a);
	int lb = strlen(b);
	if (la != lb)
		return 0;
	for (size_t i = 0; i <= la; i++)
	{
		if (a[i] != b[i])
			return 0;
	}
	return 1;
}
dfastat initToken(char ch)
{
	
	if (strlen(TokenText) > 0)
	{
		
		TokenList tokenNode = (TokenList)malloc(sizeof(Token));
		tokenNode->type = token.type;
		strcpy(tokenNode->text, TokenText);
		tokenNode->next = NULL;
		tokenNode->parent = NULL;
		
		if (list == NULL)
		{
			list = tokenNode;
		}
		else
		{
			TokenList p = list;
			while (p->next!=NULL)
			{
				p = p->next;
			}
			p->next = tokenNode;
			tokenNode->parent = p;
		}
		memset(TokenText, '\0',256);
		memset(token.text, '\0', 256);
	}
	dfastat newstat = InitialStat;
	if (isLetter(ch))
	{
		newstat = LetterStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = Identifier;
	}
	else if (isDigit(ch))
	{
		newstat = DigitStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = IntLiteral;
	}
	else if (ch == '=')
	{
		newstat = AssignStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = Assignment;
	}
	else if (ch == '<')
	{
		newstat = LTStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = LT;
	} 
	else if (ch == '>')
	{
		newstat = GTStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = GT;
	} 
	else if (ch == '&')
	{
		newstat = AndStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = And;
	}
	else if (ch == '|')
	{
		newstat = OrStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = Or;
	}
	else if (ch == '+')
	{
		newstat = PlusStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = Plus;
	}
	else if (ch == '-')
	{
		newstat = SubStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = Sub;
	}
	else if (ch == '*')
	{
		newstat = MultiStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = Multip;
	}
	else if (ch == '/')
	{
		newstat = DivStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = Div;
	}
	else if (ch == '(')
	{
		newstat = LeftParenStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = LeftParen;
	}
	else if (ch == '{')
	{
		newstat = LeftBraceStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = LeftBrace;
	}
	else if (ch == ')')
	{
		newstat = RightParenStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = RightParen;
	}
	else if (ch == '}')
	{
		newstat = RightBraceStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = RightBrace;
	}
	else if (ch == ';')
	{
		newstat = SemiColonStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = SemiColon;
	}
    else if (ch == ',')
	{
		newstat = DotStat;
		TokenText[strlen(TokenText) ] = ch;
		token.type = Dot;
	}
	return newstat;
}

void TokenReader()
{
	memset(TokenText, '\0', 256);
	char c;
	dfastat stat = InitialStat;
	while (!feof(sFile))
	{
		c = fgetc(sFile);
		
		switch (stat)
		{

		case InitialStat:
			stat = initToken(c);
			break;
		case LetterStat:
			if (isDigit(c) || isLetter(c))
			{
				stat = LetterStat;
				TokenText[strlen(TokenText)] = c;
			}
			else
			{
				if (cmpstring(TokenText, "main"))
				{
					token.type = Main;
				}	
				else if (cmpstring(TokenText, "if"))
				{
					token.type = If;
				}
				else if (cmpstring(TokenText, "else"))
				{
					token.type = Else;
				}
				else if (cmpstring(TokenText, "int"))
				{
					token.type = Int;
				}
				else if (cmpstring(TokenText, "char"))
				{
					token.type = Char;
				}
				else if (cmpstring(TokenText, "show"))
				{
					token.type = Show;
				}
                else if (cmpstring(TokenText, "goto"))
				{
					token.type = GOTO;
				}
                else if (cmpstring(TokenText, "return"))
				{
					token.type = Return;
				}
				else if (cmpstring(TokenText, "while"))
				{
					token.type = While;
				}
				else
				{
					token.type = Identifier;
				}
				stat = initToken(c);
			}
			break;
		case DigitStat:
			if (isDigit(c)) 
			{
				stat = DigitStat;
				TokenText[strlen(TokenText)] = c;
			} 
			else 
			{
				token.type = IntLiteral; 
				stat = initToken(c);
			}
			break;
		case AssignStat:
			stat = initToken(c);
			break;
		case LTStat:
			token.type = LT;
			stat = initToken(c);
			break;
		case GTStat:
			token.type = GT;
			stat = initToken(c);
			break;
		case PlusStat:
			token.type = Plus;
			stat = initToken(c);
			break;
		case LeftParenStat:
			token.type = LeftParen;
			stat = initToken(c);
			break;
		case LeftBraceStat:
			token.type = LeftBrace;
			stat = initToken(c);
			break;
		case RightParenStat:
			token.type = RightParen;
			stat = initToken(c);
			break;
		case RightBraceStat:
			token.type = RightBrace;
			stat = initToken(c);
			break; 
		case SemiColonStat:
			token.type = SemiColon;
			stat = initToken(c);
			break; 
        case DotStat:
			token.type = Dot;
			stat = initToken(c);
			break; 
		case SubStat:
			token.type = Sub;
			stat = initToken(c);
			break;
		case MultiStat:
			token.type = Multip;
			stat = initToken(c);
			break;
		case DivStat:
			token.type = Div;
			stat = initToken(c);
			break;
		case EqualStat:
			if (c=='=')
			{
				token.type = Equal;
				TokenText[strlen(TokenText) ] = c;
				stat = EqualStat;
			}
			else if(strlen(TokenText)==2)
			{
				token.type = Equal;
				stat = initToken(c);
			}
			else if(strlen(TokenText)==1)
			{
				token.type = Assignment;
				stat = initToken(c);
			}
			break;
		case AndStat:
			if (c=='&')
			{
				token.type = And;
				TokenText[strlen(TokenText) ] = c;
				stat = AndStat;
			}
			else
			{
				stat = initToken(c);
			}
			break;
		case OrStat:
			if (c=='|')
			{
				token.type = Or;
				TokenText[strlen(TokenText) ] = c;
				stat = OrStat;
			}
			else
			{
				stat = initToken(c);
			}
			break;
		}
	}
	if (strlen(TokenText) > 0)
	{
		initToken(c);
	}
}