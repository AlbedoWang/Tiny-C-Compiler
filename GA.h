#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexanalysis.h"


extern TokenList list;
extern TokenList IR_list;
extern FILE* sFile;
extern void TokenReader();
extern int cmpstring(char a[], char b[]);

IdTableList IdHead = NULL;
TokenList pList;
IRList IRList_Head = NULL;
FILE* IRFile;

ASTNodeList assignmentStatment();
ASTNodeList primary();
ASTNodeList intDeclare();
ASTNodeList intNodeList();
ASTNodeList boolStatement();
ASTNodeList addtiveStatment();
ASTNodeList relStatement();

ASTNodeList Body();

void varDeclarationBody(ASTNodeList node, TokenList tokentemp);
void ifBody(ASTNodeList node, TokenList tokentemp);
void showBody(ASTNodeList node, TokenList tokentemp);
void assignmentBody(ASTNodeList node, TokenList tokentemp);

int continueSign = 1;
int breakSign = 1;

char ASTNodeTypeString[30][256] = {
    "Program",
    "IntDeclaration",
    "ExpressionStmt",
    "Assignment",
    "Primary",
    "Addtive",
    "Multiplication",
    "Identifier",
    "IntLiteral",
    "Body",
    "If",
    "Else",
    "Boole",
    "GT",
    "LT",
    "Show",
    "While",
    "And",
    "Or",
    "Equal",
    "Break",
};

void addASTNode(ASTNodeList node, ASTNodeList child) {
  if (node != NULL && child != NULL) {
    ASTChildrenList childlist = (ASTChildrenList)malloc(sizeof(ASTChildren));
    childlist->nodelist = child;
    childlist->next = NULL;
    if (node->children == NULL) {
      node->children = childlist;
    } else {
      ASTChildrenList p = node->children;
      while (p->next != NULL) {
        p = p->next;
      }
      p->next = childlist;
    }
    child->parent = node;
  }
  return;
  
}

IdTableList findIdentifier(char a[]) {
  IdTableList p = IdHead;
  while (p != NULL) {
    if (cmpstring(a, p->text)) {
      return p;
    }
    p = p->next;
  }
  return p;
}

void printId(){
  IdTableList p = IdHead;
  while (p != NULL){
    printf("Id name: %s\tvalue: %d", p->text, p->value);
    p = p->next;
  }
}

void insertIdNode(char a[], int value, int k) {
  IdTableList idNode = (IdTableList)malloc(sizeof(IdTable));
  strcpy(idNode->text, a);
  idNode->value = value;
  idNode->flag = k;
  idNode->next = IdHead;
  IdHead = idNode;
  return;
}

int hundred(int n) {
  int re = 1;
  if (n == 0) {
    return 1;
  }
  while (n > 0) {
    re *= 10;
    n--;
  }
  return re;
}

int literalToNumber(char a[]) {
  int lenth = strlen(a) - 1;
  int i = lenth;
  int b = 0;
  while (i >= 0) {
    b += (int)(a[i] - '0') * hundred(lenth - i);
    i--;
  }
  return b;
}

ASTNodeList multiStatment() {
  // printf("Call multiStatment function\n");
  ASTNodeList child1 = primary();
  ASTNodeList node = child1;
  TokenList tokentmp;

  while (1) {
    tokentmp = pList;
    if (tokentmp != NULL && (tokentmp->type == Multip)) {
      pList = pList->next;
      ASTNodeList child2 = primary();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        strcpy(node->text, tokentmp->text);
        node->nodetype = Multiply;
        node->children = NULL;
        node->parent = NULL;
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("invalid multiplication expression,expecting the right part\n");
        node = NULL;
      }
    } 
    else if (tokentmp != NULL && (tokentmp->type == Div)) {
      pList = pList->next;
      ASTNodeList child2 = primary();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        strcpy(node->text, tokentmp->text);
        node->nodetype = Multiply;
        node->children = NULL;
        node->parent = NULL;
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("invalid division expression,expecting the right part\n");
        node = NULL;
      }
    }
    else {
      break;
    }
  }
  return node;
}

