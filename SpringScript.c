#include "SpringScript.h"
#include "ConfigParser.h"
#include <stdlib.h>

typedef struct ScriptData_S   ScriptData_T;
typedef struct SSRoot_S       SSRoot_T;
typedef struct SSPlayer_S     SSPlayer_T;
typedef struct SSAI_S         SSAI_T;
typedef struct SSCommon_S     SSCommon_T;
typedef struct SSTeam_S       SSTeam_T;
typedef struct SSTeamMember_S SSTeamMember_T;
typedef struct SSTeamMemEle_S SSTeamMemEle_T;
typedef struct ColorPair_S    ColorPair_T;


struct SSCommon_S
{
   const char * name;
   const char * side;
   const char * color;
   int index;
 
};


struct SSPlayer_S
{
   SSCommon_T com;
   const char * password;
};

struct SSAI_S
{
   SSCommon_T com;
   const char * lib;
   SSPlayer_T * client;
};

struct SSTeamMemEle_S
{
   int is_ai;
   union
   {
      SSPlayer_T * player;
      SSAI_T     * ai;
   } data;
};

struct SSTeamMember_S
{
   SSTeamMemEle_T * ele_list;
   int              ele_count;
};

struct SSTeam_S
{
   SSTeamMember_T * member_list;
   int              member_count;
};

struct SSRoot_S
{
   const char * mod;
   const char * map;
   const char * host_addy;
   SSPlayer_T * host;
   SSPlayer_T * humans_list;
   int          humans_count;
   SSAI_T     * ai_list;
   int          ai_count;
   SSTeam_T   * team_list;
   int          team_count;


};




struct ScriptData_S
{
   int error;
};

struct ColorPair_S
{
   const char * name;
   const char * rgb_color;
};

