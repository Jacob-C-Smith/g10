# Code Style

## Functions
Functions should generally follow this format
```
int fun ( int a, int b, int *p_c )
```

## flow control
2. when comparing an unknown value with a constant/keyword value, first must be on the left side of comparison.
```c
if (flName < 0.0f || flName > M_PI)
```
3. check for pointer validity shouldn't look like boolean check and must be compared explicitly with keyword.
```c
if (pName != nullptr)
```
4. check for result of expression when it may not be in range \[0, 1\] shouldn't look like a boolean check and must be compared explicitly.
```c
if ((uName & 0xAF) != 0U)
```
```c
if (iName < 0 || iName > 0)
```

### Literals
- Number literals:

	1\. differentiate numbers use with *lowercase* hex/binary/exponent/binary-exponent literals.
	```c
	unsigned int uNameAddress = 0x1EE7;
	unsigned int uNameBits = 0b1100100;
	float flNamePower = 0x1p4f;
	```
	2\. specify number data type with *UPPERCASE* (except float, with *lowercase*) type literal.
	```c
	unsigned int uName = 0U;
	long lName = 0L;
	long long llName = 0LL;
	long double ldName = 0.0L;
	float flName = 0.0f;
	```
	3\. wrap long constant numbers with apostrophe literal.
	```c
	int iName = 2'147'483'648;
	```
- String literals

	1\. wrap string that frequently contain code escapes with *UPPERCASE* raw string literal.
	```c
	std::string strName = R"(no new \n line)";
	```
	2\. specify string/character encoding with case-sensetive literal.
	```c
	wchar_t wName = L'\u2764';
	std::wstring wstrName = L"\u2764"s;
	char8_t uchName = u8'\u00AE';
	std::u8string ustrName = u8"\u2764"s;
	char16_t uchName = u'\u2764';
	std::u16string ustrName = u"\u2764"s;
	char32_t uchName = U'\U0010FFFF';
	std::u32string ustrName = U"\U0010FFFF"s;
	```
	3\. specify string type with custom literal.
	```c
	std::string strName = "string"s;
	std::string_view strName = "string"sv;
	```
### break
1. line breaks must be LF (line feed, single '\n' character without carriage return '\r' character)
2. braces with short aggregate initialization, short lambda expression can be on same line.
```c
std::array<std::pair<int, float>> arrName{{ { 0, 0.0f } }};
FunctionName([&x](const auto& name) { return name.x > x; });
```
3. each brace must be on it's own line, except when enclosed body are empty.
```c
void Function()
{
	int iName = 0;
}

void EmptyFunction() { };
```
4. one-line body statements, must be on new-line
```c
if (bCondition)
	Function();

while (bCondition)
	Function();

for (int i = 0; i < n; i++)
	Function(i);
```
### space
1. must be placed after pointer/reference to align them on left side, except declarations of multiple variables.
```c
void* pName = nullptr;
char *szNameFirst = nullptr, *szNameSecond = nullptr;
std::string& strName = "name";
std::string&& strName = "name";
```
2. must be placed around assignment/ternary/binary but not unary/member operators.
```c
iName = -iName;
uName = (uName & 0x2);
bName = (bName ? true : false);
```
3. must be placed after keywords in control flow statements.
```c
while (bName)
{
	if (bName)
		bName = !bName;
}
```
4. must be placed between empty curve/square braces, such as list initialization, constructor with initializer list, lambda capture etc, but not aggregate initialization.
```c
int iName = { };
std::array<std::pair<int, float>> arrName{{ { 0, 0.0f } }};

Name_t(const int iName) :
	iName(iName) { }

auto Name = [ ](int& nNameCount)
{
	nNameCount++;
};
```
5. must be placed after comma and colon, except conditions when new line must be placed instead.
```c
int iName = 0, iSecondName = 0;

class CDerived : IBase { };

Name_t(const int iName) :
	iName(iName) { }
```
### macro
1. arguments must be enclosed in parenthesis if they can be used as an expression.
```c
#define Q_NAME(X) ((X) * 0.5f)
float flName = Q_NAME(5.0f - 8.0f);
```
2. expression must be enclosed in parenthesis.
```c
#define Q_NAME (-1)
```
### comment
- hot words:

	1\. `@todo: <explanation>` - explains things to do/fix/improve in the future updates.

	2\. `@note: <info>` - recommended information for the user to read.

	3\. `@author: <name>` - explains things to test for some reason.

	4\. `@credits: <author>` - credentials of the author of used/referenced code.
	
	5\. `@file: <path>` - path to the following file
	
	6\. `@ida <full method/variable name if known> [inlined]: (method name of pattern source if known) <module> -> <pattern>`
	- syntax:

		pattern itself are enclosed with quotes `""` and represent found address.

		address enclosed with brackets `[]` represent it's dereference.

		address enclosed with parentheses `()` used to clarify sequence when it's unclear.

		address prefixed with `U8`/`U16`/`U32`/`U64` or `I8`/`I16`/`I32`/`I64` represent data type for the cast, if not specified, it's `U32` by default.

		address prefixed with `ABS` represent conversion of relative address to absolute.

	7\. `@xref: (method name of reference source if known) <string/name>` - reference to the following pattern.
	- syntax:

		reference by string are enclosed with quotes `""`.

		reference by name are enclosed with apostrophes `''`.

