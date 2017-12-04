#ifndef _LOGGER_HH
#define _LOGGER_HH 1

#include <stdlib.h>
#if !_MSC_VER
#include <stdio.h>
#endif // !_MSC_VER

#include <vector>
#include <utility>  // pair

// have to do this here because of circular dependencies
#if ! defined (YYLTYPE) && ! defined (YYLTYPE_IS_DECLARED)
typedef struct YYLTYPE {
    int first_line;
    int first_column;
    int last_line;
    int last_column;
} YYLTYPE;

#define YYLTYPE_IS_DECLARED 1
#define YYLTYPE_IS_TRIVIAL 1
#endif

enum LogLevel {
  LOG_ERROR,            // errors
  LOG_WARN,             // warnings
  LOG_INTERAL_ERROR,    // internal errors
  LOG_INFO,             // what we're up to
  LOG_DEBUG,            // base debug messages
  LOG_DEBUG_MINOR,      // minor debug messages
  LOG_DEBUG_SPAM,       // spammy debug messages
  LOG_CONTINUE,         // continuation of last message

  LOG_LAST
};

enum ErrorCode {
    /***********************************************************************
     IMPORTANT NOTES:
       - The order of these errors must match the order of the corresponding
         messages in logger.cc.
       - The same applies both to the errors and to the warnings.
       - The numeric codes are used in the test suite. Don't remove one from
         the middle: change it to E_removed_NNN or to W_removed_NNN instead,
         and the corresponding text in logger.cc to "".
       - Add new codes to the end, right before E_LAST or W_LAST.
     ***********************************************************************/

    // errors
    E_ERROR                 = 10000,
    E_DUPLICATE_DECLARATION,           // 10001
    E_INVALID_OPERATOR,                // 10002
    E_DEPRECATED,                      // 10003
    E_DEPRECATED_WITH_REPLACEMENT,     // 10004
    E_WRONG_TYPE,                      // 10005
    E_UNDECLARED,                      // 10006
    E_UNDECLARED_WITH_SUGGESTION,      // 10007
    E_INVALID_MEMBER,                  // 10008
    E_MEMBER_NOT_VARIABLE,             // 10009
    E_MEMBER_WRONG_TYPE,               // 10010
    E_ARGUMENT_WRONG_TYPE,             // 10011
    E_TOO_MANY_ARGUMENTS,              // 10012
    E_TOO_FEW_ARGUMENTS,               // 10013
    E_CHANGE_STATE_IN_FUNCTION,        // 10014
    E_WRONG_TYPE_IN_ASSIGNMENT,        // 10015
    E_WRONG_TYPE_IN_MEMBER_ASSIGNMENT, // 10016
    E_RETURN_VALUE_IN_EVENT_HANDLER,   // 10017
    E_BAD_RETURN_TYPE,                 // 10018
    E_SYNTAX_ERROR,                    // 10019
    E_GLOBAL_INITIALIZER_NOT_CONSTANT, // 10020
    E_NO_OPERATOR,                     // 10021
    E_NO_EVENT_HANDLERS,               // 10022
    E_removed_1,                       // 10023
    E_removed_2,                       // 10024
    E_removed_3,                       // 10025
    E_NOT_ALL_PATHS_RETURN,            // 10026
    E_ARGUMENT_WRONG_TYPE_EVENT,       // 10027
    E_TOO_MANY_ARGUMENTS_EVENT,        // 10028
    E_TOO_FEW_ARGUMENTS_EVENT,         // 10029
    E_INVALID_EVENT,                   // 10030
    E_DUPLICATE_DECLARATION_BUILTIN,   // 10031
    E_MULTIPLE_EVENT_HANDLERS,         // 10032
    E_BREAK_WITHOUT_SWITCH,            // 10033
    E_SWITCH_MULTIPLE_DEFAULTS,        // 10034
    E_INCOMPATIBLE_CASE_TYPE,          // 10035
    E_DECLARATION_NOT_ALLOWED,         // 10036
    E_GOD_MODE_FUNCTION,               // 10037
    E_LAST