static ColorPair_T ColorTable[] =
{
   { "AliceBlue",            "0.94 0.97 1.00" },
   { "AntiqueWhite",         "0.98 0.92 0.84" },
   { "Aqua",                 "0.00 1.00 1.00" },
   { "Aquamarine",           "0.50 1.00 0.83" },
   { "Azure",                "0.94 1.00 1.00" },
   { "Beige",                "0.96 0.96 0.86" },
   { "Bisque",               "1.00 0.89 0.77" },
   { "Black",                "0.00 0.00 0.00" },
   { "BlanchedAlmond",       "1.00 0.92 0.80" },
   { "Blue",                 "0.00 0.00 1.00" },
   { "BlueViolet",           "0.54 0.17 0.89" },
   { "Brown",                "0.65 0.16 0.16" },
   { "BurlyWood",            "0.87 0.72 0.53" },
   { "CadetBlue",            "0.37 0.62 0.63" },
   { "Chartreuse",           "0.50 1.00 0.00" },
   { "Chocolate",            "0.82 0.41 0.12" },
   { "Coral",                "1.00 0.50 0.31" },
   { "CornflowerBlue",       "0.39 0.58 0.93" },
   { "Cornsilk",             "1.00 0.97 0.86" },
   { "Crimson",              "0.86 0.08 0.24" },
   { "Cyan",                 "0.00 1.00 1.00" },
   { "DarkBlue",             "0.00 0.00 0.55" },
   { "DarkCyan",             "0.00 0.55 0.55" },
   { "DarkGoldenRod",        "0.72 0.53 0.04" },
   { "DarkGray",             "0.66 0.66 0.66" },
   { "DarkGrey",             "0.66 0.66 0.66" },
   { "DarkGreen",            "0.00 0.39 0.00" },
   { "DarkKhaki",            "0.74 0.72 0.42" },
   { "DarkMagenta",          "0.55 0.00 0.55" },
   { "DarkOliveGreen",       "0.33 0.42 0.18" },
   { "DarkOrange",           "1.00 0.55 0.00" },
   { "DarkOrchid",           "0.60 0.20 0.80" },
   { "DarkRed",              "0.55 0.00 0.00" },
   { "DarkSalmon",           "0.91 0.59 0.48" },
   { "DarkSeaGreen",         "0.56 0.74 0.56" },
   { "DarkSlateBlue",        "0.28 0.24 0.55" },
   { "DarkSlateGray",        "0.18 0.31 0.31" },
   { "DarkSlateGrey",        "0.18 0.31 0.31" },
   { "DarkTurquoise",        "0.00 0.81 0.82" },
   { "DarkViolet",           "0.58 0.00 0.83" },
   { "DeepPink",             "1.00 0.08 0.58" },
   { "DeepSkyBlue",          "0.00 0.75 1.00" },
   { "DimGray",              "0.41 0.41 0.41" },
   { "DodgerBlue",           "0.12 0.56 1.00" },
   { "FireBrick",            "0.70 0.13 0.13" },
   { "FloralWhite",          "1.00 0.98 0.94" },
   { "ForestGreen",          "0.13 0.55 0.13" },
   { "Fuchsia",              "1.00 0.00 1.00" },
   { "Gainsboro",            "0.86 0.86 0.86" },
   { "GhostWhite",           "0.97 0.97 1.00" },
   { "Gold",                 "1.00 0.84 0.00" },
   { "GoldenRod",            "0.85 0.65 0.13" },
   { "Gray",                 "0.50 0.50 0.50" },
   { "Green",                "0.00 0.50 0.00" },
   { "GreenYellow",          "0.68 1.00 0.18" },
   { "HoneyDew",             "0.94 1.00 0.94" },
   { "HotPink",              "1.00 0.41 0.71" },
   { "IndianRed",            "0.80 0.36 0.36" },
   { "Indigo",               "0.29 0.00 0.51" },
   { "Ivory",                "1.00 1.00 0.94" },
   { "Khaki",                "0.94 0.90 0.55" },
   { "Lavender",             "0.90 0.90 0.98" },
   { "LavenderBlush",        "1.00 0.94 0.96" },
   { "LawnGreen",            "0.49 0.99 0.00" },
   { "LemonChiffon",         "1.00 0.98 0.80" },
   { "LightBlue",            "0.68 0.85 0.90" },
   { "LightCoral",           "0.94 0.50 0.50" },
   { "LightCyan",            "0.88 1.00 1.00" },
   { "LightGoldenRodYellow", "0.98 0.98 0.82" },
   { "LightGray",            "0.83 0.83 0.83" },
   { "LightGrey",            "0.83 0.83 0.83" },
   { "LightGreen",           "0.56 0.93 0.56" },
   { "LightPink",            "1.00 0.71 0.76" },
   { "LightSalmon",          "1.00 0.63 0.48" },
   { "LightSeaGreen",        "0.13 0.70 0.67" },
   { "LightSkyBlue",         "0.53 0.81 0.98" },
   { "LightSlateGray",       "0.47 0.53 0.60" },
   { "LightSteelBlue",       "0.69 0.77 0.87" },
   { "LightYellow",          "1.00 1.00 0.88" },
   { "Lime",                 "0.00 1.00 0.00" },
   { "LimeGreen",            "0.20 0.80 0.20" },
   { "Linen",                "0.98 0.94 0.90" },
   { "Magenta",              "1.00 0.00 1.00" },
   { "Maroon",               "0.50 0.00 0.00" },
   { "MediumAquaMarine",     "0.40 0.80 0.67" },
   { "MediumBlue",           "0.00 0.00 0.80" },
   { "MediumOrchid",         "0.73 0.33 0.83" },
   { "MediumPurple",         "0.58 0.44 0.86" },
   { "MediumSeaGreen",       "0.24 0.70 0.44" },
   { "MediumSlateBlue",      "0.48 0.41 0.93" },
   { "MediumSpringGreen",    "0.00 0.98 0.60" },
   { "MediumTurquoise",      "0.28 0.82 0.80" },
   { "MediumVioletRed",      "0.78 0.08 0.52" },
   { "MidnightBlue",         "0.10 0.10 0.44" },
   { "MintCream",            "0.96 1.00 0.98" },
   { "MistyRose",            "1.00 0.89 0.88" },
   { "Moccasin",             "1.00 0.89 0.71" },
   { "NavajoWhite",          "1.00 0.87 0.68" },
   { "Navy",                 "0.00 0.00 0.50" },
   { "OldLace",              "0.99 0.96 0.90" },
   { "Olive",                "0.50 0.50 0.00" },
   { "OliveDrab",            "0.42 0.56 0.14" },
   { "Orange",               "1.00 0.65 0.00" },
   { "OrangeRed",            "1.00 0.27 0.00" },
   { "Orchid",               "0.85 0.44 0.84" },
   { "PaleGoldenRod",        "0.93 0.91 0.67" },
   { "PaleGreen",            "0.60 0.98 0.60" },
   { "PaleTurquoise",        "0.69 0.93 0.93" },
   { "PaleVioletRed",        "0.86 0.44 0.58" },
   { "PapayaWhip",           "1.00 0.94 0.84" },
   { "PeachPuff",            "1.00 0.85 0.73" },
   { "Peru",                 "0.80 0.52 0.25" },
   { "Pink",                 "1.00 0.75 0.80" },
   { "Plum",                 "0.87 0.63 0.87" },
   { "PowderBlue",           "0.69 0.88 0.90" },
   { "Purple",               "0.50 0.00 0.50" },
   { "RebeccaPurple",        "0.40 0.20 0.60" },
   { "Red",                  "1.00 0.00 0.00" },
   { "RosyBrown",            "0.74 0.56 0.56" },
   { "RoyalBlue",            "0.25 0.41 0.88" },
   { "SaddleBrown",          "0.55 0.27 0.07" },
   { "Salmon",               "0.98 0.50 0.45" },
   { "SandyBrown",           "0.96 0.64 0.38" },
   { "SeaGreen",             "0.18 0.55 0.34" },
   { "SeaShell",             "1.00 0.96 0.93" },
   { "Sienna",               "0.63 0.32 0.18" },
   { "Silver",               "0.75 0.75 0.75" },
   { "SkyBlue",              "0.53 0.81 0.92" },
   { "SlateBlue",            "0.42 0.35 0.80" },
   { "SlateGray",            "0.44 0.50 0.56" },
   { "SlateGrey",            "0.44 0.50 0.56" },
   { "Snow",                 "1.00 0.98 0.98" },
   { "SpringGreen",          "0.00 1.00 0.50" },
   { "SteelBlue",            "0.27 0.51 0.71" },
   { "Tan",                  "0.82 0.71 0.55" },
   { "Teal",                 "0.00 0.50 0.50" },
   { "Thistle",              "0.85 0.75 0.85" },
   { "Tomato",               "1.00 0.39 0.28" },
   { "Turquoise",            "0.25 0.88 0.82" },
   { "Violet",               "0.93 0.51 0.93" },
   { "Wheat",                "0.96 0.87 0.70" },
   { "White",                "1.00 1.00 1.00" },
   { "WhiteSmoke",           "0.96 0.96 0.96" },
   { "Yellow",               "1.00 1.00 0.00" },
   { "YellowGreen",          "0.60 0.80 0.20" },
   { NULL,                   NULL             }
};

