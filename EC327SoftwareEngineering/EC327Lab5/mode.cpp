#include "mode.h"
#include <bits/stdc++.h>
using namespace std;

void findModes(int* input, int size, int &frequency, int* result, int &result_count)
{
  int count = 0;
  int bestcount = 0;
  sort(input, input + size); 
  for(int i = 0; i < size; i++)
  {
    if (input[i] == input[i - 1])
    {
      count++;
    }
    else if(count >= bestcount)
    {
      bestcount = count;
      count = 1;
    }
    else if(count < bestcount)
    {
      count = 1;
    }  
  }

  int j = 0;
  count = 0;
  for(int i = 0; i <= size; i++)
  {
    if (input[i] == input[i - 1])
    {
      count++;
    }
    else if(count == bestcount)
    {
      result[j] = input[i - 1];
      count = 1;
      j++;
    }
    else// if(count < bestcount)
    {
      count = 1;
    } 
  }
  result_count = j;
  frequency = bestcount;
}

  /*sort(input, input + 10)
  
  int temp = 0
  for(int i = 0; i < size; i++)
  {
    for(int j = 0, j < size; j++)
    {	
      if(*(input + j) == *(input + i) && i != j)
      {

	
  */
/*
  case -10:
	
      case -9:
      case -8:
      case -7:
      case -6:
      case -5:
      case -4:
      case -3:
      case -2:
      case -1:
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
      case 10:*/

/*if (input[i] == input[i - 1])
    {
      count++;
    }
    else if(count >= bestcount)
    {
      bestcount = count;
      count = 1;
    }
    else if(count < bestcount)
    {
      count = 1;
      }     	*/
