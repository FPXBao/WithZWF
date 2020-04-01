#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define NULL -1

typedef struct {//栈
    char str[30];
    int top;
}Stack;

void initialStack(Stack *S){//初始化栈
    S->top=NULL;
}

int StackEmpty(Stack S){//栈的判空
    if(S.top==NULL) return 1;
    return 0;
}

int StackFull(Stack S){//栈的判满
    if(S.top==19) return 1;
    return 0;
}

void PushStack(Stack *S,char e){//入栈
    if(StackFull(*S)) return ;
    S->str[++S->top]=e;
}

char PopStack(Stack *S){//出栈
    if(StackEmpty(*S)) return 0;
    return S->str[S->top--];
}

int CharPriority(char ch){//符号优先级判断
    if(ch=='(') return 0;
    if(ch=='+'||ch=='-') return 1;
    if(ch=='*'||ch=='/') return 2;
    if(ch==')') return 3;
    return 0;
}

void turnRPNandCalculate(int *num,char *ch){//转逆波兰并计算结果
    int i,j=0;
    char e;
    int num1[30]; char ch1[30];
    for(i=0;i<30;i++){
        num1[i]=-1; ch1[i]=' ';
    }
    Stack *s;
    s=(Stack*)malloc(sizeof(Stack));
    initialStack(&s);
    FILE *fp;
    fp=fopen("Answers.txt","a");
    for(i=0;i<30;i++){
        if(num[i]!=-1&&ch[i]==' ') num1[j++]=num[i];
        else if(num[i]==-1&&ch[i]=='(') PushStack(s,ch[i]);
        else if(num[i]==-1&&ch[i]==')'){
            PushStack(s,ch[i]);
            while(s->top!=NULL){
                e=PopStack(s);
                if(e!='('&&e!=')') ch1[j++]=e;
                if(e=='(') break;
            }
        }
        else if(ch[i]!=' '&&ch[i]!='='){
            if(CharPriority(ch[i])<=CharPriority(s->str[s->top])){
                while(s->top!=NULL&&(CharPriority(ch[i])<=CharPriority(s->str[s->top]))){
                    e=PopStack(s);
                    if(e!='('&&e!=')') ch1[j++]=e;
                    if(e=='(') break;
                }
                PushStack(s,ch[i]);
            }
            else PushStack(s,ch[i]);
        }
    }
    while(s->top!=NULL){
        e=PopStack(s);
        ch1[j++]=e;
    }
    double x,y; double count[30];
    for(i=0,j=0;i<30;i++){
        if(num1[i]!=-1) count[j++]=num1[i];
        if(ch1[i]=='+'||ch1[i]=='-'||ch1[i]=='*'||ch1[i]=='/'){
            x=count[j-2]; y=count[j-1];
            j-=2;
            switch(ch1[i]){
                case '+':{count[j++]=x+y;break;}
                case '-':{count[j++]=x-y;break;}
                case '*':{count[j++]=x*y;break;}
                case '/':{count[j++]=x/y;break;}
                default: break;
            }
        }
    }
    fprintf(fp,"%f\n",count[0]);
    fclose(fp);
}

void getIntandChar(FILE *f,int *num,char *ch){//提取数值和运算符
    char c[99];
    int i,j,k,count=0; int tag=0; int LEN;
    fgets(c,99,f);
    j=0;
    LEN=strlen(c);
    c[LEN-1]='\0';
    for(i=0;i<30;i++){
        num[i]=-1;
        ch[i]=' ';
    }

    for(i=0,j=0;i<LEN;i++){//筛选数值和运算符
        if(c[i]>='0'&&c[i]<='9'){
            count*=10;
            count+=c[i]-'0';
            tag=1;
        }
        else if((c[i]=='+'||c[i]=='-'||c[i]=='*'||c[i]=='/'||c[i]=='('||c[i]==')'||c[i]=='\''||c[i]=='=')&&tag==1){
            num[j++]=count; count=0; ch[j++]=c[i]; tag=0;
        }
        else if((c[i]=='+'||c[i]=='-'||c[i]=='*'||c[i]=='/'||c[i]=='('||c[i]==')'||c[i]=='\''||c[i]=='=')&&tag==0)
            ch[j++]=c[i];
    }
}