ASTNodeList addtiveStatment() {
  // printf("Call additivestatment function\n");
  ASTNodeList child1 = multiStatment();
  ASTNodeList node = child1;
  TokenList tokentmp;
  while (1) {
    tokentmp = pList;
    if (tokentmp != NULL && (tokentmp->type == Plus)) {
      pList = pList->next;
      ASTNodeList child2 = multiStatment();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        strcpy(node->text, tokentmp->text);
        node->nodetype = Addtive;
        node->children = NULL;
        node->parent = NULL;
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("invalid Addtive expression,expecting the right part\n");
        node = NULL;
      }
    } else if (tokentmp != NULL && (tokentmp->type == Sub)) {
      pList = pList->next;
      ASTNodeList child2 = multiStatment();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        strcpy(node->text, tokentmp->text);
        node->nodetype = Addtive;
        node->children = NULL;
        node->parent = NULL;
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("invalid Addtive expression,expecting the right part\n");
        node = NULL;
      }
    } else {
      break;
    }
  }
  return node;
}

ASTNodeList equalStatement() {
  ASTNodeList child1 = relStatement(); 
  ASTNodeList node = child1;
  TokenList tokentmp;

  while (1) {
    tokentmp = pList;
    if (tokentmp != NULL && tokentmp->type == Equal) {
      pList = pList->next;
      ASTNodeList child2 = relStatement();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        strcpy(node->text, tokentmp->text);
        node->nodetype = EqualASTNode;
        node->children = NULL;
        node->parent = NULL;
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("Error in andStatement: expecting right operand of Equal\n");
        exit(0);
      }
    } else {
      break;
    }
  }
  return node;
}

ASTNodeList andStatement() {
  ASTNodeList child1 = equalStatement(); 
  ASTNodeList node = child1;
  TokenList tokentmp;

  while (1) {
    tokentmp = pList;
    if (tokentmp != NULL && tokentmp->type == And) {
      pList = pList->next;
      ASTNodeList child2 = equalStatement();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        strcpy(node->text, tokentmp->text);
        node->nodetype = AndASTNode;
        node->children = NULL;
        node->parent = NULL;
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("Error in andStatement: expecting right operand of AND\n");
        exit(0);
      }
    } else {
      break;
    }
  }
  return node;
}

// And Or Equal
ASTNodeList AOEStatement() {
  // printf("Call AOEStatement function\n");
  ASTNodeList child1 = andStatement();
  ASTNodeList node = child1;
  TokenList tokentmp = pList;

  while (1) {
    tokentmp = pList;
    // if  (tokentmp != NULL && tokentmp->type == And) {
    //   pList = pList->next;
    //   ASTNodeList child2 = andStatement();
    //   if (child2 != NULL) {
    //     node = (ASTNodeList)malloc((sizeof(ASTNode)));
    //     node->nodetype = AndASTNode;
    //     node->children = NULL;
    //     node->parent = NULL;
    //     strcpy(node->text, tokentmp->text);
    //     addASTNode(node, child1);
    //     addASTNode(node, child2);
    //     child1 = node;
    //   } else {
    //     printf("error:Lack of Rel after EQStatement\n");
    //     exit(0);
    //   }
    // }
    // else 
    if (tokentmp != NULL && tokentmp->type == Or) {
      pList = pList->next;
      ASTNodeList child2 = andStatement();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc((sizeof(ASTNode)));
        node->nodetype = OrASTNode;
        node->children = NULL;
        node->parent = NULL;
        strcpy(node->text, tokentmp->text);
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("error:Lack of Rel after EQStatement\n");
        exit(0);
      }
    }
    // else if (tokentmp != NULL && tokentmp->type == Equal) {
    //   pList = pList->next;
    //   ASTNodeList child2 = andStatement();
    //   if (child2 != NULL) {
    //     node = (ASTNodeList)malloc((sizeof(ASTNode)));
    //     node->nodetype = EqualASTNode;
    //     node->children = NULL;
    //     node->parent = NULL;
    //     strcpy(node->text, tokentmp->text);
    //     addASTNode(node, child1);
    //     addASTNode(node, child2);
    //     child1 = node;
    //   } else {
    //     printf("error:Lack of Rel after EQStatement\n");
    //     exit(0);
    //   }
    // }
    else {
      break;
    }
  }
  return node;
}