static const char * ConvertColor(const char * color)
{
   int i;
   const char * result;
   const char * a, *b;
   char aa, bb;

   i = 0;
   result = NULL;
   while(ColorTable[i].name != NULL && result == NULL)
   {
      a = ColorTable[i].name;
      b = color;
      aa = 'a';
      bb = aa;
      while(*a != '\0' && *b != '\0' && aa == bb)
      {
         if(*a >= 'A' && *a <= 'Z')
         {
            aa = *a - ('A' - 'a');
         }
         else
         {
            aa = *a;
         }

         if(*b >= 'A' && *b <= 'Z')
         {
            bb = *b - ('A' - 'a');
         }
         else
         {
            bb = *b;
         }
         a++;
         b++;
         
      }
      if(aa == bb)
      {
         result = ColorTable[i].rgb_color;
      }
      i++;
   }
   if(result == NULL)
   {
      result = color;
   }
   return result;
}

static void SpringScript_Error(ScriptData_T * data, const char * function_name, const char * error_message)
{
   data->error ++;
   fprintf(stderr, "Error: %s: %s\n", function_name, error_message);
}

static void SprintScript_WriteCommonPlayer(SSCommon_T * com, int team_index, FILE * output)
{
   fprintf(output, "      Name=%s;\n", com->name);
   fprintf(output, "      Team=%i;\n", team_index);
}