1. preferred to be in *lowercase*, except when extra attention is required.
2. stylistic comments must be written in doxygen style, with spaces around colon.
3. multi-line comments less than 5 lines must be written in C++ style (double-slash and triple-slash for stylistic), otherwise C style must be used (slash with asterisk and slash with double-asterisk for stylistic).

# Naming conventions
conditions are sorted by their priority, if a higher priority condition overrides condition with a lower priority, it is preferred.
we're prefer concrete and explicit definition over simplicity, but remain readability.

### Functions
1. function name must be written in *snake_case*.
```c
void snake_case ( );
```

### Variables
1. Variable name must be written in *snake_case* and prefixed with hungarian notation of data type.

	- Pointer:

		1. If the variable is a struct, prefix with '_' overriding any other prefix.
		```c
		struct my_struct_s _my_struct = { 0 };
		```

		2. If the variable is a function argument and a pointer, prefix with 'p' for each level of indirection.
		```c
		void function_a ( u32  *p_result );
		void function_b ( u32 **pp_result );
		```

	- Function:

		1. No additional conditions.
		```c
		void function ( );
		```
	
	- Function pointers
		1. If the variable is a function pointer, prefix with 'pfn' 
		```c
		void (*pfn_function_c) ( u32 *a, u32 *b );
		```

	- Container:

		1. Fixed-size array variables must be prefixed with '_'.
		```c
		char _name[128] = { 0 };
		```

		2\. varying-size C/C++/STL container variable must be prefixed with 'vec'.
		```c
		int* vecName = malloc(nSize);
		int* vecName = new int[nSize];
		std::vector<int> vecName = { };
		```

	- Boolean:

		1. No additional conditions.
		```c
		bool boolean = false;
		```
	- Integer:

		1. if the variable supposed to indicate index/count/size/mode/type, prefix becomes 'n' regardless the data type.
		```c
		std::ptrdiff_t nNameIndex = arrName.find(...);
		std::size_t nNameSize = arrName.size();
		std::ptrdiff_t nNameCount = arrName.size() - arrInvalidName.size();
		ENameMode nNameMode = NAME_MODE_FIRST;
		```
		2\. if the variable is unsigned, prefix becomes 'u' regardless the data type, except `long` qualifiers, where it must be appended instead.
		```c
		std::uint8_t uNameByte = 0U;
		std::uint16_t uNameShort = 0U;
		std::uint32_t uNameInt = 0U;
		std::uint64_t ullNameLongLongInt = 0ULL;
		```
		3\. if none of the conditions are met.
		```c
		char chName;
		short shName;
		int iName;
		long long llName;
		```
	- floating point:

		1\. no additional conditions.
		```c
		float flName = 0.0f;
		double dName = 0.0;
		long double ldName = 0.0L;
		```
	- string:

		1\. if the variable is a single character.
		```c
		char chName = '\0';
		wchar_t wName = L'\000';
		char8_t uchName = u8'\u0000';
		char16_t uchName = u'\u0000';
		char32_t uchName = U'\U00000000';
		```
		2\. if the variable a zero-terminated string it must be prefixed with 'sz' and safe wrapped string variables with 'str'.
		```c
		const char* szName = "";
		std::string strName = ""s;
		```
		3\. if the variable have character type `wchar_t`, `char8_t`, `char16_t`, `char32_t` it must append string type to the prefix, other character types don't affect the prefix.
		```c
		const wchar_t* wszName = L"Example";
		const char8_t* uszName = u8"Example";
		const char16_t* uszName = u"Example";
		const char32_t* uszName = U"Example";
		std::wstring wstrName = L"Example"s;
		std::u8string ustrName = u8"Example"s;
		std::u16string ustrName = u"Example"s;
		std::u32string ustrName = U"Example"s;
		```
	- other:

		1\. if the data type of the variable is part of the STL.
		```c
		std::filesystem::path pathName = { };
		std::ifstream ifsName = { };
		std::ofstream ofsName = { };
		```
		2\. if the data type of the variable is part of the WinAPI.
		```c
		DWORD dwName = 0UL;
		WORD wName = 0U;
		BYTE dName = 0U;
		WPARAM wName = 0U;
		LPARAM lName = 0L;
		HRESULT hName = 0L;
		LRESULT lName = 0L;
		```
2. if variable defined as `auto` type, and final type is known, it still requires to include hungarian notation.
```c
auto iName = 0;
```
3. if none of the conditions are met, then hungarian notation is redundant.
```c
Unknown_t unknownName = { };
```
### structure
1. must be suffixed with '\_t' to specify that it's an structure.
```c
struct same_s;
```
### class
1. if class is interface (have virtual table and doesn't have variables), then it must be prefixed with 'I'.
```c
class IName;
```
2. must be prefixed with 'C' to specify that it's an class.
```c
class CName;
```
### enumeration
1. must be prefixed with 'E' to specify that it's an enumeration.
```c
enum EName : int { };
```
### macro
1. the name must be written in *UPPER_SNAKE_CASE*.
```c
#define Q_NAME
```
2. if the macro is defined in a particular module/header, then it must be prefixed with at least one letter of the module/header name.

<sub>math.h</sub>
```c
#define M_NAME
```
3. arguments must be written in *UPPER_SNAKE_CASE*.
```c
#define Q_STRINGIFY(PLAIN_NAME) #PLAIN_NAME
```