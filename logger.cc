#include <stdarg.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "logger.hh"
#include "lslmini.hh"
#include "lslmini.tab.h"

Logger* Logger::instance = NULL;


Logger *Logger::get() {
   if ( instance == NULL )
      instance = new Logger();
   return instance;
}

void Logger::log(LogLevel level, YYLTYPE *yylloc, const char *fmt, ...) {
   va_list        args;
   va_start(args, fmt);
   logv(level, yylloc, fmt, args);
   va_end(args);
}

void Logger::error(YYLTYPE *yylloc, ErrorCode error, ...) {
   // FIXME: magic numbers
   static char   buf[1024];
   char         *bp = buf;
   const char   *fmt;
   const char   *fp;
   bool          seen_before;
   LogLevel      level = (error < W_WARNING) ? LOG_ERROR : LOG_WARN;
   va_list       args;

   *bp = 0;

   if ( level == LOG_ERROR ) {
      fmt = error_messages[ (int)(error - E_ERROR) ];
   } else {
      fmt = warning_messages[ (int)(error - W_WARNING) ];
   }

   if ( check_assertions ) {
      // if we're checking assertions, messages will be removed by matching assertions,
      // so we just add "Unexpected" to all of them.
      bp += sprintf(bp, level == LOG_ERROR ? "Unexpected error: " : "Unexpected warning: ");

      // upgrade everything to error
      level = LOG_ERROR;

   }


   // see if we've seen this error code before
   if ( std::find(errors_seen.begin(), errors_seen.end(), error) != errors_seen.end() ) {
      seen_before = true;
   } else {
      seen_before = false;
      errors_seen.push_back(error);
   }


   if ( show_error_codes ) {
      bp += sprintf(bp, "[E%d] ", (int)error );
   }

   for ( fp = fmt ; *fp ; ++fp ) {
      if ( *fp == '\n' )
         break;
      else
         *bp++ = *fp;
   }
   *bp = 0;

   va_start(args, error);
   logv( level, yylloc, buf, args, error );
   va_end(args);

   // extra info we haven't seen before
   if ( !seen_before && *fp != 0 ) {
      while ( *fp ) {
         bp = buf;
         for ( ++fp; *fp; ++fp ) {
            if ( *fp == '\n' )
               break;
            else
               *bp++ = *fp;
         }
         *bp = 0;
         log( LOG_CONTINUE, NULL, buf );
      }
   }
}


void Logger::logv(LogLevel level, YYLTYPE *yylloc, const char *fmt, va_list args, ErrorCode error) {
   char          *type           = NULL;
   static char    buf[1024];
   char          *bp             = buf;
   switch (level) {
      case LOG_ERROR:
         type = "ERROR";
         ++errors;
         break;
      case LOG_WARN:
         type = "WARN";
         ++warnings;
         break;
      case LOG_INFO:
         if ( show_info == false ) return;
         type = "INFO";
         break;
      case LOG_DEBUG:
      case LOG_DEBUG_MINOR:
      case LOG_DEBUG_SPAM:
#ifdef DEBUG_LEVEL
         if ( DEBUG_LEVEL < level ) return;
         type = "DEBUG";
#else /* not DEBUG_LEVEL */
         return;
#endif /* not DEBUG_LEVEL */
         break;
      case LOG_CONTINUE:
         vsprintf( bp, fmt, args );
         if ( last_message )
            last_message->cont( buf );

         return;
      default:
         file = stdout;
         type = "OTHER";
         break;
   }

   if (file_path != NULL)
   {
      bp += sprintf(bp, "%s::", file_path); 
   }
   bp += sprintf(bp, "%5s:: ", type );
   if ( yylloc != NULL ) { 
      bp += sprintf(bp, "(%3d,%3d)", yylloc->first_line, yylloc->first_column); 
      if ( show_end )
         bp += sprintf(bp, "-(%3d,%3d)", yylloc->last_line, yylloc->last_column);
      bp += sprintf(bp, ": ");
   }
   bp += vsprintf(bp, fmt, args);

   last_message = new LogMessage( level, yylloc, buf, error );
   //  fprintf(stderr, "%p\n", last_message);
   messages.push_back(last_message);
   return;
}

struct LogMessageSort {
   bool operator()(LogMessage* const& left, LogMessage* const& right) {
      if ( left->get_type() < right->get_type() )
         return true;
      else if ( left->get_type() > right->get_type() )
         return false;

      if ( left->get_loc()->first_line < right->get_loc()->first_line )
         return true;
      else if ( left->get_loc()->first_line > right->get_loc()->first_line )
         return false;

      if ( left->get_loc()->first_column < right->get_loc()->first_column )
         return true;

      return false;
   }
};