    ,
    // warnings
    W_WARNING               = 20000,
    W_SHADOW_DECLARATION,              // 20001
    W_ASSIGNMENT_IN_COMPARISON,        // 20002
    W_CHANGE_TO_CURRENT_STATE,         // 20003
    W_CHANGE_STATE_HACK_CORRUPT,       // 20004
    W_CHANGE_STATE_HACK,               // 20005
    W_MULTIPLE_JUMPS_FOR_LABEL,        // 20006
    W_EMPTY_IF,                        // 20007
    W_removed_1,                       // 20008
    W_DECLARED_BUT_NOT_USED,           // 20009
    W_LIST_COMPARE,                    // 20010
    W_CONDITION_ALWAYS_TRUE,           // 20011
    W_CONDITION_ALWAYS_FALSE,          // 20012
    W_removed_2,                       // 20013
    W_UNUSED_EVENT_PARAMETER,          // 20014
    W_STATEMENTS_BEFORE_CASE,          // 20015
    W_CONSTANT_SWITCH,                 // 20016
    W_SWITCH_NO_DEFAULT,               // 20017
    W_DUPLICATE_CASE,                  // 20018
    W_L_STRING,                        // 20019
    W_PRINT,                           // 20020
    W_LAST

};

enum AssertType {
    NO_ASSERTIONS,
    EXPECTED_ASSERTIONS,
    ALL_ASSERTIONS
};

#define LOG         Logger::get()->log
#define LOGV        Logger::get()->logv
#define IN(v)       (v)->get_lloc()
#define LINECOL(l)  (l)->first_line, (l)->first_column
#define HERE        IN(this)
#define ERROR       Logger::get()->error


#ifdef _MSC_VER
#ifdef DEBUG_LEVEL
#define DEBUG LOG
#else /* not DEBUG_LEVEL */
#define DEBUG __noop
#endif /* not DEBUG_LEVEL */
#else /* not _MSC_VER */
#ifdef DEBUG_LEVEL
#define DEBUG LOG
#else /* not DEBUG_LEVEL */
#ifdef __GNUC__
#define DEBUG(args...)
#else /* not __GNUC__ */
#define DEBUG(...)
#endif /* not __GNUC__ */
#endif /* not DEBUG_LEVEL */
#endif /* not _MSC_VER */

// Logger for a script. Singleton
class Logger {
  public:
    // get current instance
    static Logger* get();
    ~Logger();

    void set_path(const char *fpath) ;
    void log(LogLevel type, YYLTYPE *loc, const char *fmt, ...);
    void logv(LogLevel type, YYLTYPE *loc, const char *fmt, va_list args, ErrorCode error=(ErrorCode)0);
    void error( YYLTYPE *loc, ErrorCode error, ... );
    void report();

    int     get_errors()    { return errors;    }
    int     get_warnings()  { return warnings;  }
    void    set_show_end(bool v) { show_end = v; }
    void    set_show_info(bool v){ show_info = v;}
    void    set_sort(bool v)     { sort = v;     }
    void    set_show_error_codes(bool v) { show_error_codes = v; }
    void    set_check_assertions(AssertType v) { check_assertions = v; }
    void    set_file_path(const char *v) { file_path = v; }

    void    add_assertion( int line, ErrorCode error ) {
      assertions.push_back( new std::pair<int, ErrorCode>( line, error ) );
    }

  protected:
    Logger() : errors(0), warnings(0), show_end(false), show_info(false), sort(true), show_error_codes(false), check_assertions(NO_ASSERTIONS), file_path(NULL), last_message(NULL), file(stderr) {};
    int     errors;
    int     warnings;
    bool    show_end;
    bool    show_info;
    bool    sort;
    bool    show_error_codes;
    AssertType check_assertions;
    const char *file_path;
    class LogMessage *last_message;

  private:
    static Logger *instance;
    FILE    *file;
    std::vector<class LogMessage*>    messages;
    std::vector<ErrorCode>            errors_seen;
    std::vector< std::pair<int, ErrorCode>* >    assertions;
    static const char* error_messages[];
    static const char* warning_messages[];
};

////////////////////////////////////////////////////////////////////////////////
// Log message entry, for sorting
class LogMessage {
  public:
    LogMessage( LogLevel type, YYLTYPE *loc, char *message, ErrorCode error );
    ~LogMessage();

    LogLevel    get_type() { return type; }
    YYLTYPE    *get_loc()  { return &loc;  }
    ErrorCode   get_error() { return error; }
    void        cont(char *message);
    void        print(FILE *fp);

  private:
    LogLevel            type;

    // we need our own copy of loc, because messages logged in the parser will be
    // handing us a copy of a loc structure that is constantly changing, and will
    // be invalid when we go to sort.
    YYLTYPE             loc;

    std::vector<char*>  messages;
    ErrorCode           error;
};

#endif /* not LOGGER_HH */
