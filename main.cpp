#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sysexits.h>
#include <filesystem>

enum class language
{C, Cpp};

//convert String to lowercase
std::string strToLower(std::string in)
{
    std::transform(in.begin(), in.end(), in.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return in;
}

std::string getHeaderName(const std::string &filePath)
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
std::string getCFileName(const std::string &filePath)
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

//get filename with extension
std::string getFilename(const std::string &filePath)
{
    //separate filename
    auto begin {filePath.find_last_of("/\\") + 1};
    return filePath.substr(begin);
}

std::string getPrefix(const std::string &filePath, enum language &lang)
{
    switch (lang)
    {
        case language::C:
            return "//\n"
                   "// Created by fileString: https://github.com/TjarkG/fileString\n"
                   "//\n"
                   "\n#ifndef " + getHeaderName(filePath) +
                   "\n#define " + getHeaderName(filePath) +
                   "\n\n"
                   "const char " + getCFileName(filePath) + "[] = \"";
        case language::Cpp:
            return "//\n"
                   "// Created by fileString: https://github.com/TjarkG/fileString\n"
                   "//\n"
                   "\n#ifndef " + getHeaderName(filePath) +
                   "\n#define " + getHeaderName(filePath) +
                   "\n\n#include <string>\n\n"
                   "const std::string " + getCFileName(filePath) + " {\"";
        default:
            return "\"";
    }
}

std::string getSuffix(const std::string &filePath, enum language &lang)
{
    switch (lang)
    {
        case language::C:
            return "\";\n\n#endif //" + getHeaderName(filePath) + "\n";
        case language::Cpp:
            return "\"};\n\n#endif //" + getHeaderName(filePath) + "\n";
        default:
            return "\"";
    }
}

std::string getFileEnding(enum language &lang)
{
    switch (lang)
    {
        case language::C:
            return ".h";
        case language::Cpp:
            return ".hpp";
        default:
            return "";
    }
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
                std::stringstream tmp;
                tmp << "\\x" << std::hex << (int) ((uint8_t) in);
                out.append(tmp.str());
            }
            break;
    }

    return out;
}

void printHelp()
{
    //TODO
    std::cout << "help\n";
}

int main(int argc, char **argv)
{
    bool verbose {false};
    bool help {argc == 1};  //show help if no arguments are supplied
    enum language lang {language::C};
    std::string outDir {};

    std::vector<std::string> const argList(argv + 1, argv + argc); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    std::vector<std::string> files {};

    for(int i = 0; i < argList.size(); ++i)
    {
        //make argument lowercase
        const auto argument {strToLower(argList[i])};

        if(argument == "-h")
            help = true;
        else if(argument == "-v")
            verbose = true;
        else if(argument == "-l")
        {
            ++i;
            if (i == (argc - 1))
            {
                std::cerr << "Error: -l argument without language specifier\n";
                exit(EX_USAGE);
            }

            const auto next {strToLower(argList[i])};
            if(next == "cpp" || next == "c++")
                lang = language::Cpp;
            else if (next == "c")
                lang = language::C;
            else
            {
                std::cerr << "Error: " << argList[i] << " is not a valid language specifier\n";
                exit(EX_USAGE);
            }
        }
        else if(argument == "-o")
        {
            ++i;
            if (i == (argc - 1))
            {
                std::cerr << "Error: -o argument without Output Path\n";
                exit(EX_USAGE);
            }

            std::filesystem::file_status const s {std::filesystem::status (argList[i])};
            if(std::filesystem::is_directory(s))
                outDir = argList[i];
            else
            {
                std::cerr << "Error: " << argList[i] << " is not a directory\n";
                exit(EX_IOERR);
            }
        }
        else
            files.push_back(argList[i]);
    }

    if(help)
        printHelp();
    if (verbose)
    {
        std::cout << "Language Mode: ";

        switch (lang)
        {
            case language::C:
                std::cout << "C";
                break;
            case language::Cpp:
                std::cout << "C++";
                break;
        }

        std::cout << "\n";
    }

    for (const auto& inPath: files)
    {
        std::string outPath {};
        if (outDir.empty())
            outPath = inPath + getFileEnding(lang);
        else
            outPath = outDir + "/" + getFilename(inPath) + getFileEnding(lang);

        std::ifstream inFile {inPath};
        std::ofstream outFile {outPath};

        if (inFile.is_open())
        {
            if (outFile.is_open())
            {
                outFile << getPrefix(inPath, lang);

                char c{};
                while (!(inFile.get(c), inFile.eof()))
                {
                    outFile << transformCharC(c);
                }
                inFile.close();

                outFile << getSuffix(inPath, lang);

                if (verbose)
                    std::cout << "Output Path: " << outPath << "\n";
            }
            else
            {
                std::cerr << "Error: Unable to open file " << outPath << "\n";
                exit(EX_IOERR);
            }
        }
        else
        {
            std::cerr << "Error: Unable to open file " << inPath << "\n";
            exit(EX_IOERR);
        }
    }

    return 0;
}
