#include<string.h>
#include<stdlib.h>
#include<stdio.h>
typedef enum dfaStat
{
	InitialStat,   	
	LetterStat,    	 
	DigitStat,     	 
	AssignStat,	   	 
	LTStat,        	
	GTStat,    		
	PlusStat,      	
	SubStat,	   	
	MultiStat,     	
	DivStat,       	
	LeftParenStat, 	
	LeftBraceStat, 	
	RightParenStat,	
	RightBraceStat,	
	SemiColonStat, 	
	DotStat,       	
	AndStat, 		
	OrStat,			
	EqualStat, 		
	BreakStat,      
}dfastat;
typedef enum TokenType
{
	Plus,       // +
	Sub,		// -
	Multip,     // *
	Div,		// /
	GT,         // >
	LT,         // <
	SemiColon,  // ;
	LeftParen,  // (
	RightParen, // )
	Assignment, // =
	LeftBrace,  // { 
	RightBrace, // }
	If,         // if
	Else,       // else
	Int,
	Char,
	Identifier,  //
	IntLiteral,  //
	Main,
	Dot, // ,
	Show,
	Return,
	GOTO,
	IFNZ,
	While,
	And,
	Or,
	Equal,
	Break,
}TokenType;
typedef struct Token
{
	enum TokenType type;
	char text[256];
	struct Token* next;
	struct Token* parent;
}Token;
typedef struct Token* TokenList;


typedef enum ASTNodeType
{
	Program,            
	IntDeclaration,     
	ExpressionStmt,     
	AssignmentASTnode , 
  	Primary,            
	Addtive,            
	Multiply,
	IdentifierASTnode,  
	IntLiteralASTnode, 
	BodyASTNode,	    
	IfASTNode,
	ElseASTNode,
	BooleASTNode,
	GTASTNode,
	LTASTNode,
	ShowASTNode,
	WhileASTNode,
	AndASTNode,
	OrASTNode,
	EqualASTNode,
	BreakASTNode,
}ASTNodeType;


typedef  struct ASTNode
{
	ASTNodeType nodetype;         
	char text[256];								
	struct ASTNode* parent;				
	struct ASTChildren* children;
}ASTNode;
typedef struct ASTNode* ASTNodeList;    

typedef struct  ASTChildren
{
	struct ASTNode* nodelist;
	struct ASTChildren* next;
}ASTChildren;
typedef struct ASTChildren* ASTChildrenList;

typedef struct Result
{
	int result;
}Result;
typedef struct Result* ResultList;  

typedef struct IdTable
{
	char text[256];
	int value;
	int flag;
	struct IdTable* next;
}IdTable;
typedef struct IdTable* IdTableList;

typedef struct IRnode
{
	char a[256];
	int flag;     
	int Regester; 
	int Memory;   
	struct IRnode* next;
}IRnode;
typedef struct IRnode* IRList;