ASTNodeList relStatement() {
  // printf("Call relStatement function\n");
  ASTNodeList child1 = addtiveStatment();
  ASTNodeList node = child1;
  TokenList tokentmp = pList;

  while (1) {
    tokentmp = pList;
    if (tokentmp != NULL && tokentmp->type == GT) {
      pList = pList->next;
      ASTNodeList child2 = addtiveStatment();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc((sizeof(ASTNode)));
        node->nodetype = GTASTNode;
        node->children = NULL;
        node->parent = NULL;
        strcpy(node->text, tokentmp->text);
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("error:Lack of Rel after EQStatement\n");
        exit(0);
      }
    } else if (tokentmp != NULL && tokentmp->type == LT) {
      pList = pList->next;
      ASTNodeList child2 = addtiveStatment();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc((sizeof(ASTNode)));
        node->nodetype = LTASTNode;
        node->children = NULL;
        node->parent = NULL;
        strcpy(node->text, tokentmp->text);
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("error:Lack of Rel after EQStatement\n");
        exit(0);
      }
    } else {
      break;
    }
  }
  return node;
}

ASTNodeList whilestat() {
  // printf("Call whilestatement function\n");
  ASTNodeList node = NULL;
  TokenList tokentmp = pList;

  if (tokentmp != NULL && tokentmp->type == While) {
      pList = pList->next;
      node = (ASTNodeList)malloc(sizeof(ASTNode));
      node->nodetype = WhileASTNode;
      strcpy(node->text, tokentmp->text);
      node->children = NULL;
      node->parent = NULL;
      tokentmp = pList;
      TokenList tmp = pList;
      if (tokentmp != NULL && tokentmp->type == LeftParen) {
        pList = pList->next;
        ASTNodeList child = boolStatement();
        if (child == NULL) {
          printf("invalide while if statement in ifStatment, expecting RightParen\n");
          node = NULL;
        }
        else {
          addASTNode(node, child);
          tokentmp = pList;
          
          if (tokentmp != NULL && tokentmp->type == RightParen) {
            pList = pList->next;
            tokentmp = pList;
            if (tokentmp != NULL && tokentmp->type == LeftBrace) {
              pList = pList->next;
              ASTNodeList child =Body();
              if (child == NULL) {
                 printf(
                  "invalide while statement in whileStatment,expecting "
                   "body\n");
                  node = NULL;
              }
              else {
                addASTNode(node, child);
                tokentmp = pList;
                if (tokentmp != NULL && tokentmp->type == RightBrace) 
                {
                    pList = pList->next;
                }
              }

            }else {
              printf(
              "invalid statement in whileStatment,expecting semicolon\n");
            }
          }else{
            printf(
              "invalid statement in whileStatment,expecting RightParen\n");
          }
        }

      }else {
      pList = tmp;
      node = NULL;
    }

  }
  return node;
}

ASTNodeList breakStatement() {
    ASTNodeList node = NULL;
    TokenList tokentmp = pList;
    // printf("%s , %d ", pList->text, pList->type);
    if (tokentmp != NULL && tokentmp->type == Break) {
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        node->nodetype = BreakASTNode;
        strcpy(node->text, tokentmp->text);
        node->children = NULL;
        node->parent = NULL;
        pList = pList->next->next;
        // printf("%s , %d ", pList->text, pList->type);
    }
    return node;
}

ASTNodeList boolStatement() {
  // printf("Call boolstatement function\n");
  TokenList tokentmp = pList;
  ASTNodeList node = (ASTNodeList)malloc(sizeof(ASTNode));
  node->nodetype = BooleASTNode;
  node->children = NULL;
  node->parent = NULL;
  strcpy(node->text, "Boole");

  ASTNodeList child1 = AOEStatement();
  addASTNode(node, child1);
  return node;
}

