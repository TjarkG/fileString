//
// Created by fileString: https://github.com/TjarkG/fileString
//

#ifndef TEST_TXT_H
#define TEST_TXT_H

#include <string>

const std::string test {"This is a testfile\n"
"\ttest\n"
"//\n"
"\\r\n"
"%\n"
"\xc3\xb6\xc3\xa4\xc3\xbc\n"
"\'test\"\n"
""};

#endif //TEST_TXT_H
