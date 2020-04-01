#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define NULL -1

typedef struct {//ջ
    char str[30];
    int top;
}Stack;

void initialStack(Stack *S){//��ʼ��ջ
    S->top=NULL;
}

int StackEmpty(Stack S){//ջ���п�
    if(S.top==NULL) return 1;
    return 0;
}

int StackFull(Stack S){//ջ������
    if(S.top==19) return 1;
    return 0;
}

void PushStack(Stack *S,char e){//��ջ
    if(StackFull(*S)) return ;
    S->str[++S->top]=e;
}

char PopStack(Stack *S){//��ջ
    if(StackEmpty(*S)) return 0;
    return S->str[S->top--];
}

int CharPriority(char ch){//�������ȼ��ж�
    if(ch=='(') return 0;
    if(ch=='+'||ch=='-') return 1;
    if(ch=='*'||ch=='/') return 2;
    if(ch==')') return 3;
    return 0;
}

void turnRPNandCalculate(int *num,char *ch){//ת�沨����������
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

void getIntandChar(FILE *f,int *num,char *ch){//��ȡ��ֵ�������
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

    for(i=0,j=0;i<LEN;i++){//ɸѡ��ֵ�������
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

void ChangeFraction(int *num,char *ch){//���������Ϊ�ٷ���
    int i,j;
    for(i=0;i<30;i++){
        if(ch[i]=='\''){
            num[i+1]=num[i-1]*num[i+3]+num[i+1];
            num[i-1]=-1;
            ch[i]=' ';
        }
    }
}

void CreateNum(int n,int r,FILE *f){//���������������
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
        for(j=0,k=0;j<count;j++,k++){//4��������+3�����������������count+1
            if(rand()%2==1&&j<3&&tag1==0){//�������0��1��1��ʾ������ţ�0���
                fprintf(f,"%c",c1[0]);
                tag1=1;
                flag=j;
            }

            if(rand()%2==1){//�������0��1��1��ʾ��ȡ��Ȼ����0��ʾ��ȡ����
                fprintf(f,"%d",t[k]); tag++;
            }else{
                if(t[k]>t[k+1]&&t[k]%t[k+1]!=0&&t[k+1]!=0){//����Ƿ���Ҫ��Ϊ�����
                    fprintf(f,"%d'%d/%d",t[k]/t[k+1],t[k]%t[k+1],t[k+1]);
                    k+=2; tag++;
                }
                else if(t[k]<t[k+1]&&t[k]!=0){//����Ƿ�Ϊ�����
                    fprintf(f,"%d/%d",t[k],t[k+1]);
                    k+=2; tag++;
                }else{//��������������ֱ�������Ȼ��
                    fprintf(f,"%d",t[k]);
                    k++; tag++;
                }
            }

            if(tag1!=0&&flag<j){
                fprintf(f,"%c",c1[1]);
                tag1=0;
            }

            if(tag%4!=0) fprintf(f,"%c",c[rand()%4]);//�������0��3֮����������������������
        }
        fprintf(f,"=\n");
        fclose(f);
    }
}

void checkAnswer(){//����
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
    printf("1.������Ŀ�ʹ�   2.���𰸶Դ�");
    scanf("%d",&j);
    switch(j){
        case 1:{
            printf("�������������������Ŀ��");
            scanf("%d",&n);
            printf("������ֵ�����ֵ��");
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