static int SpringScript_WriteGame(SSRoot_T * root, FILE * output)
{
   int i, k;
   int player_count;
   int ai_count;
   int team_count;
   int team_lead;
   int result;
   SSTeamMember_T * member;
   SSCommon_T * com;
   SSPlayer_T * player;
   SSAI_T  * ai;
   result = 1;
   // Write Map Name
   fprintf(output, "   MapName=%s;\n", root->map);
   // Write Mod Name
   fprintf(output, "   GameType=%s;\n", root->mod);
   // Write IsHost
   fprintf(output, "   IsHost=1;\n");
   // Write HostIP
   fprintf(output, "   HostIP=%s;\n", root->host_addy);
   // Write MyPlayerName
   fprintf(output, "   MyPlayerName=%s;\n", root->host->com.name);

   // Write Players
   player_count = 0;
   ai_count = 0;
   team_count = 0;
   for(i = 0; i < root->team_count; i++)
   {
      for(k = 0; k < root->team_list[i].member_count; k++)
      {
         member = &root->team_list[i].member_list[k];

         if(member->ele_list[0].is_ai == 0)
         {
            player = member->ele_list[0].data.player;
            player->com.index = player_count;
            fprintf(output, "   [PLAYER%i]\n   {\n", player->com.index);
            SprintScript_WriteCommonPlayer(&player->com, team_count, output);
            fprintf(output, "      Password=%s;\n", player->password);
            fprintf(output, "      Spectator=0;\n");
            fprintf(output, "      IsFromDemo=0;\n");
            fprintf(output, "   }\n\n");
            player_count ++;
            team_count ++;
         }
         else
         {
            ai = member->ele_list[0].data.ai;
            ai->com.index = ai_count;
            fprintf(output, "   [AI%i]\n   {\n", ai->com.index);
            SprintScript_WriteCommonPlayer(&ai->com, team_count, output);
            fprintf(output, "      ShortName=%s;\n", ai->lib);
            fprintf(output, "      Host=%i;\n", ai->client->com.index);
            fprintf(output, "   }\n\n");
            ai_count ++;
            team_count ++;
         }
      }
   }

   // Write Teams
   team_count = 0;
   for(i = 0; i < root->team_count; i++)
   {
      for(k = 0; k < root->team_list[i].member_count; k++)
      {
         member = &root->team_list[i].member_list[k];

         if(member->ele_list[0].is_ai == 0)
         {
            com = &member->ele_list[0].data.player->com;
            team_lead = player->com.index;
         }
         else
         {
            com = &member->ele_list[0].data.ai->com;
            team_lead = member->ele_list[0].data.ai->client->com.index;
         }

            fprintf(output, "   [TEAM%i]\n   {\n", team_count);
            
            fprintf(output, "      TeamLeader=%i;\n", team_lead);
            fprintf(output, "      AllyTeam=%i;\n", i);
            fprintf(output, "      RgbColor=%s;\n", ConvertColor(com->color));
            fprintf(output, "      Side=%s;\n", com->side);
            fprintf(output, "      Advantage=0;\n");
            fprintf(output, "      IncomeMultiplier=1;\n");
            fprintf(output, "   }\n\n");
            team_count ++;
         
      }
   }
   // Write Ally Teams
   for(i = 0; i < root->team_count; i++)
   {
      fprintf(output, "   [ALLYTEAM%i]\n   {\n", i);
      fprintf(output, "      NumAllies=0;\n");
      fprintf(output, "   }\n\n");
   }


   return result;

}

