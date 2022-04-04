#include<iostream>
using namespace std;
int min(int a, int b)
{
 if(a>b)
 {
  return b;
 }
 return a;
}
int max(int a, int b)
{
 if(a<b)
 {
  return b;
 }
 return a;
}
//-1 t be replaced by infinity
int E[1000][1000];
int F[1000][1000];
int I[1000][1000];
int A[1000][1000];
int X[1000][1000];
int B[1000][1000];
int n;
int T = n/7;
int piecewise(int points[][2])
{
 for(int i=0;i<n;i++)
 {
  E[i][i] = -1;
  for(int j=i+1;j<n;j++)
  {
   int xy=0,x=0,y=0,x_2=0;
   for(int k=i;k<=j;k++)
   {
    xy+=points[k][0]*points[k][1];
   }
   for(int k=i;k<=j;k++)
   {
    x+=points[k][0];
   }
   for(int k=i;k<=j;k++)
   {
    y+=points[k][1];
   }
   for(int k=i;k<=j;k++)
   {
    x_2+=points[k][0]*points[k][0];
   }
   m_i_j = ((j-i+1)*xy - x*y)/((j-i+1)*x_2 - x*x);
   c_i_j = (y - m_i_j*x)/(j-i+1);
   A[i][j] = m_i_j;
   B[i][j] = c_i_j;
   E[i][j] = 0;
   for(int k=i;k<=j;k++)
   {
    E[i][j] += (points[k][1] - m_i_j*points[k][0] - c_i_j)*(points[k][1] - m_i_j*points[k][0] - c_i_j);
   }
  }
 }
 for(int j=0;j<n;j++)
 {
  for(int t=j;t<=T;t++)
  {
   F[j][t] = -1;
   I[j][t] = 0;
   X[j][t] = 0;
  }
  F[j][0] = E[0][j];
  I[j][0] = 1;
  X[j][1] = 0;
 }
 for(int t=1;t<=min(j-1,T);t++)
 {
  F[j][t] = -1;
  I[j][t] = 0;
  X[j][0] = 0;
  for(int i=t;t<=j-1;i++)
  {
   int k = I[i][t-1];
   if(k!=0 && A[k][i] != A[i][j])
   {
       int x = (B[k][i] - B[i][j])/(A[i][j] - A[k][i]);
       if(x >= points[max(0,i)][0] && x<= points[min(n-1,i)][0] && F[j][t] > F[i][t-1] + E[i][j])
       {
        F[j][t] = F[i][t-1] + E[i][j];
        I[j][t] = i;
        X[j][t] = x;
       }
   }
  }
 }
}