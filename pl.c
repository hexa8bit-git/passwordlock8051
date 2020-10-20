#include<reg51.h>
#include<string.h>
#define lcdport P2
sbit rs=P3^4;
sbit rw=P3^5;
sbit en=P3^6;
sbit m1=P3^0;
sbit m2=P3^1;
sbit r1=P1^0;
sbit r2=P1^1;
sbit r3=P1^2;
sbit r4=P1^3;
sbit c1=P1^4;
sbit c2=P1^5;
sbit c3=P1^6;
sbit buzzer=P1^7;
char uid[]="16377";
char master[]="*0000";
char id[5];
char ne[5];

void lcdint();
void user_id(char);
void lcdstring(char *);
void delay(int);
void lcdcmd(char);
void lcddata(char);
void check_id();
char scankey();
char check();
void door_open();
void door_close();
void sounder();
void forg_pass();
void main()
{
int n;
int count=0;
char key;
start:
P2=0x00;
P1=0xff;
lcdint();
lcdstring("System Is Locked");
lcdcmd(0xc0);
lcdstring("Enter Passcode");
delay(100);
lcdcmd(0x01);
lcdcmd(0x02);
lcdstring("Passcode:");
lcdcmd(0xc0);
n=0;
while(n<5)
{
key=check();
id[n]=key;
lcddata(key);
delay(100);
n++;
}
lcdcmd(0x01);
lcdcmd(0x02);
if(count==2){
forg_pass();
count=0;
}
else if(strcmp(uid,id)==0)
{
lcdstring("Passcode Matched");
delay(200);
door_open();
delay(600);
door_close();
lcdcmd(0x01);
lcdcmd(0x02);
count=0;
}
else
{
lcdstring("Try Again");
sounder();
count++;
delay(200);
lcdcmd(0x01);
lcdcmd(0x02);
}
goto start;
}
void sounder()
{
int i;
for(i=0;i<10;i++)
{
buzzer=1;
buzzer=0;
}
}
void door_open()
{
lcdcmd(0x01);
lcdcmd(0x02);
lcdstring("Opening The Door");
m1=1;
m2=0;
delay(400);
m1=0;
m2=0;
}
void door_close()
{
lcdcmd(0x01);
lcdcmd(0x02);
lcdstring("Closing The Door");
m1=1;
m2=0;
m1=0;
m2=1;
delay(400);
m1=0;
m2=0;
}
void forg_pass()
{
int n=0;
char key;
lcdcmd(0x01);
lcdcmd(0x02);
lcdstring("Too many attempts");
lcdcmd(0xc0);
lcdstring("Enter master key");
delay(100);
lcdcmd(0x01);
lcdcmd(0x02);
lcdstring("Master Key:");
lcdcmd(0xc0);
while(n<5)
{
key=check();
ne[n]=key;
lcddata(key);
delay(100);
n++;
}
if(strcmp(master,ne)==0)
{
lcdcmd(0x01);
lcdcmd(0x02);
lcdstring("Welcome Master");
delay(200);
door_open();
delay(600);
door_close();
lcdcmd(0x01);
lcdcmd(0x02);
delay(100);
}
}
char check()
{
char b='a';
while(b=='a')
{
b=scankey();
}
return b;
}
char scankey()
{
c1=c2=c3=1;
r1=r2=r3=r4=0;
r1=0;r2=r3=r4=1;
if(c1==0)
{
delay(2);
return '1';
}
if(c2==0)
{
delay(2);
return '2';
}
if(c3==0)
{
delay(2);
return '3';
}
r2=0;r1=r3=r4=1;
if(c1==0)
{
delay(2);
return '4';
}
if(c2==0)
{
delay(2);
return '5';
}
if(c3==0)
{
delay(2);
return '6';
}
r3=0;r1=r2=r4=1;
if(c1==0)
{
delay(2);
return '7';
}
if(c2==0)
{
delay(2);
return '8';
}
if(c3==0)
{
delay(2);
return '9';
}
r4=0;r1=r2=r3=1;
if(c1==0)
{
delay(2);
return '*';
}
if(c2==0)
{
delay(2);
return '0';
}
if(c3==0)
{
delay(2);
return '#';
}
return 'a';
}
void lcdstring(char *p)
{
while(*p!='\0')
{
lcddata(*p);
delay(10);
p++;
}
}
void lcdint()
{
lcdcmd(0x38);
delay(2);
lcdcmd(0x01);
delay(2);
lcdcmd(0x80);
delay(2);
lcdcmd(0x0e);
delay(2);
}
void delay(int x)
{
int i,j;
for(i=0;i<x;i++)
for(j=0;j<1275;j++);
}
void lcdcmd(char a)
{
lcdport=a;
rs=0;
rw=0;
en=1;
delay(1);
en=0;
}
void lcddata(char value)
{
lcdport=value;
rs=1;
rw=0;
en=1;
delay(1);
en=0;
}