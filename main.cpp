#include <iostream>
#include <fstream>
#include <string>

std::string getHeaderName(std::string &filePath)
{
    //separate filename
    auto begin {filePath.find_last_of("/\\") + 1};
    std::string name {filePath.substr(begin)};

    for(char &c: name)
    {
        //replace all non alphadigits with underscore
        if(!std::isalnum(c))
            c = '_';
        //make everything Uppercase
        else
            c = (char) toupper(c);
    }

    //add _H to the End
    name += "_H";

    return name;
}

//removes directory and type from name and converts it to a valid c name
std::string getFileNameWithoutType(std::string &filePath)
{
    //separate filename
    auto begin {filePath.find_last_of("/\\") + 1};
    auto end {filePath.find_last_of('.')};
    std::string name {filePath.substr(begin, end-begin)};

    //add underscore if first char is a digit
    if (std::isdigit(name[0]))
        name.insert(0, "_");

    //replace characters not allowed in C names with underscore
    for(char &c: name)
    {
        if(!std::isalnum(c))
            c = '_';
    }

    return name;
}

std::string getPrefix(std::string &filePath)
{
    return "//\n"
           "// Created by fileString\n"
           "//\n"
           "\n#ifndef " + getHeaderName(filePath) +
           "\n#define " + getHeaderName(filePath) +
           "\n\n"
           "char " + getFileNameWithoutType(filePath) + "[] = \"";
}

std::string getSuffix(std::string &filePath)
{
    return "\";\n\n#endif //" + getHeaderName(filePath);
}

std::string transformCharC(char in)
{
    std::string out {};
    switch (in)
    {
        case '\n':
            out.append("\\n\"\n\"");
            break;
        case '\t':
            out.append("\\t");
            break;
        case '\v':
            out.append("\\v");
            break;
        case '\b':
            out.append("\\b");
            break;
        case '\r':
            out.append("\\r");
            break;
        case '\f':
            out.append("\\f");
            break;
        case '\a':
            out.append("\\a");
            break;
        case '\\':
            out.append("\\\\");
            break;
        case '?':
            out.append("\\?");
            break;
        case '\'':
            out.append("\\'");
            break;
        case '\"':
            out.append("\\\"");
            break;
        default:
            if(std::isprint(in))
                out.push_back(in);
            else
            {
                char tmp[32];
                sprintf(tmp, "\\x%2X", in & 0xFF);
                out.append(tmp);
            }
            break;
    }

    return out;
}

int main()
{
    std::string filePath {"tests/test.txt"};
    std::ifstream inFile {filePath};
    if (inFile.is_open())
    {
        std::cout << getPrefix(filePath);

        char c;
        while (!(inFile.get(c), inFile.eof()))
        {
            std::cout << transformCharC(c);
        }
        inFile.close();

        std::cout << getSuffix(filePath);
    }

    else std::cout << "Unable to open file";
    return 0;
}