static void SpringScript_ParseACom(ScriptData_T * data, SSRoot_T * root, CPValue_T * player, SSCommon_T * out_player)
{
   int index;
   CPValueObject_T * obj;
   CPObjectPair_T * pair;

   obj = &player->data.object;
   // Name
   index = ConfigParser_GetIndexOfKey(player, "name");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_String)
      {
         out_player->name = pair->value->data.string.token->str; 
      }
      else
      {
         SpringScript_Error(data, "SpringScript_ParseACom", "Expected 'name' to be a Value");
      }
   }
   else
   {
      SpringScript_Error(data, "SpringScript_ParseACom", "Expected object to contain 'name'");
   }

   // Side
   index = ConfigParser_GetIndexOfKey(player, "side");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_String)
      {
         out_player->side = pair->value->data.string.token->str; 
      }
      else
      {
         SpringScript_Error(data, "SpringScript_ParseACom", "Expected 'side' to be a Value");
      }
   }
   else
   {
      SpringScript_Error(data, "SpringScript_ParseACom", "Expected object to contain 'side'");
   }

   // color
   index = ConfigParser_GetIndexOfKey(player, "color");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_String)
      {
         out_player->color = pair->value->data.string.token->str; 
      }
      else
      {
         SpringScript_Error(data, "SpringScript_ParseACom", "Expected 'color' to be a Value");
      }
   }
   else
   {
      SpringScript_Error(data, "SpringScript_ParseACom", "Expected object to contain 'color'");
   }
}

static void SpringScript_ParseAPlayer(ScriptData_T * data, SSRoot_T * root, CPValue_T * player, SSPlayer_T * out_player)
{
   int index;
   CPValueObject_T * obj;
   CPObjectPair_T * pair;

   obj = &player->data.object;
   SpringScript_ParseACom(data, root, player, &out_player->com);

   // Password
   index = ConfigParser_GetIndexOfKey(player, "password");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_String)
      {
         out_player->password = pair->value->data.string.token->str; 
      }
      else
      {
         SpringScript_Error(data, "SpringScript_ParseAPlayer", "Expected 'password' to be a Value");
      }
   }
   else
   {
      SpringScript_Error(data, "SpringScript_ParseAPlayer", "Expected object to contain 'password'");
   }
}

static void SpringScript_ParseAAI(ScriptData_T * data, SSRoot_T * root, CPValue_T * player, SSAI_T * out_ai)
{
   int index, i;
   const char * name;
   CPValueObject_T * obj;
   CPObjectPair_T * pair;

   obj = &player->data.object;


   SpringScript_ParseACom(data, root, player, &out_ai->com);

   // lib
   index = ConfigParser_GetIndexOfKey(player, "lib");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_String)
      {
         out_ai->lib = pair->value->data.string.token->str; 
      }
      else
      {
         SpringScript_Error(data, "SpringScript_ParseAAI", "Expected 'lib' to be a Value");
      }
   }
   else
   {
      SpringScript_Error(data, "SpringScript_ParseAAI", "Expected object to contain 'lib'");
   }

   // client
   index = ConfigParser_GetIndexOfKey(player, "client");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_String)
      {
         name = pair->value->data.string.token->str; 
         // Find Player
         out_ai->client = NULL;
         for(i = 0; i < root->humans_count; i++)
         {
            if(name == root->humans_list[i].com.name)
            {
               out_ai->client = &root->humans_list[i];
               break;
            }
         }
         if(out_ai->client == NULL)
         {
            SpringScript_Error(data, "SpringScript_ParseAAI", "ai client name does not match any human name");
         }

      }
      else
      {
         SpringScript_Error(data, "SpringScript_ParseAAI", "Expected 'client' to be a Value");
      }
   }
   else
   {
      SpringScript_Error(data, "SpringScript_ParseAAI", "Expected object to contain 'client'");
   }
}


