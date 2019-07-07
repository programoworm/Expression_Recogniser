#include<stdio.h>
#include"colors.h"
#include<string.h>
#include<stdlib.h>

#define NONE -1

enum Reserved {INT=10,SUM,SUB,MULT,DIV,LPAREN,RPAREN};

typedef struct token{
	int type;
	int value;
}Token;

int pos=0;

void none(Token token){
	token.type=NONE;
	token.value=NONE;
}

void synerror(){
	printf(RED"[info]"RESET"Syntax/Semantic Error\n");
	exit(0);
}


void match(int current_token_type,int type){
	if(current_token_type==type){
		pos++;
		return;
	}
	printf(RED"[Interpretation error]"RESET"This is parse error!!");
	//exit(0);
	//synerror();
}

int integer(char *str){
	int i=0;	
	char num[10];
	num[i++]=str[pos++];
	while(str[pos]!=EOF && str[pos]>='0' && str[pos]<='9')
		num[i++]=str[pos++];
	pos--;
	num[i]='\0';
	//printf("%s\n",num);
	return atoi(num);
}

Token scanner(char *str,char ch){
	Token token;
	//printf("ch=%c\n",ch);
	while(ch== ' '){
		ch=str[++pos];
		//printf("%c\n",ch);
	}
	if(ch== '\n'||ch==EOF){
		token.type=EOF;
		token.value=EOF;
		return token;
		//printf("%c\n",ch);
	}
	if(ch=='('){
		token.type=LPAREN;
		token.value='(';
		return token;
	}
	if(ch==')'){
		token.type=RPAREN;
		token.value=')';
		return token;
	}
	if(ch>='0' && ch<='9'){
		token.type=INT;
		token.value=integer(str);
		return token;
	}
	if(ch=='+'){
		token.type=SUM;
		token.value='+';
		return token;
	}
	if(ch=='-'){
		token.type=SUB;
		token.value='-';
		return token;
	}
	if(ch=='*'){
		token.type=MULT;
		token.value='*';
		return token;
	}
	if(ch=='/'){
		token.type=DIV;
		token.value='/';
		return token;
	}
	printf(RED"[Interpretation error]"RESET"Scan error!! '%d' cannot be recognised as token!",ch);
	//exit(0);
	//synerror();
}

int vmachine(int left,Token op, int right){	
	if(op.type==DIV)
		return left/right;	
	if(op.type==MULT)
		return left*right;	
	if(op.type==SUM)
		return left+right;	
	if(op.type==SUB)
		return left-right;
}

/*int parser(char *str){
	int result;
	Token current_token,left,op,right;
	
	current_token=scanner(str,str[pos]);
	pos++;
	//printf("%d\n",current_token.value);
	left=current_token;
	match(left.type,INT);
	
	current_token=scanner(str,str[pos]);
	pos++;
	//printf("%c\n",current_token.value);
	op=current_token;
	switch(op.type){
		case SUM: match(op.type,SUM);
				break;
		case SUB: match(op.type,SUB);
				break;
		case MULT: match(op.type,MULT);
				break;
		case DIV: match(op.type,DIV);
				break;
	}

	current_token=scanner(str,str[pos]);
	pos++;
	right=current_token;
	match(right.type,INT);
	current_token=scanner(str,str[pos]);pos++;
	//pos++;
	result=term(current_token);
	//printf("%c\n",str[pos]);
	//if(str[pos]=='\n')
	//	printf("HI\n");
	if(str[pos]!=EOF && str[pos]!='\n'){
		//printf("Here is me!\n");
		current_token=scanner(str,str[pos]);pos++;
		while(str[pos-1]!=EOF && str[pos-1]!='\n' && (current_token.type==SUM || current_token.type==SUB || current_token.type==MULT || current_token.type==DIV)){
			op=current_token;//pos++;
			//printf("%c\n",str[pos]);
			current_token=scanner(str,str[pos]);pos++;
			//printf("OP=%c Token=%d\n",op.value,current_token.value);
			//printf("Result=%d\n",result);
			result=vmachine(result,op,term(current_token));
			current_token=scanner(str,str[pos]);pos++;
		}
	}
	return result;
	//return vmachine(left,op,right);
}*/
int expr(char *str,Token current_token);

int factor(char *str,Token current_token){
	if(current_token.type==LPAREN){
		pos++;
		current_token=scanner(str,str[pos]);
		int result=expr(str,current_token);
		current_token=scanner(str,str[pos]);
		//printf("type=%d\n",current_token.type);
		match(current_token.type,RPAREN);
		return result;
	}
	match(current_token.type,INT);
	return current_token.value;
}

int term(char *str,Token current_token){
	int result;
	Token op;
	result=factor(str,current_token);
	current_token=scanner(str,str[pos]);
	while(str[pos]!=EOF && str[pos]!='\n' && (current_token.type==MULT || current_token.type==DIV)){
		op=current_token;pos++;
		current_token=scanner(str,str[pos]);
		result=vmachine(result,op,factor(str,current_token));
		current_token=scanner(str,str[pos]);
	}
	return result;
}

int expr(char *str,Token current_token){
	Token op; int result;
	result=term(str,current_token);
	current_token=scanner(str,str[pos]);
	while(str[pos]!=EOF && str[pos]!='\n' && (current_token.type==SUM || current_token.type==SUB)){
		op=current_token;pos++;
		current_token=scanner(str,str[pos]);
		result=vmachine(result,op,term(str,current_token));
		current_token=scanner(str,str[pos]);
	}
	return result;
}

int parser(char *str){
	int result;
	Token current_token=scanner(str,str[pos]);
	result=expr(str,current_token);
	return result;
}
/*int next_token(char *str){
	//char ch;
	int pos=0;
	Token current_token;
	while(str[pos]!=EOF)//current
		scanner(str[pos],token,pos++);	
	token[pos]=EOF
	return expr(token);
}*/


int main(){
	char ch;
	char string[100];
	while(1){
		pos=0;
		printf(GREEN"Knudon>> "RESET);
	//ch=getchar();
	//printf("%c",ch);
	//while(ch=getchar()!='\n')
	//	printf("%c",ch);
	//	scanf("%s",string);
		fgets(string,100,stdin);
	//	printf("%s\n",string);
		string[strlen(string)]=EOF;
		printf("%d\n",parser(string));
	}
	//printf("%d",string[3]);
	//scanf("%c",&ch);
	//printf("%d\n",ch-48);
	return 0;
}
