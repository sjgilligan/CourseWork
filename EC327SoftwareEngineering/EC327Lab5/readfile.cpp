#include "readfile.h"
#include <fstream>

using namespace std;

void readFiletoArray(const char* filename, int* data)
{
  ifstream f;
  f.open(filename);
  
  for(int i = 0; i < 10; i++)
  {
    f >> *(data + i);
  }
  f.close();
}
  
