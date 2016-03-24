#include <stdio.h>
#include "SpringScript.h"




int main(int argc, char * args[])
{
   Scanner_T input;
   Scanner_InitFromFile(&input, "test.txt");
   SpringScript_Run(&input, stdout);

   Scanner_Destroy(&input);
   fprintf(stderr, "End\n");
   return 0;
}