// 赋值语句
ASTNodeList assignmentStatment() {
  ASTNodeList node = NULL;
  TokenList tokentmp = pList;

  if (tokentmp != NULL && tokentmp->type == Identifier) {
    pList = pList->next;
    node = (ASTNodeList)malloc(sizeof(ASTNode));
    node->nodetype = AssignmentASTnode;
    strcpy(node->text, tokentmp->text);
    node->children = NULL;
    node->parent = NULL;
    tokentmp = pList;
    TokenList tmp = pList;
    if (tokentmp != NULL && tokentmp->type == Assignment) {
      pList = pList->next;
      ASTNodeList child = addtiveStatment();
      if (child == NULL) {
        printf("invalid statement in assignmentStatment, expecting semicolon\n");
        node = NULL;
      } else {
        addASTNode(node, child);
        tokentmp = pList;
        if (tokentmp != NULL && tokentmp->type == SemiColon) {
          pList = pList->next;
        } else {
          printf("invalid statement in assignmentStatment, expecting semicolon\n");
        }
      }
    } else {
      pList = tmp;
      node = NULL;
    }
  }
  return node;
}

ASTNodeList primary() {
  // printf("Call primary function\n");
  ASTNodeList node = NULL;
  TokenList tokentmp = pList;

  if (tokentmp != NULL) {
    if (tokentmp->type == IntLiteral) {
      pList = pList->next;
      node = (ASTNodeList)malloc(sizeof(ASTNode));
      node->nodetype = IntLiteralASTnode;
      strcpy(node->text, tokentmp->text);
      node->children = NULL;
      node->parent = NULL;
    } else if (tokentmp->type == Identifier) {
      pList = pList->next;
      node = (ASTNodeList)malloc(sizeof(ASTNode));
      node->nodetype = IdentifierASTnode;
      strcpy(node->text, tokentmp->text);
      node->children = NULL;
      node->parent = NULL;
    } else if (tokentmp->type == LeftParen) {
      pList = pList->next;
      node = AOEStatement();
      if (node != NULL) {
        tokentmp = pList;
        if (tokentmp != NULL && tokentmp->type == RightParen) {
          pList = pList->next;
        } else {
          printf("expecting right parenthesis in expression\n");
          node = NULL;
        }
      } else {
        printf(
            "expecting an additive expression inside parenthesis in "
            "expression\n");
        node = NULL;
      }
    }
  }
  return node;
}

ASTNodeList intDeclare() {
  // printf("Call intDeclare function\n");
  ASTNodeList node = NULL;
  TokenList tokentmp = pList;

  if (tokentmp != NULL && tokentmp->type == Int) {
    tokentmp = pList;
    pList = pList->next;
    if (pList != NULL && pList->type == Identifier) {
      tokentmp = pList;
      pList = pList->next;
      node = (ASTNodeList)malloc(sizeof(ASTNode));
      strcpy(node->text, tokentmp->text);
      node->nodetype = IntDeclaration;
      node->children = NULL;
      node->parent = NULL;
      tokentmp = pList;
      if (tokentmp != NULL && tokentmp->type == Assignment) {
        pList = pList->next;
        ASTNodeList child = addtiveStatment();
        if (child == NULL) {
          printf("invalide variable initialization,\
          expecting an expression\n");
          node = NULL;
        } else {
          addASTNode(node, child);
        }
      }
    } else {
      printf("variable name expected!\n");
      node = NULL;
    }
  }
  return node;
}

void dumpAST(ASTNodeList p, int deepth) {
  if (p == NULL) {
    return;
  }
  char a[100];
  memset(a, '\0', 100);
  for (int i = 0; i < deepth; i++) {
    a[i] = '\t';
  }
  printf("%s%s\t%s\n", a, ASTNodeTypeString[p->nodetype], p->text);
  ASTChildrenList children = p->children;
  while (children != NULL) {
    dumpAST(children->nodelist, deepth + 1);
    children = children->next;
  }
  return;
}

