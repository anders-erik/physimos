#pragma once

#include <vector>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>

#include <variant>
#include <memory>



#define QUOTATION_MARK      '\u0022'
#define SOLLIDUS            '\u002F'
#define SOLLIDUS_REVERSE    '\u005C'

#define SPACE               '\u0020'
#define TAB                 '\u0009'
#define NEW_LINE            '\u000A'
#define CARRIAGE_RETURN     '\u000D'


typedef std::string     JSS; // Json source/serialized string

typedef std::string     j_string;
typedef bool            j_bool;
typedef std::nullptr_t  j_null;
typedef double          j_float;
typedef long long int   j_int;

