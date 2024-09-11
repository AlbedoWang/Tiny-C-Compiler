#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GA.h"

int tempVarCount = 1;
int labelCount = 1;
int startLabel = 0;
int endLabel = 0;


IRList IR(ASTNodeList node){
	IRList irList = NULL;
	ASTChildrenList child = NULL;
	ASTChildrenList childtmp = NULL;
	IRList childList1 = NULL;
	IRList childList2 = NULL;
	IdTableList tmpIdNode = NULL;
	ASTNodeList Looptmp = NULL;

	char L1tmp[100];
	char L2tmp[100];

	char IRtmp[256];
	int L1;
	int L2;

	switch (node->nodetype)
	{
	case Program:
		child = node->children;
		while (child != NULL)
		{
			IR(child->nodelist);
			child = child->next;
		}
		break;

	case BodyASTNode:
		child = node->children;
		while (child != NULL)
		{
			IR(child->nodelist);
			child = child->next;
		}
		break;

	case AssignmentASTnode:

		tmpIdNode = findIdentifier(node->text);
		if (tmpIdNode == NULL)
		{
			printf("%s not identified\n", node->text);
			exit(0);
		}
		child = node->children;
		childList1 = IR(child->nodelist);
		if (childList1 != NULL)
		{
			printf("\t%s = %s\n", node->text, childList1->a);
			fprintf(IRFile, "\t%s = %s\n", node->text, childList1->a);
			tmpIdNode->flag = 1;
		}
		break;

	case IntDeclaration:
		child = node->children;
		if (child != NULL)
		{
			childList1 = IR(child->nodelist);
			insertIdNode(node->text, 0, 1);
			printf("\t%s = %s\n", node->text, childList1->a);
			fprintf(IRFile, "%s = %s\n", node->text, childList1->a);
		}
		else
		{
			insertIdNode(node->text, 0, 0);
		}
		break;

	case IdentifierASTnode:
		tmpIdNode = findIdentifier(node->text);
		if (tmpIdNode == NULL)
		{
			printf("%s not identified\n", node->text);
			exit(0);
		}
		if (tmpIdNode->flag == 0)
		{
			printf("%s not assignment\n", node->text);
			exit(0);
		}
		irList = (IRList)malloc(sizeof(IRnode));
		strcpy(irList->a, node->text);
		break;

	case IntLiteralASTnode:
		irList = (IRList)malloc(sizeof(IRnode));
		strcpy(irList->a, node->text);
		break;

	case Addtive:
		child = node->children;
		char totalExpr[256] = {0};

		while (child != NULL) {
			childList1 = IR(child->nodelist);
			if (childList1 == NULL) {
				printf("Error evaluating child of additive expression\n");
				exit(0);
			}

			if (totalExpr[0] != '\0') {
				if (strcmp(node->text, "+") == 0) {
					printf("\tT%d = %s + %s\n", tempVarCount, totalExpr, childList1->a);
					fprintf(IRFile, "\tT%d = %s + %s\n", tempVarCount, totalExpr, childList1->a);
					sprintf(totalExpr, "T%d", tempVarCount++);
				} else if (strcmp(node->text, "-") == 0) {
					printf("\tT%d = %s - %s\n", tempVarCount, totalExpr, childList1->a);
					fprintf(IRFile, "\tT%d = %s - %s\n", tempVarCount, totalExpr, childList1->a);
					sprintf(totalExpr, "T%d", tempVarCount++);
				} else {
					printf("Unknown operator in multiplication/division expression\n");
					exit(0);
				}
			} else {
				strcpy(totalExpr, childList1->a);
			}

			child = child->next;
		}

		irList = (IRList)malloc(sizeof(IRnode));
		strcpy(irList->a, totalExpr);
    	break;
	
	case Multiply:
		child = node->children;
		childList1 = IR(child->nodelist); 
		if (childList1 == NULL) {
			printf("Error evaluating left operand of multiplication expression\n");
			exit(0);
		}

		child = child->next;
		childList2 = IR(child->nodelist); 
		if (childList2 == NULL) {
			printf("Error evaluating right operand of multiplication expression\n");
			exit(0);
		}

		irList = (IRList)malloc(sizeof(IRnode));
		if (irList == NULL) {
			printf("Memory allocation failed for multiplication result\n");
			exit(0);
		}

		if (strcmp(node->text, "*") == 0) {
			printf("\tT%d = %s * %s\n", tempVarCount, childList1->a, childList2->a);
			fprintf(IRFile, "\tT%d = %s * %s\n", tempVarCount, childList1->a, childList2->a);
		} else if (strcmp(node->text, "/") == 0) {
			printf("\tT%d = %s / %s\n", tempVarCount, childList1->a, childList2->a);
			fprintf(IRFile, "\tT%d = %s / %s\n", tempVarCount, childList1->a, childList2->a);
		} else {
			printf("Unknown operator in multiplication/division expression\n");
			exit(0);
		}

		sprintf(irList->a, "T%d", tempVarCount++);
		break;

	case ShowASTNode:
		childList1 = IR(node->children->nodelist);
		printf("\tprint %s\n", childList1->a);
		fprintf(IRFile, "print %s\n", childList1->a);
		break;

	case IfASTNode:
		childList1 = IR(node->children->nodelist);
		// printf("%d\n", node->nodetype);
		if (childList1 == NULL) {
			printf("Error evaluating condition of if statement\n");
			exit(0);
		}
		// printf("T%d = %s\n", tempVarCount, childList1->a);
		// fprintf(IRFile, "T%d = %s\n", tempVarCount, childList1->a);

		L1 = labelCount++;
		printf("\tIFNZ %s GOTO L%d\n", childList1->a, L1);
		fprintf(IRFile, "\tIFNZ T%d GOTO L%d\n", childList1->a, L1);
		// tempVarCount++;

		// IR(node->children->next->nodelist);
		childtmp = node->children->next->next;
		
		if (node->children->next->next) {
			IR(node->children->next->next->nodelist);
			L2 = labelCount++;
			printf("\tGOTO L%d\n", L2);
		}

		printf("L%d:\n", L1);
		fprintf(IRFile, "L%d:\n", L1);
		IR(node->children->next->nodelist);
		if(childtmp){
			printf("L%d:\n", L2);
			fprintf(IRFile, "L%d:\n", L2);
		}

		break;

	case ElseASTNode:
		IR(node->children->nodelist);
		break;
	
	case BooleASTNode:
		child = node->children;
		childList1 = IR(child->nodelist);
		if (childList1 == NULL) {
			printf("Error in evaluating boolean expression\n");
			exit(0);
		}
		irList = (IRList)malloc(sizeof(IRnode));
		strcpy(irList->a, childList1->a);
		break;

	case LTASTNode:
		childList1 = IR(node->children->nodelist);
		childList2 = IR(node->children->next->nodelist);
		if (childList1 == NULL || childList2 == NULL) {
			printf("Error in evaluating operands of '<' operation\n");
			exit(0);
		}
		irList = (IRList)malloc(sizeof(IRnode));
		if (irList == NULL) {
			printf("Memory allocation failed\n");
			exit(0);
		}
		printf("\tT%d = %s < %s\n", tempVarCount, childList1->a, childList2->a);
		fprintf(IRFile, "\tT%d = %s < %s\n", tempVarCount, childList1->a, childList2->a);
		sprintf(irList->a, "T%d", tempVarCount++);
		break;

	case GTASTNode:
		childList1 = IR(node->children->nodelist);
		childList2 = IR(node->children->next->nodelist);
		if (childList1 == NULL || childList2 == NULL) {
			printf("Error in evaluating operands of '>' operation\n");
			exit(0);
		}
		irList = (IRList)malloc(sizeof(IRnode));
		if (irList == NULL) {
			printf("Memory allocation failed\n");
			exit(0);
		}
		printf("\tT%d = %s > %s\n", tempVarCount, childList1->a, childList2->a);
		fprintf(IRFile, "\tT%d = %s > %s\n", tempVarCount, childList1->a, childList2->a);
		sprintf(irList->a, "T%d", tempVarCount++);
		break;
	
	case AndASTNode:
		child = node->children;
		childList1 = IR(child->nodelist); 
		if (childList1 == NULL) {
			printf("Error evaluating left operand of AND expression\n");
			exit(0);
		}

		child = child->next;
		childList2 = IR(child->nodelist);  
		if (childList2 == NULL) {
			printf("Error evaluating right operand of AND expression\n");
			exit(0);
		}

		irList = (IRList)malloc(sizeof(IRnode));
		printf("\tT%d = %s AND %s\n", tempVarCount, childList1->a, childList2->a);
		fprintf(IRFile, "\tT%d = %s AND %s\n", tempVarCount, childList1->a, childList2->a);
		sprintf(irList->a, "T%d", tempVarCount++);
		break;

	case OrASTNode:
		child = node->children;
		childList1 = IR(child->nodelist);
		if (childList1 == NULL) {
			printf("Error evaluating left operand of OR expression\n");
			exit(0);
		}

		child = child->next;
		childList2 = IR(child->nodelist);  
		if (childList2 == NULL) {
			printf("Error evaluating right operand of OR expression\n");
			exit(0);
		}

		irList = (IRList)malloc(sizeof(IRnode));
		printf("\tT%d = %s OR %s\n", tempVarCount, childList1->a, childList2->a);
		fprintf(IRFile, "\tT%d = %s OR %s\n", tempVarCount, childList1->a, childList2->a);
		sprintf(irList->a, "T%d", tempVarCount++);
		break;

	case EqualASTNode:
		child = node->children;
		childList1 = IR(child->nodelist);
		if (childList1 == NULL) {
			printf("Error evaluating left operand of EQUAL expression\n");
			exit(0);
		}

		child = child->next;
		childList2 = IR(child->nodelist);
		if (childList2 == NULL) {
			printf("Error evaluating right operand of EQUAL expression\n");
			exit(0);
		}

		irList = (IRList)malloc(sizeof(IRnode));
		printf("\tT%d = %s == %s\n", tempVarCount, childList1->a, childList2->a);
		fprintf(IRFile, "\tT%d = %s == %s\n", tempVarCount, childList1->a, childList2->a);
		sprintf(irList->a, "T%d", tempVarCount++);
		break;


	case WhileASTNode:
        startLabel = labelCount++;
        endLabel = labelCount++;

        printf("L%d:\n", startLabel);
        fprintf(IRFile, "L%d:\n", startLabel);

        child = node->children;
        childList1 = IR(child->nodelist);

        if (childList1 == NULL) {
            printf("Error evaluating the condition of the while loop\n");
            exit(0);
        }

        printf("\tIFNZ %s GOTO L%d\n", childList1->a, endLabel);
        fprintf(IRFile, "\tIFNZ %s GOTO L%d\n", childList1->a, endLabel);

        child = child->next;
        if (child != NULL) {
            IR(child->nodelist);
        } else {
            printf("Error: Missing body of the while loop\n");
            exit(0);
        }

        printf("\tGOTO L%d\n", startLabel);
        fprintf(IRFile, "GOTO L%d\n", startLabel);

        printf("L%d:\n", endLabel);
        fprintf(IRFile, "L%d:\n", endLabel);
    	break;

	case BreakASTNode:
		printf("\tGOTO L%d\n", endLabel);
		break;

	default:
		break;
	}
	return irList;
}


int main() {
	sFile = fopen("./text_compile/test2.txt", "r");

	if (sFile == NULL) {
		printf("Fail to open file!\n");
		return 0;
	}
	list = NULL;
	TokenReader();

	TokenList p = list;
	printf("-Lexer Analysis\n");
	while (p != NULL) {
		printf("Token = '%s'\tType = % d\n", p->text, p->type);
		p = p->next;
	}

	printf(
		"\n-Grammar Analysis\n\n"
		"--Print AST\n");
	pList = list;
	ASTNodeList AST = Programming();

	if (AST != NULL) {
		dumpAST(AST, 0);
		printf("\n--Print parameter value in 'show' function\n");
		evaluate(AST);
		printf("\n--Pirnt IR\n");
		IR(AST);
	}
	fclose(sFile);
	fclose(IRFile);
	return 0;
}