ASTNodeList intNodeList() {
  // printf("Call intNodeList function\n");
  ASTNodeList node = NULL;
  TokenList tokentmp = pList;

  if (pList != NULL && pList->type == Identifier) {
    tokentmp = pList;
    pList = pList->next;
    node = (ASTNodeList)malloc(sizeof(ASTNode));
    strcpy(node->text, tokentmp->text);
    node->nodetype = IntDeclaration;
    node->children = NULL;
    node->parent = NULL;
    tokentmp = pList;
    if (tokentmp != NULL && tokentmp->type == Assignment) {
      pList = pList->next;
      ASTNodeList child = addtiveStatment();
      if (child == NULL) {
        printf("invalide variable initialization, expecting an expression\n");
        node = NULL;
      } else {
        addASTNode(node, child);
      }
    }
  } else {
    printf("variable name expected!\n");
    node = NULL;
  }
  return node;
}

ResultList evaluate(ASTNodeList node) {
  // printf("Call evaluate function\n");
  ResultList resultList = NULL;
  ASTChildrenList child = NULL;
  ASTChildrenList child2 = NULL;
  ResultList childList1 = NULL;
  ResultList childList2 = NULL;
  IdTableList tmpIdNode = NULL;
  IdTableList tmpIdNode1 = NULL;
  ASTNodeList parent = NULL;
  ASTNodeList parenttmp = NULL;
  ResultList conditionResult = NULL;
  int tmp, tmp1, tmpcontinue, tmpbreak;

  switch (node->nodetype) {
    case Program:
      child = node->children;
      while (child != NULL) {
        evaluate(child->nodelist);
        child = child->next;
      }
      break;

    case BodyASTNode:
      child = node->children;
      while (child != NULL) {
        evaluate(child->nodelist);
        child = child->next;
      }
      break;

    case IntDeclaration:
      child = node->children;
      if (child != NULL && continueSign == 1 && breakSign == 1) {
        childList1 = evaluate(child->nodelist);
        insertIdNode(node->text, childList1->result, 1);
      } else {
        insertIdNode(node->text, 0, 0);
      }
      break;

    case IntLiteralASTnode:
      resultList = (ResultList)malloc(sizeof(Result));
      resultList->result = literalToNumber(node->text);
      break;

    case IdentifierASTnode:
      tmpIdNode = findIdentifier(node->text);
      if (tmpIdNode == NULL) {
        printf("%s not identified\n", node->text);
        exit(0);
      }
      if (tmpIdNode->flag == 0) {
        printf("%s not assignment\n", node->text);
        exit(0);
      }
      resultList = (ResultList)malloc(sizeof(Result));
      resultList->result = tmpIdNode->value;
      break;

    case LTASTNode:
      child = node->children;
      tmp = evaluate(child->nodelist)->result;
      child = child->next;
      tmp1 = evaluate(child->nodelist)->result;
      resultList = (ResultList)malloc(sizeof(Result));
      if (tmp < tmp1) {
        resultList->result = 1;
      } else {
        resultList->result = 0;
      }
      break;

    case GTASTNode:
      child = node->children;
      tmp = evaluate(child->nodelist)->result;
      child = child->next;
      tmp1 = evaluate(child->nodelist)->result;
      resultList = (ResultList)malloc(sizeof(Result));
      if (tmp > tmp1) {
        resultList->result = 1;
      } else {
        resultList->result = 0;
      }
      break;

    case AndASTNode:
      child = node->children;
      tmp = evaluate(child->nodelist)->result;
      child = child->next;
      tmp1 = evaluate(child->nodelist)->result;
      resultList = (ResultList)malloc(sizeof(Result));
      if (tmp && tmp1) {
        resultList->result = 1;
      } else {
        resultList->result = 0;
      }
      break;

    case OrASTNode:
      child = node->children;
      tmp = evaluate(child->nodelist)->result;
      child = child->next;
      tmp1 = evaluate(child->nodelist)->result;
      resultList = (ResultList)malloc(sizeof(Result));
      if (tmp || tmp1) {
        resultList->result = 1;
      } else {
        resultList->result = 0;
      }
      break;

    case EqualASTNode:
      child = node->children;
      tmp = evaluate(child->nodelist)->result;
      child = child->next;
      tmp1 = evaluate(child->nodelist)->result;
      resultList = (ResultList)malloc(sizeof(Result));
      if (tmp == tmp1) {
        resultList->result = 1;
      } else {
        resultList->result = 0;
      }
      break;

    case Addtive:
      child = node->children;
      if (child != NULL) {
        childList1 = evaluate(child->nodelist);
        if (childList1 == NULL) {
          printf("Error evaluating left child of additive expression\n");
          exit(0);
        }
      } else {
        printf("Left child of additive expression is missing\n");
        exit(0);
      }

      child = child->next;
      if (child != NULL) {
        childList2 = evaluate(child->nodelist);
        if (childList2 == NULL) {
          printf("Error evaluating right child of additive expression\n");
          exit(0);
        }
      } else {
        printf("Right child of additive expression is missing\n");
        exit(0);
      }

      resultList = (ResultList)malloc(sizeof(Result));
      if (resultList == NULL) {
        printf("Memory allocation failed for additive result\n");
        exit(0);
      }
      if (cmpstring(node->text, "+")) {
        resultList->result = childList1->result + childList2->result;
      } else {
        resultList->result = childList1->result - childList2->result;
      }
      break;

    case Multiply:
      child = node->children;
      childList1 = evaluate(child->nodelist);
      if(childList1==NULL){
        printf("Error evaluating right child of multiplication expression\n");
      }
      child = child->next;
      childList2 = evaluate(child->nodelist);
      if(childList2==NULL){
        printf("Error evaluating left child of multiplication expression\n");
      }
      resultList = (ResultList)malloc(sizeof(Result));
      if (cmpstring(node->text, "*")) {
        resultList->result = childList1->result * childList2->result;
      } else {
        resultList->result = childList1->result / childList2->result;
      }
      break;

    case AssignmentASTnode:
      tmpIdNode = findIdentifier(node->text);
      if (tmpIdNode == NULL) {
        printf("%s not identified\n", node->text);
        exit(0);
      }
      if (tmpIdNode->flag == 0) {
        tmpIdNode->flag = 1;
      }
      child = node->children;
      if (child != NULL) {
        childList2 = evaluate(child->nodelist);
        if (childList2 == NULL) {
          printf("Error evaluating right child of additive expression\n");
          exit(0);
        }
      } else {
        printf("Right child of additive expression is missing\n");
        exit(0);
      }
      tmp = childList2->result;
      tmpIdNode->value = tmp;
      break;

    case ShowASTNode:
      child = node->children;
      if (child != NULL) {
        childList1 = evaluate(child->nodelist);
      } else {
        printf("Right child of additive expression is missing\n");
        exit(0);
      }
      tmp = childList1->result;
      printf("%d\n", tmp);

      break;

    case IfASTNode:
      conditionResult = evaluate(node->children->nodelist);
      if (conditionResult->result) {
        evaluate(node->children->next->nodelist);
      } else if (node->children->next->next) {
        evaluate(node->children->next->next->nodelist);
      }
      break;

    case ElseASTNode:
      evaluate(node->children->nodelist);
      break;

    case WhileASTNode:
      child = node->children;
      tmp = evaluate(child->nodelist)->result;
      child2 = child->next;
      while (tmp == 1) {
        evaluate(child2->nodelist);
        tmp = evaluate(child->nodelist)->result;
      } 
      break;

    case BooleASTNode:
      child = node->children;
      resultList = evaluate(child->nodelist);
      break;

    case BreakASTNode:
      parenttmp = node;
      while (parenttmp){
        if (parenttmp->nodetype!=WhileASTNode){
          parenttmp = parenttmp->parent;
        }
      }
      if (parenttmp==NULL) {
        printf("Break in No Loop\n");
      }
      break;

    default:
      break;
  }
  return resultList;
}