void Logger::report() {
   std::vector<LogMessage*>::iterator i;
   if ( check_assertions ) {
      std::vector< std::pair<int, ErrorCode>* >::iterator ai;
      for ( ai = assertions.begin(); ai != assertions.end(); ++ai ) {
         for ( i = messages.begin(); i != messages.end(); ++i ) {
            if ( (*ai)->first == (*i)->get_loc()->first_line && (*ai)->second == (*i)->get_error() ) {
               --errors; // when check assertions, warnings are treated as errors.
               messages.erase(i);
               i = messages.end() + 1; // HACK?: ensure that i isn't messages.end()
               break;
            }
         }
         if ( i == messages.end() )
            LOG( LOG_ERROR, NULL, "Assertion failed: error %d on line %d.", (*ai)->second, (*ai)->first );
      }
   }

   if ( sort )
      std::sort( messages.begin(), messages.end(), LogMessageSort() );

   for ( i = messages.begin(); i != messages.end(); ++i )
      (*i)->print(stderr);

   fprintf(stderr, "TOTAL:: Errors: %d  Warnings: %d\n", errors, warnings);
}

LogMessage::LogMessage( LogLevel type, YYLTYPE *loc, char *message, ErrorCode error ) : type(type), error(error) {
   char *np = new char[strlen(message)+1];
   if ( loc ) this->loc = *loc;
   if ( np != NULL ) {
      strcpy(np, message);
      messages.push_back(np);
   }
}

void LogMessage::cont( char *message ) {
   char *np = new char[strlen(message)+1];
   if ( np != NULL ) {
      strcpy(np, message);
      messages.push_back(np);
   }
}



void LogMessage::print( FILE *fp ) {
   std::vector<char*>::const_iterator i;
   for ( i = messages.begin(); i != messages.end(); ++i ) {
      if ( i != messages.begin() ) 
         fprintf( fp, "%20s", "");
      fprintf( fp, "%s\n", *i );
   }
}

LogMessage::~LogMessage() {
   std::vector<char*>::const_iterator i;
   for ( i = messages.begin(); i != messages.end(); ++i )
      delete *i;
}




/// ERROR MESSAGE

/****************************************************************************
  IMPORTANT: Do not add messages here without having added the corresponding
             enum first in logger.hh; see caveats there.
 ****************************************************************************/

const char *Logger::error_messages[] = {
   "ERROR",                                                           // 10000
   "Duplicate declaration of `%s'; previously declared at (%d, %d).", // 10001
   "Invalid operator: %s %s %s.",                                     // 10002
   "`%s' is deprecated.",                                             // 10003
   "`%s' is deprecated, use %s instead.",                             // 10004
   "Attempting to use `%s' as a %s, but it is a %s.",                 // 10005
   "`%s' is undeclared.",                                             // 10006
   "`%s' is undeclared; did you mean %s?",                            // 10007
   "Invalid member: `%s.%s'.",                                        // 10008
   "Trying to access `%s.%s', but `%1$s' is a %3$s",                  // 10009
   "Attempting to access `%s.%s', but `%1$s' is not a vector or rotation.", // 10010
   "Passing %s as argument %d of `%s' which is declared as `%s %s'.", // 10011
   "Too many arguments to function `%s'.",                            // 10012
   "Too few arguments to function `%s'.",                             // 10013
   "Functions cannot change state.",                                  // 10014
   "`%s %s' assigned a %s value.",                                    // 10015
   "%s member assigned %s value (must be float or integer).",         // 10016
   "Event handlers cannot return a value.",                           // 10017
   "Returning a %s value from a %s function.",                        // 10018
   "%s", // Syntax error, bison includes all the info.                // 10019
   "Global initializer must be constant.",                            // 10020
   "Expression and constant without operator.",                       // 10021
   "State must have at least one event handler.",                     // 10022
   "",                                                                // 10023
   "`%s' is a constant and cannot be used as an lvalue.",             // 10024
   "`%s' is a constant and cannot be used in a variable declaration.", // 10025
   "Not all code paths return a value.",                              // 10026
   "Declaring `%s' as parameter %d of `%s' which should be `%s %s'.", // 10027
   "Too many parameters for event `%s'.",                             // 10028
   "Too few parameters for event `%s'.",                              // 10029
   "`%s' is not a valid event name.",                                 // 10030
   "`%s' is an event name, and cannot be used as a function name.",   // 10031
   "Multiple handlers for event `%s'.",                               // 10032

};

const char *Logger::warning_messages[] = {
   "WARN",                                                            // 20000
   "Declaration of `%s' in this scope shadows previous declaration at (%d, %d)",  // 20001
   "Suggest parentheses around assignment used as truth value.",      // 20002
   "Changing state to current state acts the same as return. (SL1.8.3)\n"
      "If this is what you intended, consider using return instead.", // 20003
   "Changing state in a list or string function will corrupt the stack.\n"
      "Using the return value from this function will cause a run-time bounds check error.\n"
      "See: http://lsl.project.zone/lsl/language/state#hacks",        // 20004
   "Using an if statement to change state in a function is a hack and may have unintended side-effects.\n"
      "See: http://lsl.project.zone/lsl/language/state#hacks",        // 20005
   "Multiple jumps for label `%s' - only the last will execute.",     // 20006
   "Empty if statement.",                                             // 20007
   "",                                                                // 20008
   "%s `%s' declared but never used.",                                // 20009
   "Using == on lists only compares lengths.\n"
      "See: http://wiki.secondlife.com/wiki/List#Comparing_Lists",    // 20010
   "Condition is always true.",                                       // 20011
   "Condition is always false.",                                      // 20012
   "",                                                                // 20013 (unused)
   "Unused event parameter `%s'.",                                    // 20014
   "Statements before the first case label won't be executed.",       // 20015
};
