#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "lslmini.hh"
#include "logger.hh"

char *builtins_file = NULL;
extern const char *builtins_txt[];

struct _TypeMap {
   const char *name;
   LST_TYPE type;
} types[] = {
   {"void",    LST_NULL},
   {"integer", LST_INTEGER},
   {"float",   LST_FLOATINGPOINT},
   {"string",  LST_STRING},
   {"key",     LST_KEY},
   {"vector",  LST_VECTOR},
   {"rotation",LST_QUATERNION},
   {"list",    LST_LIST},
   {NULL,      LST_ERROR}
};

LLScriptType *str_to_type(const char *str) {
   for (int i = 0; types[i].name != NULL; ++i) {
      if ( strcmp(types[i].name, str) == 0 )
         return LLScriptType::get(types[i].type);
   }
   fprintf(stderr, "invalid type in builtins.txt: %s\n", str);
   exit(EXIT_FAILURE);
   return LLScriptType::get(LST_ERROR);
}

void LLScriptScript::define_builtins() {
   LLScriptFunctionDec *dec = NULL;
   FILE *fp = NULL;
   char buf[1024];
   char original[1024];
   char *ret_type = NULL;
   char *name = NULL;
   char *ptype = NULL, *pname = NULL;
   int line = 0;

   if(builtins_file != NULL) {
      fp = fopen(builtins_file, "r");

      if (fp==NULL) {
         snprintf(buf, 1024, "couldn't open %s", builtins_file);
         perror(buf);
         exit(EXIT_FAILURE);
      }
   }

   while (1) {
      if (fp) {
         if (fgets(buf, 1024, fp)==NULL)
            break;
      } else {
         if (builtins_txt[line]==NULL)
            break;
         strncpy(buf, builtins_txt[line], 1024);
         ++line;
      }

      strcpy(original, buf);

      ret_type = strtok(buf,  " (),");

      if ( ret_type == NULL ) {
         fprintf(stderr, "error parsing %s: %s\n", builtins_file, original);
         exit(EXIT_FAILURE);
         return;
      }

      if (!strcmp(ret_type, "const")) {
         ret_type = strtok(NULL, " =(),");
         name     = strtok(NULL, " =(),");

         if ( ret_type == NULL || name == NULL ) {
            fprintf(stderr, "error parsing %s: %s\n", builtins_file, original);
            exit(EXIT_FAILURE);
            return;
         }

         LLScriptType *const_type = str_to_type(ret_type);
         LLScriptSymbol *symbol = new LLScriptSymbol(
                  strdup(name), const_type, SYM_VARIABLE, SYM_BUILTIN
                  );
         // find the start of the next token
         char *value = name + strlen(name) + 1;
         LLScriptConstant *constant = NULL;
         value += strspn(value, "= "); // past the equal sign, skipping spaces
         switch (const_type->get_itype()) {
            case LST_INTEGER:
               {
                  int32_t num = 0;
                  if (sscanf(value, "0x%x", (uint32_t *)&num) == 1
                      || sscanf(value, "0X%x", (uint32_t *)&num) == 1
                      || sscanf(value, "%d", &num) == 1) {
                     constant = new LLScriptIntegerConstant(num);
                  }
               }
            case LST_FLOATINGPOINT:
               {
                  float num = 0.f;
                  if (sscanf(value, "%g", &num) == 1)
                     constant = new LLScriptFloatConstant(num);
               }
            case LST_VECTOR:
               {
                  float x = 0.f, y = x, z = x;
                  if (sscanf(value, "<%f,%f,%f>", &x, &y, &z) == 3)
                     constant = new LLScriptVectorConstant(x, y, z);
               }
            case LST_QUATERNION:
               {
                  float x = 0.f, y = x, z = x, s = 1.f;
                  if (sscanf(value, "<%f,%f,%f,%f>", &x, &y, &z, &s) == 4)
                     constant = new LLScriptQuaternionConstant(x, y, z, s);
               }
            case LST_STRING:
               {
                  char *p = value;
                  char val[1024];
                  int i = 0;
                  if (*p++ != '"') break;
                  while (*p != '"') {
                     if (*p == '\\' && *++p == 'n')
                        val[i++] = '\n';
                     else
                        val[i++] = *p;

                     if (*p == '\0')
                        break;
                     p++;
                  }
                  if (*p == '"') {
                     val[i] = 0;
                     p = new char[strlen(val) + 1];
                     strcpy(p, val);
                     constant = new LLScriptStringConstant(p);
                  }
               }

            //case LST_KEY: case LST_LIST:
            // just use default
            default:
               ;
         }
         symbol->set_constant_value(constant);
         define_symbol(symbol);
      }
      else if (!strcmp(ret_type, "event")) {
         name     = strtok(NULL, " (),");

         if ( ret_type == NULL || name == NULL ) {
            fprintf(stderr, "error parsing %s: %s\n", builtins_file, original);
            exit(EXIT_FAILURE);
            return;
         }

         dec = new LLScriptFunctionDec();
         while ( (ptype = strtok(NULL, " (),")) != NULL ) {
            if ( (pname = strtok(NULL, " (),")) != NULL ) {
               dec->push_child(new LLScriptIdentifier( str_to_type(ptype), strdup(pname)));
            }
         }

         define_symbol( new LLScriptSymbol(
                  strdup(name), str_to_type("void"), SYM_EVENT, SYM_BUILTIN, dec
                  ));
      }
      else {
         name     = strtok(NULL, " (),");

         if ( ret_type == NULL || name == NULL ) {
            fprintf(stderr, "error parsing %s: %s\n", builtins_file, original);
            exit(EXIT_FAILURE);
            return;
         }

         dec = new LLScriptFunctionDec();
         while ( (ptype = strtok(NULL, " (),")) != NULL ) {
            if ( (pname = strtok(NULL, " (),")) != NULL ) {
               dec->push_child(new LLScriptIdentifier( str_to_type(ptype), strdup(pname)));
            }
         }

         define_symbol( new LLScriptSymbol(
                  strdup(name), str_to_type(ret_type), SYM_FUNCTION, SYM_BUILTIN, dec
                  ));
      }
   }
}