void varDeclarationBody(ASTNodeList node, TokenList tokentmp) {
  // printf("Call varDeclarationBody function\n");
  ASTNodeList child = intDeclare();
  if (child != NULL) {
    addASTNode(node, child);
  } else {
    printf("error:IntDeclare\n");
    exit(0);
  }

  while (1) {
    if (pList != NULL && pList->type == Dot) {
      pList = pList->next;
      ASTNodeList child = intNodeList();
      if (child != NULL) {
        addASTNode(node, child);
      } else {
        printf("error:IntDeclare\n");
        exit(0);
      }
    } else if (pList != NULL && pList->type == SemiColon) {
      pList = pList->next;
      break;
    } else {
      printf("invalide statement in varDeclaration,\
      expecting semicolon\n");
    }
  }
}

void assignmentBody(ASTNodeList node, TokenList tokentmp) {
  // printf("Call assignmentBody function\n");
  ASTNodeList child = assignmentStatment();
  if (child != NULL) {
    addASTNode(node, child);
  } else {
    printf("error : assignmentStatment\n");
    exit(0);
  }
}

void showBody(ASTNodeList node, TokenList tokentmp) {
  // printf("Call showBody function\n");
  tokentmp = pList; 
  if (tokentmp != NULL && tokentmp->type == Show) {
    pList = pList->next; 
    tokentmp = pList; 
    if (tokentmp != NULL && tokentmp->type == LeftParen) {
      pList = pList->next;
      tokentmp = pList;
      if (tokentmp != NULL && tokentmp->type == Identifier) {
        ASTNodeList newnode = (ASTNodeList)malloc(sizeof(ASTNode));
        strcpy(newnode->text, "show");
        newnode->nodetype = ShowASTNode;
        newnode->children = NULL;
        newnode->parent = NULL;
        // printf("%s , %d ", pList->text, pList->type);
        ASTNodeList child = addtiveStatment();
        if (child != NULL) {
          addASTNode(newnode, child);
        } else {
          printf("error: Primary\n");
          exit(0);
        }
        addASTNode(node, newnode);
        tokentmp = pList;
        if (tokentmp != NULL && tokentmp->type == RightParen) {
          pList = pList->next; 
          tokentmp = pList;
          if (tokentmp != NULL && tokentmp->type == SemiColon) {
            pList = pList->next;
            tokentmp = pList;
          } else {
            printf("Error: Missing semicolon after show statement\n");
            exit(0);
          }
        } else {
          printf("Error: Missing rightparen after show statement\n");
          exit(0);
        }
      } else {
        printf("Error: Expected identifier after 'show'\n");
        exit(0);
      }
    } else {
      printf("Error: Expected leftparen after 'show'\n");
    }
  } else {
    printf("Error: Expected 'show' keyword\n");
    exit(0);
  }
}

