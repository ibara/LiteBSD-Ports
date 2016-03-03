#include<stdio.h>  // Krzysztof Gabis 2015
#include<stdlib.h> // Usage: ./bf program
int p[64*1024],d[512*1024],r,c,t,e;int main(int c,char*v[]){
if(c<2)exit(1);FILE*f=fopen(v[1],"r");while(f&&(c=getc(f))!=
EOF)p[r++]=c;r=0;while((c=p[r])){e=0;if(c=='>')t++;if(c=='<')
t--;if(c=='+')d[t]++;if(c=='-')d[t]--;if(c=='.')putc(d[t],
stdout);if(c==',')d[t]=getc(stdin);while(c=='['&&!d[t]){if(
p[r]=='[')e++;if(p[r]==']'&&e--==1)break;r++;}while(c==']'&&
d[t]){if(p[r]==']')e++;if(p[r]=='['&&e--==1)break;r--;}r++;}}
