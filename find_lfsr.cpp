#include<iostream>
#include<cmath>
#include<stdio.h>
#include<stdlib.h>
using namespace std;
int poly1[]={1,0,0,0,0,0,1}; // feedback polynomial of degree 7
int poly2[]={0,0,0,1,0,0,0,0,1}; // feedback polynomial of degree 9
int poly3[]={0,1,0,0,0,0,0,0,0,0,1};  // feedback polynomial of degree 11
int poly4[]={1,0,1,1,0,0,0,0,0,0,0,0,1};  // feedback polynomial of degree 13
int *arr1,*arr2,*arr3,*arr4;
int l1=7;                        // LENGTH OF LFSR-1
int l2=9;                       // LENGTH OF LFSR-2
int l3=11;                         // LENGTH OF LFSR-3
int l4=13;                   // LENGTH OF LFSR-4
int no_clk=2000;
void int_2_bin(int x,int n,int* &bit)
{
  for(int i=0;i<n;i++)
  bit[i]=(x>>i)&0x1;	
}
int LFSR(int length,int* &arr,int poly[])
{
           int i,fb;
	fb=0;
          for(i=0;i<length;i++)
	fb^=(poly[i]*arr[length-i-1]);
	for(i=0;i<length-1;i++)
          arr[i]=arr[i+1];
         arr[length-1]=fb;
         return (arr[0]);
}
int main()
{
	FILE *ifp,*ofp,*ofp1,*ofp2,*ofp3,*ofp4;
	int *init_st1,*init_st2,*init_st3,*init_st4;
	int *derived_init_st1,*derived_init_st2,*derived_init_st3,*derived_init_st4;
	int i,j,k=0,t,x1,x2,x3,x4,f;
	int cnt1=0,cnt2=0,cnt3=0,cnt4=0;
	char ch;
	int *z,st,st1,st3;
	/********************OPENING DATA FILES*****************/
	ifp=fopen("key_seq.txt","r");
	if(ifp==NULL)
	{
		printf("\n Unable to open input file.\n");
		exit(0);
	}
	ofp=fopen("corr_lfsr2.txt","w");
	if(ofp==NULL)
	{
		printf("\n Unable to open output file.\n");
		exit(0);
	}
	
	/******************READING THE INPUT FILE**************/
	init_st1=new int[l1];
	derived_init_st1=new int[l1];
	arr1=new int[l1];
	init_st2=new int[l2];
	derived_init_st2=new int[l2];
	arr2=new int[l2];
	init_st3=new int[l3];
	derived_init_st3=new int[l3];
	arr3=new int[l3];
	init_st4=new int[l4];
	derived_init_st4=new int[l4];
	arr4=new int[l4];
	z=new int[no_clk];
	for(i=0;i<no_clk;i++)
	{
		fscanf(ifp,"%c",&ch);
		z[i]=ch-48;
	}
	
	/*********************FINDING INITIAL STATE OF LFSR-2**************/
	for(st=0;st<(1<<l2);st++)
{
		for(i=0;i<l2;i++)
		{
			arr2[i]=init_st2[i]=(st>>i)&0x1;
		}
	cnt2=0;
	for(t=0;t<no_clk;t++)
	{
		x2=LFSR(l2,arr2,poly2);
		if(z[t]==x2)
		cnt2++;
	}
	float match_prob=(float)cnt2/no_clk;
	if(match_prob>0.7 && match_prob<0.8)
	{
		fprintf(ofp,"\n");
		for(i=l2-1;i>=0;i--)
		{
			derived_init_st2[i]=init_st2[i];
			fprintf(ofp,"%d",init_st2[i]);
		}
		fprintf(ofp,"   %d:\t p(z(t)=x2(t))=%0.2f",st,match_prob);
	}
}
/*********************FINDING INITIAL STATE OF LFSR-4**************/
	for(st=0;st<(1<<l4);st++)
	{
	for(i=0;i<l4;i++)
	{
		arr4[i]=init_st4[i]=(st>>i)&0x1;
	}	
	cnt4=0;
	for(t=0;t<no_clk;t++)
	{
		x4=LFSR(l4,arr4,poly4);
		if(z[t]==x4)
		cnt4++;
	}
	float match_prob=(float)cnt4/no_clk;
	if(match_prob>0.55 && match_prob<0.70)
	{
		fprintf(ofp,"\n");
		for(i=l4-1;i>=0;i--)
		{
			derived_init_st4[i]=init_st4[i];
			fprintf(ofp,"%d",init_st4[i]);
		}
		fprintf(ofp,"   %d:\t p(z(t)=x4(t))=%0.2f",st,match_prob);
	}
}
fprintf(ofp,"\n");
/******************FINDING INITIAL STATES OF LFSR-1 & LFSR-3 THROUGH EXHAUSTIVE RESEARCH***************/
for(st1=1;st1<(1<<l1);st1++)
{
	for(i=0;i<l1;i++)
	{
		arr1[i]=init_st1[i]=(st1>>i)&0x1;
	}
for(st3=1;st3<(1<<l3);st3++)
{
	for(i=0;i<l3;i++)
	{
		arr3[i]=init_st3[i]=(st3>>i)&0x1;
	}
	
for(i=l1-1;i>=0;i--)
	arr1[i]=init_st1[i];	
for(i=l2-1;i>=0;i--)
	arr2[i]=derived_init_st2[i];
for(i=l4-1;i>=0;i--)
	arr4[i]=derived_init_st4[i];
for(k=0;k<no_clk;k++)
	{
		x1=LFSR(l1,arr1,poly1);
		x2=LFSR(l2,arr2,poly2);
		x3=LFSR(l3,arr3,poly3);
		x4=LFSR(l4,arr4,poly4);
		f=1^(x1*x2*x3)^(x1*x3)^(x1*x4)^(x3*x4)^(x1)^(x4)^(x2)^(x3);	
		if(f!=z[k])
		break;		
    }
//    fprintf(ofp,"%d",k);
	if(k==no_clk)
    {
    	fprintf(ofp,"\n****************EUREKA....INITIAL STATES OF LFSR-1 & LFSR-3 FOUND!!!**************************\n");
    	fprintf(ofp,"Initial state for LFSR-1 :");
    	for(i=l1-1;i>=0;i--)
    	{
    		derived_init_st1[i]=init_st1[i];
    		fprintf(ofp,"%d",init_st1[i]);
    	}
    	fprintf(ofp,"\nInitial state for LFSR-3 :");
    	for(i=l3-1;i>=0;i--)
    	{
    		derived_init_st4[i]=init_st4[i];
    		fprintf(ofp,"%d",init_st3[i]);
    	}
    }
  }
}
//*********************************************************//
 fclose(ifp);
 fclose(ofp);
delete[] init_st1;
delete[] init_st2;
delete[] init_st3;
delete[] init_st4;
delete[] arr1;
delete[] arr2;
delete[] arr3;
delete[] arr4;
delete[] z;
return 0;
}