void ChangeFraction(int *num,char *ch){//将真分数化为假分数
    int i,j;
    for(i=0;i<30;i++){
        if(ch[i]=='\''){
            num[i+1]=num[i-1]*num[i+3]+num[i+1];
            num[i-1]=-1;
            ch[i]=' ';
        }
    }
}

void CreateNum(int n,int r,FILE *f){//生成随机四则运算
    int i,j,k,x,y; int count; int num[999]; int t[7]; int tag=0,tag1=0; int flag;
    char c[4]={'+','-','*','/'}; char c1[2]={'(',')'};
    f=fopen("Exercises.txt","a");
    if(f==NULL) printf("ERROR\n");
    fclose(f);
    for(x=0;x<r;x++) num[x]=x;
    for(y=0;y<n;y++){
        f=fopen("Exercises.txt","a");
        for(i=0;i<8;i++){
            t[i]=num[rand()%r];
        }
        count=4;
        for(j=0,k=0;j<count;j++,k++){//4个运算数+3个运算符，若有括号count+1
            if(rand()%2==1&&j<3&&tag1==0){//随机生成0和1，1表示添加括号，0则非
                fprintf(f,"%c",c1[0]);
                tag1=1;
                flag=j;
            }

            if(rand()%2==1){//随机生成0和1，1表示抽取自然数，0表示抽取分数
                fprintf(f,"%d",t[k]); tag++;
            }else{
                if(t[k]>t[k+1]&&t[k]%t[k+1]!=0&&t[k+1]!=0){//检测是否需要化为真分数
                    fprintf(f,"%d'%d/%d",t[k]/t[k+1],t[k]%t[k+1],t[k+1]);
                    k+=2; tag++;
                }
                else if(t[k]<t[k+1]&&t[k]!=0){//检测是否为真分数
                    fprintf(f,"%d/%d",t[k],t[k+1]);
                    k+=2; tag++;
                }else{//若出现整除，则直接输出自然数
                    fprintf(f,"%d",t[k]);
                    k++; tag++;
                }
            }

            if(tag1!=0&&flag<j){
                fprintf(f,"%c",c1[1]);
                tag1=0;
            }

            if(tag%4!=0) fprintf(f,"%c",c[rand()%4]);//随机生成0到3之间的数，用以随机输出运算符
        }
        fprintf(f,"=\n");
        fclose(f);
    }
}

void checkAnswer(){//检查答案
    FILE *f,*fp,*fy;
    f=fopen("Answers.txt","r");
    fp=fopen("StuAnswers.txt","r");
    char a[50],b[50];
    for(int i=0;i<50;i++){
        a[i]=b[i]=' ';
    }
    int correct=0,wrong=0;
    fgets(a,99,f);
    fgets(b,99,fp);
    while(!feof(f)&&!feof(fp)){
        if(strcmp(a,b)==0) correct++;
        else wrong++;
        fgets(a,99,f);
        fgets(b,99,fp);
    }
    fy=fopen("Grade.txt","w");
    fprintf(fy,"Correct:%d\nWrong:%d",correct,wrong);
    fclose(f);
    fclose(fp);
    fclose(fy);
}

int main(){
    FILE *f,*fp;
    int n,r,i,j;
    int num[30]; char ch[30];
    srand(time(0));
    f=fopen("Exercises.txt","r+");
    printf("1.生成题目和答案   2.检查答案对错：");
    scanf("%d",&j);
    switch(j){
        case 1:{
            printf("输入生成四则运算的数目：");
            scanf("%d",&n);
            printf("输入数值的最大值：");
            scanf("%d",&r);
            CreateNum(n,r,f);
            for(i=0;i<n;i++){
                getIntandChar(f,num,ch);
                ChangeFraction(num,ch);
                turnRPNandCalculate(num,ch);
            }
            break;
        }
        case 2:{
            checkAnswer();
            break;
        }
    }
    fclose(f);
    return 0;
}
