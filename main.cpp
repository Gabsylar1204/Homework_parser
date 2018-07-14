#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <streambuf>

using namespace std;

regex bold ("\\*\\*.*?\\*\\*");
regex italic ("_.*?_");
regex h1 (".*?(=)+\n");
regex h_ ("\n(#)+?.*?\n");
regex u_list("(\\*[' '].*?\n)+");
regex o_list("(\\d\\.[ ].*?\n)+");

string ToString(size_t sz) {

  stringstream ss;

  ss << sz;

  return ss.str();
}

void parseForBold(string& text)
{
    while (regex_search(text, bold))
    {
        text.replace(text.find("**"), 2, "<strong>");
        text.replace(text.find("**"), 2, "</strong>");
        //parse(text);
    }
}

void parseForItalic(string& text)
{
    while (regex_search(text, italic))
    {
        text.replace(text.find("_"), 2, "<em>");
        text.replace(text.find("_"), 2, "</em>");
        //parse(text);
    }
}

void parseForHeaders(string& text)
{
    if(regex_search(text, h1))
    {
        size_t header_end = text.find("\n=");
        size_t header_length = text.find("=\n") - header_end;
        text.insert(header_end, "</h1>");
        text.insert(header_end - header_length, "<h1>");
        text.erase(text.find("\n=")+1, header_length);
    }
    while (regex_search(text, h_))
    {
        size_t level = text.find("# ")+1 - text.find("#");
        string header = "<h" + ToString(level) + ">",
        closed_header = "</h" + ToString(level) + ">";
        text.replace(text.find("#"), level+1, header);
        text.insert(text.find("\n", text.find(header)), closed_header);
    }
}
void parseForLists(string& text)
{
    while(regex_search(text, u_list))
        text.replace(text.find("* "), 2, "<li>");
    if (regex_search(text, o_list))
        text = regex_replace(text, regex("([0-9]*[0-9]+)\\.[ ]"), "<li>");
    size_t pos = 0;
    while(text.find("<li>", pos) != string::npos)
    {
        text.insert(text.find("\n", text.find("<li>", pos)), "</li>");
        pos = text.find("<li>", pos)+3;
    }
}

int main()
{
    std::ifstream f("test.txt");
    std::string text;

    f.seekg(0, std::ios::end);
    text.reserve(f.tellg());
    f.seekg(0, std::ios::beg);

    text.assign((std::istreambuf_iterator<char>(f)),
                std::istreambuf_iterator<char>());
    parseForBold(text);
    parseForItalic(text);
    parseForHeaders(text);
    parseForLists(text);
    cout << text << endl;
    return 0;
}