void ifBody(ASTNodeList node, TokenList tokentmp) {
  if (tokentmp != NULL && tokentmp->type == If) {
    pList = pList->next; 

    ASTNodeList ifNode = (ASTNodeList)malloc(sizeof(ASTNode));
    strcpy(ifNode->text, "if");
    ifNode->nodetype = IfASTNode;
    ifNode->children = NULL;
    ifNode->parent = NULL;

    ASTNodeList condition = boolStatement();
    if (condition != NULL) {
      addASTNode(ifNode, condition);
    } else {
      printf("Error: Invalid condition in if statement\n");
      exit(0);
    }
    pList = pList->next;
    ASTNodeList ifBody = Body();
    if (ifBody != NULL) {
      addASTNode(ifNode, ifBody);
    } else {
      printf("Error: Invalid body in if statement\n");
      exit(0);
    }
    pList = pList->next;
    tokentmp = pList;
    if (tokentmp != NULL && tokentmp->type == Else) {
      pList = pList->next; 

      ASTNodeList elseNode = (ASTNodeList)malloc(sizeof(ASTNode));
      strcpy(elseNode->text, "else");
      elseNode->nodetype = ElseASTNode;
      elseNode->children = NULL;
      elseNode->parent = NULL;

      pList = pList->next;
      ASTNodeList elseBody = Body();
      if (elseBody != NULL) {
        addASTNode(elseNode, elseBody);
      } else {
        printf("Error: Invalid body in else statement\n");
        exit(0);
      }
      pList = pList->next;
      addASTNode(ifNode, elseNode);
    }

    addASTNode(node, ifNode);
  } else {
    printf("Error: Expected 'if' keyword\n");
    exit(0);
  }
}

