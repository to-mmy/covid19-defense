# <a name="top"></a>Style Guidelines for CIS 29 Group 6 Project
## Table of Contents
1. [Names](#names)
   - [Name Capitalization](#nameCap)
   - [Naming Guidelines](#nameGuide)
2. [Indentation and Spacing](#spacing)
   - [Braces, Block Indentation](#spacingBraces)
   - [Spacing and Line Breaks](#spacingLines)
3. [Files](#files)
   - [File Organization](#fileOrg)
   - [Header Files: Guards and Includes](#fileHeader)
4. [Declarations](#declarations)
5. [Statements](#statements)
6. [Miscellaneous](#misc)

## <a name="names"></a>Names
### <a name="nameCap"></a>Name Capitalization
- Names should begin with a lower case letter and be lower case, and words should begin with an upper case letter.
  - e.g. `fooBar`
- Types such as classes should begin with an upper case letter.
  - e.g. `FooBar`
- Namespaces should begin with a lower case letter, and words should be separated by underscores.
  - e.g. `foo_bar`
- Constants and macros should be all caps with words separated by underscores.
  - e.g. `FOO_BAR`

[to top](#top)

### <a name="nameGuide></a>Naming Guidelines
- Names should be descriptive and in English.
  - The variable's use should be apparent from the name. The name should be easy to type without looking up how it is spelled.
  - Exception: Loop variables/variables with a short scope (less than 5 lines) are exempt as long as their purpose is obvious.
- Class getter and setter functions should be clearly named as follows:
  - e.g. `getMemberVariable()`, `setMemberVariable()`
- Avoid commonly used identifiers or wrap them with a namespace.
  - This avoids name clashes
  - e.g. map, vector, set

[to top](#top)

## <a name="spacing"></a>Indentation and Spacing
### <a name="spacingBraces"></a>Braces, Block Indentation
- Braces shall be as follows:
  - The opening brace should be placed at the end of the enclosing statement.
  - The closing brace should be on a line of its own lined up with the enclosing statement.
  - Statements between the braces are indented 4 columns to the right relative to the enclosing statement.
  - Exception: statements enclosed by namespace wrappers need not be indented.
  - Exception: empty braces (`{}`) need not be broken into lines.
  - Exception: access modifiers should be aligned with enclosing statement.
  - Examples:
```
void function(int a) {
    int i;
    if (a > 0) {
        i = a;
    }
    else {
        i = a * -1;
    }
}
```
```
namespace n {
class A {
private:
    int aInt;
public:
    A(int a) : aInt(a) {}
};
}
```
- Do not use Tab characters.
  - Set up your editor to replace Tabs with spaces if you must.

[to top](#top)

### <a name="spacingLines"></a>Spacing and Line Breaks
- Place each statement on a line on its own.
- Declare each variable in a separate declaration.
  - Avoids confusion with such statements as `int* p, i;`
- Lines shall not exceed 100 characters.
  - If a line would exceed 100 characters, do your best to break it where it would make sense and align the next portion with what it was broken from.
  - For example:
```
void aReallyReallyLongFunctionName(int reallyLongParameterOne, int reallyLongParameterTwo, 
                                   int reallyLongParameterThree) {
    std::cout << reallyLongParameterOne << ' ' << reallyLongParameterTwo << ' '
              << reallyLongParameterThree << '\n';
}
```


[to top](#top)

## <a name="files"></a>Files
### <a name="fileOrg"></a>File Organization
- File organization is subject to change.
  - Therefore, neither absolute nor relative directory names should be used in `#include` directives.
- Header files and source files shall be kept separately in the `/include` and `/src` directories respectively
- Resource files such as images and sounds shall be placed in the `/resources` directory
- Libraries' header and source files shall be placed in `/libs/<libraryName>/include` and `/libs/<libraryName>/src` directories
- Library object files shall be built to `/lib`
- C++ source files shall have extension `.cpp`.
- C++ header files shall have extension `.h`.
- Only one externally visible class per header file.
- Only one externally visible function per header file.
- Class and function declarations and implementations should be split into header and source files respectively.
  - The header and source files should have the same name, differing only by file extension.

[to top](#top)

### <a name="fileHeader"></a>Header Files: Guards and Includes
- Header files must have include guards.
  - The name of the macro used in the include guard shall have the same name as the file (excluding the extension) followed by the suffix "_H".
  - Example:
```
// File.h
#ifndef FILE_H
#define FILE_H
...
#endif // FILE_H
```
- Put all `#include` directives at the top of files.
- Project headers shall be included with `""` and system headers with `<>`.
- All project header `#include` directives shall go before system `#include` directives.
- No `using` statements in header files.

[to top](#top)

## <a name="declarations"></a>Declarations
- Declare inherited functions `virtual`, and use the keyword `override`.
- Do not use global variables. Global constants should be wrapped in a namespace.
- Do not use `using namespace` declarations.
  - `using std::cout;` and similar are O.K.
- Declare class data private.

[to top](#top)

## <a name="statements"></a>Statements
- Never use `goto`s.
- Do not use `break` or `continue` in loops.
- All `switch` statements shall have a `default` label.
- Do not use `do-while` loops.
  - Any do-while loop can and should be rewritten into a while loop or for loop.

[to top](#top)

## <a name="misc"></a>Miscellaneous
- Avoid macros.
  - Replace with constants, enumerations, inline functions, or lambdas.
- If a value is to be used more than once, use a constant instead of literals.
- Use C++ style casting not C style casting.
- Use prefix increment/decrement instead of postfix increment/decrement when the value of the variable is not used.
  - For class objects, the postfix operation has to keep a temporary return value of the object before changing the object. It is a good habit to get into to use prefix increment/decrement at all times.
  - e.g. `for (auto iter = myVector.cbegin(); iter != myVector.cend(); ++iter)` instead of `iter++`.
[to top](#top)