static void SpringScript_ParsePlayers(ScriptData_T * data, SSRoot_T * root, CPValue_T * players)
{
   int index;
   int i;
   CPValueObject_T * obj;
   CPObjectPair_T * pair;
   CPValueArray_T * array;

   obj = &players->data.object;
   // Parse Humans
   index = ConfigParser_GetIndexOfKey(players, "humans");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_Array)
      {
         array = &pair->value->data.array;
         root->humans_count = array->size;
         root->humans_list  = malloc(sizeof(SSPlayer_T) * array->size);
         for(i = 0; i < array->size; i++)
         {
            if(array->value_list[i].type == e_CPVT_Object)
            {
               SpringScript_ParseAPlayer(data, root, &array->value_list[i], &root->humans_list[i]); 
            }
            else
            {
               SpringScript_Error(data, "SpringScript_ParsePlayers", "Expected 'humans' to be an array of objects");
            }
         }

      }
      else
      {
         SpringScript_Error(data, "SpringScript_ParsePlayers", "Expected 'humans' to be an array");
         root->humans_list = NULL;
      }
   }
   else
   {
      SpringScript_Error(data, "SpringScript_ParsePlayers", "Expected 'humans' to be inside 'players'");
      root->humans_list = NULL;
   }

   // Parse ais
   index = ConfigParser_GetIndexOfKey(players, "ais");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_Array)
      {
         array = &pair->value->data.array;
         root->ai_count = array->size;
         root->ai_list  = malloc(sizeof(SSAI_T) * array->size);
         for(i = 0; i < array->size; i++)
         {
            if(array->value_list[i].type == e_CPVT_Object)
            {
               SpringScript_ParseAAI(data, root, &array->value_list[i], &root->ai_list[i]); 
            }
            else
            {
               SpringScript_Error(data, "SpringScript_ParsePlayers", "Expected 'ais' to be an array of objects");
            }
         }

      }
      else
      {
         SpringScript_Error(data, "SpringScript_ParsePlayers", "Expected 'ais' to be an array");
         root->ai_list = NULL;
      }
   }
   else
   {
      SpringScript_Error(data, "SpringScript_ParsePlayers", "Expected 'ais' to be inside 'players'");
      root->ai_list = NULL;
   }


}

static void SpringScript_ParseTeamMember(ScriptData_T * data, SSRoot_T * root, SSTeamMember_T * member, CPValue_T * value)
{
   CPValueString_T * name;
   SSTeamMemEle_T * ele;
   const char * str;
   int i;

   name = &value->data.string;
   str = name->token->str;
   member->ele_count = 1;
   member->ele_list  = malloc(sizeof(SSTeamMemEle_T));
   ele = &member->ele_list[0];
   ele->data.player = NULL;
   for(i = 0; i < root->humans_count; i++)
   {
      if(str == root->humans_list[i].com.name)
      {
         ele->is_ai = 0;
         ele->data.player = &root->humans_list[i];
         break;
      }
   }

   if(ele->data.player == NULL)
   {

      for(i = 0; i < root->ai_count; i++)
      {
         if(str == root->ai_list[i].com.name)
         {
            ele->is_ai = 1;
            ele->data.ai = &root->ai_list[i];
            break;
         }

      }
   }
}

static void SpringScript_ParseTeamArray(ScriptData_T * data, SSRoot_T * root, CPValue_T * value)
{
   CPValueArray_T * array, * mem_array;
   int i, k;
   SSTeam_T * team;
   array = &value->data.array;
   root->team_count = array->size;
   root->team_list = malloc(sizeof(SSTeam_T) * array->size);
   for(i = 0; i < array->size; i ++)
   {
      team = &root->team_list[i];
      if(array->value_list[i].type == e_CPVT_String)
      {
         team->member_count = 1;
         team->member_list  = malloc(sizeof(SSTeamMember_T));
         SpringScript_ParseTeamMember(data, root, &team->member_list[0], &array->value_list[i]);
         
      }
      else if(array->value_list[i].type == e_CPVT_Array)
      {
         mem_array = &array->value_list[i].data.array;
         team->member_count = mem_array->size;
         team->member_list  = malloc(sizeof(SSTeamMember_T) * mem_array->size);
         for(k = 0; k < mem_array->size; k++)
         {
            if(mem_array->value_list[k].type == e_CPVT_String)
            {
               SpringScript_ParseTeamMember(data, root, &team->member_list[k], &mem_array->value_list[k]);
            }
            else
            {
               SpringScript_Error(data, "SpringScript_ParseTeamArray", "Expected team member to be a name");
            }
         }
      }
      else
      {
         SpringScript_Error(data, "SpringScript_ParseTeamArray", "Expected team element to be an array or name");
      }
   }

}