void whileBody(ASTNodeList node, TokenList tokentmp) {
  // printf("Call whileBody function\n");
  ASTNodeList child = whilestat();
  if (child != NULL) {
    addASTNode(node, child);
  } else {
    printf("Error : Invalid body in while statement\n");
    exit(0);
  }
}

void BreakBody(ASTNodeList node, TokenList tokentmp) {
  ASTNodeList child = breakStatement();
  if (child != NULL) {
    addASTNode(node, child);
  } else {
    printf("Error : Invalid body in break statement\n");
    exit(0);
  }
}

ASTNodeList Body() {
  // printf("Call Body function\n");
  printf("%s , %d ", pList->text, pList->type);
  ASTNodeList node = NULL;
  node = (ASTNodeList)malloc(sizeof(ASTNode));
  node->nodetype = BodyASTNode;
  strcpy(node->text, "Body");
  node->children = NULL;
  node->parent = NULL;
  TokenList tokentmp = pList;
  ASTNodeList tempBody;
  // printf("Current token type: %d \n", tokentmp->type);

  if (tokentmp != NULL && tokentmp->type == Int) {
    varDeclarationBody(node, tokentmp);
    tempBody = Body();
    addASTNode(node, tempBody);
  } else if (tokentmp != NULL && tokentmp->type == Identifier) {
    assignmentBody(node, tokentmp);
    tempBody = Body();
    addASTNode(node, tempBody);
  } else if (tokentmp != NULL && tokentmp->type == If) {
    ifBody(node, tokentmp);
    tempBody = Body();
    addASTNode(node, tempBody);
  } else if (pList != NULL && pList->type == Show) {
    showBody(node, tokentmp);
    tempBody = Body();
    addASTNode(node, tempBody);
  } else if (tokentmp != NULL && tokentmp->type == While) {
    whileBody(node, tokentmp);
    tempBody = Body();
    addASTNode(node, tempBody);
  } else if (tokentmp != NULL && tokentmp->type == Break) {
    BreakBody(node, tokentmp);
  } else if (pList != NULL && pList->type == RightBrace) {
    return NULL;
  } else {
    printf("error:unknown statement in main body");
    exit(0);
  }
  return node;
}

ASTNodeList Programming() {
  // printf("Call Programming function\n");
  TokenList tokentmp = pList;
  ASTNodeList node = NULL;

  if (tokentmp != NULL && tokentmp->type == Main) {
    pList = pList->next;
    node = (ASTNodeList)malloc(sizeof(ASTNode));
    node->nodetype = Program;
    node->children = NULL;
    node->parent = NULL;
    strcpy(node->text, tokentmp->text);
    tokentmp = pList;
    if (tokentmp != NULL && tokentmp->type == LeftParen) {
      pList = pList->next;
      tokentmp = pList;
      if (tokentmp != NULL && tokentmp->type == RightParen) {
        pList = pList->next;
        tokentmp = pList;
        if (tokentmp != NULL && tokentmp->type == LeftBrace) {
          pList = pList->next;
          tokentmp = pList;
          ASTNodeList child = Body();
          if (child != NULL) {
            addASTNode(node, child);
            if (pList != NULL && pList->type == RightBrace) {
              pList = pList->next;
            } else {
              printf("error:Lack of RightBrace\n");
              exit(0);
            }
          }
        } else {
          printf("error:Lack of LeftBrace\n");
          exit(0);
        }
      } else {
        printf("error:Lack of RightParen\n");
        exit(0);
      }
    } else {
      printf("error:Lack of LeftParen\n");
      exit(0);
    }
  }
  return node;
}
