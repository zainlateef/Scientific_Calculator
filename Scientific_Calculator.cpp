//
//  main.cpp
//  TheSciCalc
//
//  Created by Zain Lateef on 10/26/16.
//  Copyright © 2016 Zain Lateef. All rights reserved.
//




#define LP 40
#define RP 41
#define MULT 42
#define PLUS 43
#define SUB 45
#define DEC 46
#define DIVI 47
#define EXP 94
#define PI 3.14169
#define E 2.718






#include <iostream>
#include <cmath>
#include <unordered_map>
#include <string>
using namespace std;




struct node
{
    string part;
    char op;
    node * next=NULL;
    int precedence=-1;
};
node * top_op_stack=NULL;
node * first_in_line=NULL;
node * last_in_line=NULL;
node * tempbox=NULL;




void push_queue (string input);
void push_op_stack (char oper);
void push_queue (string input);
int grabnumber(string str,int i);
int skipwhitespace(string str,int i);
char pop_op_stack ();


unordered_map<string, double> igotthekeyz;
bool divzero=false;
bool bad_var=false;


//THE OP STACK OPERATIONS----------------------------------------------------------------------------------------
void push_op_stack (char oper)
{
    
    
    if(top_op_stack!=NULL)
    {
        int holding=(int)oper;
        int topofstack=(int)top_op_stack->op;
        char temp;
        
        while(((holding==PLUS||holding==SUB)&&(topofstack==PLUS||topofstack==SUB||topofstack==MULT||topofstack==DIVI||topofstack==EXP))&&top_op_stack!=NULL)
        {
            temp=pop_op_stack();
            string str_temp(1,temp);
            push_queue(str_temp);
            if(top_op_stack!=NULL)
                topofstack=(int)top_op_stack->op;
        }
        
        while(((holding==MULT||holding==DIVI)&&(topofstack==MULT||topofstack==DIVI||topofstack==EXP))&&top_op_stack!=NULL)
        {
            
            temp=pop_op_stack();
            string str_temp(1,temp);
            push_queue(str_temp);
            if(top_op_stack!=NULL)
                topofstack=(int)top_op_stack->op;
            
        }
        
        if(holding==RP)
        {
            
            while(top_op_stack->op!='(')
            {
                temp=pop_op_stack();
                string str_temp(1,temp);
                push_queue(str_temp);
                
            }
            //for LP
            temp=pop_op_stack();
            
            if(top_op_stack!=NULL)
                if(top_op_stack->op=='s'||top_op_stack->op=='l'||top_op_stack->op=='c')
                {
                    temp=pop_op_stack();
                    string str_temp(1,temp);
                    push_queue(str_temp);
                    
                }
            
        }
    }
    
    
    if((int)oper!=RP)
    {
        node * box = new node;
        box->op=oper;
        if(top_op_stack==NULL)
            box->next=NULL;
        else
            box->next=top_op_stack;
        top_op_stack=box;
    }
    
}












char pop_op_stack ()
{
    char temp;
    if(top_op_stack==NULL)
    {
        return '\0';
    }
    else
    {
        tempbox=top_op_stack;
        temp=top_op_stack->op;
        top_op_stack=top_op_stack->next;
        delete tempbox;
        return temp;
    }
}
//----------------------------------------------------------------------------------------




//THE QUEUE OPERATIONS----------------------------------------------------------------------------------------
int size=0;
void push_queue (string input)
{
    node * box= new node;
    box->part=input;
    if(first_in_line==NULL)
    {
        first_in_line=box;
        last_in_line=box;
    }
    else
    {
        last_in_line->next=box;
        last_in_line=box;
    }
    size++;
    
}




string pop_queue()
{
    if(first_in_line==NULL)
    {
        return "empty";
    }
    string temp;
    tempbox=first_in_line;
    temp=first_in_line->part;
    first_in_line=first_in_line->next;
    delete tempbox;
    return temp;
}
//THE QUEUE OPERATIONS----------------------------------------------------------------------------------------




int grabnumber(string str,int i)
{
    int start_index=i;
    int length=0;
    int ascii=(int)str[i];
    while((ascii>47 && ascii<58) || ascii==46)
    {
        length++;
        i++;
        ascii=(int)str[i];
    }
    string number=str.substr(start_index,length);
    push_queue(number);
    
    return i;
    
}








//ORGANIZING--------------------------------------------------------------------------------------------------
void organize(string str,int i)
{
    
    int beginning;
    int length=0;
    int ascii;
    int arrlen=str.length();
    string funccheck;
    
    
    for(;i<arrlen;)
    {
        i=skipwhitespace(str,i);
        
        ascii=(int)str[i];
        if((ascii>47 && ascii<58) || ascii==46)// digit
        {
            
            i=grabnumber(str,i);
            i=skipwhitespace(str,i);
        }
        
        ascii=(int)str[i];
        
        if(ascii==MULT||ascii==PLUS||ascii==SUB||ascii==DIVI||ascii==LP||ascii==RP||ascii==EXP)
        {
            push_op_stack(str[i]);
            i++;
            i=skipwhitespace(str,i);
        }
        
        
        funccheck=str.substr(i,3);
        if(funccheck=="log")
        {
            push_op_stack('l');
            i=i+3;
            i=skipwhitespace(str,i);
        }
        if(funccheck=="sin")
        {
            push_op_stack('s');
            i=i+3;
            i=skipwhitespace(str,i);
        }
        if(funccheck=="cos")
        {
            push_op_stack('c');
            i=i+3;
            i=skipwhitespace(str,i);
        }
        
        ascii=(int)str[i];
        
        if((ascii>64 && ascii<91)||(ascii>96 && ascii<123))//variables
        {
            beginning=i;
            length=0;
            
            while((ascii>64 && ascii<91)||(ascii>96 && ascii<123))
            {
                
                i++;
                length++;
                
                ascii=(int)str[i];
            }
            
            
            string variable=str.substr(beginning,length);
            
            if(igotthekeyz.find(variable)==igotthekeyz.end())
            {
                cout<<"ERROR:"<<variable<<" is an undeclared variable"<<endl;
                bad_var=true;
                break;
            }
            else
            {
                double caster=igotthekeyz[variable];
                string transport=to_string(caster);
                if(caster<0)
                {
                    push_op_stack('(');
                    push_queue("0");
                    push_op_stack('-');
                    int leng=transport.length()-1;
                    transport=transport.substr(1,leng);
                    push_queue(transport);
                    push_op_stack(')');
                
                
                }
                else
                push_queue(transport);
            }
            
        }
        
    }
    
    char temp;
    while(top_op_stack!=NULL)
    {
        temp=pop_op_stack();
        string str_temp(1,temp);
        push_queue(str_temp);
    }
    
    
    
    
    
    
}
//EXECUTE--------------------------------------------------------------------------------------------------


