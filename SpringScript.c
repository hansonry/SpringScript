#include "SpringScript.h"
#include "ConfigParser.h"


static int SpringScript_WriteGame(CPValue_T * value, FILE * output)
{
   int index;
   CPValueObject_T * obj;
   CPObjectPair_T * pair;
   obj = &value->data.object;
   // Write Map Name
   index = ConfigParser_GetIndexOfKey(value, "map");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_String)
      {
         fprintf(output, "   MapName=%s;\n", pair->value->data.string.token->str);
      }
      else
      {
         fprintf(stderr, "Error: Expected 'map' to be a value\n");
      }
   }
   else
   {
      fprintf(stderr, "Error: Expected to find 'map'\n");
   }

   // Write Game Name
   index = ConfigParser_GetIndexOfKey(value, "mod");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_String)
      {
         fprintf(output, "   GameType=%s;\n", pair->value->data.string.token->str);
      }
      else
      {
         fprintf(stderr, "Error: Expected 'mod' to be a value\n");
      }
   }
   else
   {
      fprintf(stderr, "Error: Expected to find 'mod'\n");
   }

   // Write IsHost
   fprintf(output, "   IsHost=1;\n");

}



int SpringScript_Run(Scanner_T * input, FILE * output)
{
   int result;
   ConfigParser_T config;
   result = 1;

   fprintf(output, "[GAME]\n{\n");
   ConfigParser_Init(&config, input);

   if(config.root->type == e_CPVT_Object)
   {
      SpringScript_WriteGame(config.root, output);
   }
   else
   {
      fprintf(stderr, "Error: Expected root value to be an object\n");
   }



   ConfigParser_Destory(&config);
   fprintf(output, "}\n");
   return result;
}


