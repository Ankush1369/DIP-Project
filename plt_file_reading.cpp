#include <fstream>
#include <iostream>
using namespace std;

int main()
{
  string line;
  ifstream b_file ( "20081023025304.plt" );
  ofstream output_file;
  output_file.open("dataset.csv");
  int i=0;
  while(getline(b_file, line))
  {
   if(i>6)
   {
    output_file << line << endl;
   }
   i++;
  }
}