double execute()
{
    double array[size];
    double temp,op1,op2,answer;
    string holder;
    int i=0,counter=0;
    while(counter<size)
    {
        
        holder=pop_queue();
        counter++;
        
        if(holder[0]>47&&holder[0]<58)//if the first character is a digit
        {
            temp=atof(holder.c_str());
            array[i]=temp;
            i++;
        }
        else if(holder[0]=='*')
        {
            op2=array[i-1];
            op1=array[i-2];
            answer=op1*op2;
            array[i]=0;
            array[i-1]=0;
            i=i-2;
            
            array[i]=answer;
            i++;
            
        }
        else if(holder[0]=='/')
        {
            op2=array[i-1];
            op1=array[i-2];
            if(op2==0)
            {
                cout<<"ERROR: Division by Zero during the computation of "<<op1<<"/"<<op2<<endl;
                divzero=true;
            }
            answer=op1/op2;
            array[i]=0;
            array[i-1]=0;
            i=i-2;
            
            array[i]=answer;
            i++;
            
        }
        else if(holder[0]=='+')
        {
            op2=array[i-1];
            op1=array[i-2];
            answer=op1+op2;
            array[i]=0;
            array[i-1]=0;
            i=i-2;
            
            array[i]=answer;
            i++;
            
        }
        else if(holder[0]=='-')
        {
            op2=array[i-1];
            op1=array[i-2];
            answer=op1-op2;
            array[i]=0;
            array[i-1]=0;
            i=i-2;
            
            array[i]=answer;
            i++;
            
        }
        else if(holder[0]=='^')
        {
            op2=array[i-1];
            op1=array[i-2];
            answer=pow(op1,op2);
            array[i]=0;
            array[i-1]=0;
            i=i-2;
            
            array[i]=answer;
            i++;
            
        }
        //separate cases
        else if(holder[0]=='s')
        {
            op1=array[i-1];
            answer=sin(op1);
            array[i]=0;
            i=i-1;
            
            array[i]=answer;
            i++;
            
        }
        
        else if(holder[0]=='l')
        {
            op1=array[i-1];
            answer=log(op1);
            array[i]=0;
            i=i-1;
            
            array[i]=answer;
            i++;
            
        }
        else if(holder[0]=='c')
        {
            op1=array[i-1];
            answer=cos(op1);
            array[i]=0;
            i=i-1;
            
            array[i]=answer;
            i++;
            
        }
        
        
        
    }
    
    
    
    
    return array[0];
    
}


//EXECUTE--------------------------------------------------------------------------------------------------
void declarevar(string str,int i)
{
    int beginning=i;
    int length=0;
    double answer;
    while(str[i]!='='&& str[i]!=' ')
    {
        i++;
        length++;
    }
    string variable=str.substr(beginning,length);
    if(variable=="Pi"||variable=="e"||variable=="sin"||variable=="cos"||variable=="log"||variable=="let"||variable=="quit")
    {
        cout<<"Cannot use this variable name";
    }
    else
    {
        i=skipwhitespace(str, i);
        i++;
        organize(str,i);
        answer=execute();
        
        
        if(igotthekeyz.find(variable)==igotthekeyz.end())
        {
            igotthekeyz[variable]=answer;
            
        }
        else //redefine
        {
            igotthekeyz.erase(variable);
            igotthekeyz[variable]=answer;
        }
    }
}


//DECLARATION--------------------------------------------------------------------------------------------------




//MISC--------------------------------------------------------------------------------------------------
int skipwhitespace(string str,int i)
{
    while(str[i]==' ')
    {
        i++;
    }
    return i;
}








int main(int argc, const char * argv[])
{
    double answer;
    string str="";
    int i=0,ascii;
    
    igotthekeyz["Pi"]=PI;
    igotthekeyz["e"]=E;
    
    while(str!="quit")
    {
        getline(cin,str);
        
        if(str!="quit")
        {
            
            i=skipwhitespace(str, i);
            
            ascii=(int)str[i];
            
            if(str[i]=='l' && str[i+1]=='e' && str[i+2]=='t' && str[i+3]==' ')
            {
                
                i=i+3;
                
                i=skipwhitespace(str,i);
                declarevar(str,i);//sends i to be at postion 0 of string x=1 so at x
            }
            else
            {
                organize(str,0);
                answer=execute();
                
                if(divzero)
                {}
                else if (bad_var)
                {}
                else
                    cout<<answer<<endl;
                divzero=false;
                bad_var=false;
                
            }
            
            
            
            
            i=0;
        }
        
    }///quit
    
}
