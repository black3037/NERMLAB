#ifndef _FLOAT2STRING_H
#define _FLOAT2STRING_H

#include <stdio.h>
#include <string.h>

#include<stdio.h>
#include<stdlib.h>
 
char *iascii(int);
 
char* fascii(float i)         /* Function to convert a floating point number into a stream of ASCII characters */
{
    char *s;int j,k;
    float a,b,c;
    j=i;
    s=(char*)malloc(10*sizeof(char));
    s=iascii(j);
    printf("%s.",s);
    i=i-j;
     
    for(a=0,c=0.1,k=1;(b=(i-a))!=0;a+=c)
    {
        if(b<0)
        {
            b=i+a;
            k++; a=c/10; c=a;
        }
        if(a==1)
        { a=c/10;c=a;}
    }
     
    for(;k>0;k--)
    {
        i=10*i;
    }
    j=i;
    s=iascii(j);
    return s;
}
 
char* iascii(int i)              /* Function to convert an integer into a stream of ASCII charcters */
{
    char *s;
    int j,k;
    s=(char*)malloc(10*sizeof(char));
    for(j=0;i!=0;j++)
    {
        *s++=(i%10)+48;
        i=i/10;
    }*s='\0';
     
    for(k=j;k>0;k--,s--);
    j--;
    for(i=0;(j-i)>0;j--,i++)   
    {
        char c;         /* Loop to reverse the string s */
        c=s[i];
        s[i]=s[j];
        s[j]=c;
    }
 
    return s;
}
 
#endif