static void SpringScript_ParseRoot(ScriptData_T * data, SSRoot_T * root, CPValue_T * value)
{
   int index, i;
   int result;
   CPValueObject_T * obj;
   CPObjectPair_T * pair;
   CPValue_T * teams;
   const char * str;
   result = 1;
   obj = &value->data.object;
   // Parse Map Name
   index = ConfigParser_GetIndexOfKey(value, "map");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_String)
      {
         root->map = pair->value->data.string.token->str;
      }
      else
      {
         SpringScript_Error(data, "SpringScript_ParseRoot", "Expected 'map' to be a Value");
      }
   }
   else
   {
      SpringScript_Error(data, "SpringScript_ParseRoot", "Expected to find 'map'");
   }

   // Parse Game Name
   index = ConfigParser_GetIndexOfKey(value, "mod");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_String)
      {
         root->mod = pair->value->data.string.token->str;
      }
      else
      {
         SpringScript_Error(data, "SpringScript_ParseRoot", "Expected 'mod' to be a Value");
      }
   }
   else
   {
      SpringScript_Error(data, "SpringScript_ParseRoot", "Expected to find 'mod'");
   }


   // Parse players
   index = ConfigParser_GetIndexOfKey(value, "players");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_Object)
      {
         SpringScript_ParsePlayers(data, root, pair->value);
      }
      else
      {
         SpringScript_Error(data, "SpringScript_ParseRoot", "Expected 'players' to be an object");
      }
   }
   else
   {
      SpringScript_Error(data, "SpringScript_ParseRoot", "Expected to find 'players'");
   }

   // Parse host
   index = ConfigParser_GetIndexOfKey(value, "host");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_String)
      {
         str = pair->value->data.string.token->str;
         root->host = NULL;
         for(i = 0; i < root->humans_count; i++)
         {
            if(str == root->humans_list[i].com.name)
            {
               root->host = &root->humans_list[i];
               break;
            }
         }
         if(root->host == NULL)
         {
            SpringScript_Error(data, "SpringScript_ParseRoot", "Expected 'host' to be the name of a player");
         }

      }
      else
      {
         SpringScript_Error(data, "SpringScript_ParseRoot", "Expected 'host' to be a string");
      }
   }
   else
   {
      SpringScript_Error(data, "SpringScript_ParseRoot", "Expected to find 'host'");
   }

   // Parse host-address
   index = ConfigParser_GetIndexOfKey(value, "host-address");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_String)
      {
         str = pair->value->data.string.token->str;
         root->host_addy = str;
      }
      else
      {
         SpringScript_Error(data, "SpringScript_ParseRoot", "Expected 'host -address' to be a string");
      }
   }
   else
   {
      SpringScript_Error(data, "SpringScript_ParseRoot", "Expected to find 'host-address'");
   }
   // Find teams
   index = ConfigParser_GetIndexOfKey(value, "teams");
   if(index >= 0)
   {
      pair = &obj->pair_list[index];
      if(pair->value->type == e_CPVT_Array)
      {
         teams = pair->value;
         SpringScript_ParseTeamArray(data, root, teams);
      }
      else
      {
         SpringScript_Error(data, "SpringScript_ParseRoot", "Expected 'teams' to be an array");
         teams = NULL;
      }
   }
   else
   {
      SpringScript_Error(data, "SpringScript_ParseRoot", "Expected to find 'teams'");
      teams = NULL;
   }

}


int SpringScript_Run(Scanner_T * input, FILE * output)
{
   int result;
   ConfigParser_T config;
   ScriptData_T data;
   SSRoot_T root;
   result = 1;
   data.error = 0;

   ConfigParser_Init(&config, input);

   if(config.root->type == e_CPVT_Object)
   {
      SpringScript_ParseRoot(&data, &root, config.root);
      fprintf(output, "[GAME]\n{\n");
      SpringScript_WriteGame(&root, output);
      fprintf(output, "}\n");
   }
   else
   {
      fprintf(stderr, "Error: Expected root value to be an object\n");
   }



   ConfigParser_Destory(&config);
   return result;
}


