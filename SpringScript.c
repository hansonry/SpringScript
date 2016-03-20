#include "SpringScript.h"
#include "ConfigParser.h"





int SpringScript_Run(Scanner_T * input, FILE * output)
{
   int result;
   ConfigParser_T config;
   result = 1;

   ConfigParser_Init(&config, input);
   fprintf(output, "[GAME]\n{\n");
   


   fprintf(output, "}\n");
   ConfigParser_Destory(&config);
   return result;
}


