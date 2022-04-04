#include<iostream>
using namespace std;
int E[1000][1000];
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
   //int M_i_j = m_i_j;
   //int C_i_j = c_i_j;
   E[i][j] = 0;
   for(int k=i;k<=j;k++)
   {
    E[i][j] += (points[k][1] - m_i_j*points[k][0] - c_i_j)*(points[k][1] - m_i_j*points[k][0] - c_i_j);
   }
  }
 }

}