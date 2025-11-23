
#include <stdio.h>
#include <fcntl.h>

#define KEYFILE "CA.key"


int main(int argc,char **argv)
{

int CreateKey(char *);
int ShowReg(int);

int h;

if(argc!=2)
	{
	printf("Usage %s <name> \n",argv[0]);
	return(0);
	}

h=open(KEYFILE,O_RDONLY);
if(h>0) 
	{
	ShowReg(h);
	close(h);
	return(0);
	}

else CreateKey(argv[1]);
return(0);
}

int ShowReg(int h)
{

int i;
unsigned char x;
int cs;
int l;
char user[50];

cs=0;
for(i=0;i<200;i++)
	{
	read(h,&x,1);
	cs=cs+x;
	}

read(h,&x,1);
cs=cs%256;
if(x!=cs) return(-2);

read(h,&x,1);
l=x;

cs=0;
for(i=0;i<l;i++)
	{
	read(h,&x,1);
	cs=cs+x;
	user[i]=x-85-i;
	}	

user[l]=0;
read(h,&x,1);
cs=cs%256;
if(cs!=x) return(-3);

cs=0;
for(i=0;i<200;i++)
	{
	read(h,&x,1);
	cs=cs+x;
	}

read(h,&x,1);
cs=cs%256;
if(x!=cs) return(-2);

printf("user=%s\n",user);
return(0);
}

int CreateKey(char *user)
{
int h;
int i;
int cs;
unsigned char x;

h=open(KEYFILE,O_WRONLY|O_CREAT);
if(h<0)
	{
	printf("unable to create key file\n");
	return(-1);
	}

srand(VBeamPos());

cs=0;

for(i=0;i<200;i++)
	{
	x=rand()%256;
	cs=cs+x;
	write(h,&x,1);
	}

cs=cs%256;
x=cs;
write(h,&x,1);

x=strlen(user);
write(h,&x,1);

cs=0;

for(i=0;i<strlen(user);i++)
	{
	x=user[i]+85+i;
	write(h,&x,1);
	cs=cs+x;
	}

cs=cs%256;
x=cs;
write(h,&x,1);

cs=0;

for(i=0;i<200;i++)
	{
	x=rand()%256;
	cs=cs+x;
	write(h,&x,1);
	}

cs=cs%256;
x=cs;
write(h,&x,1);
close(h);
return(0);
}


