#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <streambuf>
#include <ctype.h>

using namespace std;

regex bold ("\\*\\*.*?\\*\\*");
regex italic ("_.*?_");
regex h1 (".*?(=)+\n");
regex h_ ("\n(#)+?.*?\n");
regex u_list("(\\*[ ].*?\n)+");
regex u_list_end("\\*[ ].*?\n[^\\*]");
regex o_list("(\\d\\.[ ].*?\n)+");
regex o_list_end("\\d\\.[ ].*?\n[!\\d]");

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
    }
}

void parseForItalic(string& text)
{
    while (regex_search(text, italic))
    {
        text.replace(text.find("_"), 2, "<em>");
        text.replace(text.find("_"), 2, "</em>");
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
void parseForU_Lists(string& text)
{
    auto lists_count = distance(sregex_iterator(text.begin(), text.end(), u_list), sregex_iterator());
    for(auto i = 1; i <= lists_count; ++i)
    {
        size_t pos = 0;
        string _list = "", list_elem = "";
        while(!regex_search(list_elem, u_list_end))
        {
            list_elem = text.substr(text.find("\n* ", pos), (text.find("\n", (text.find("\n* ", pos) + 1)) - text.find("\n* ", pos)) + 1);
            char c = text.at(text.find("\n", text.find("\n* ", pos) + 1) + 1);
            if(c != '*')
                list_elem += c;
            _list += list_elem;
            pos = text.find("\n", text.find("\n* ", pos) + 1);
        }
        if(_list.at(_list.size() - 1) != '\n')
            _list.pop_back();
        size_t list_length = _list.length();
        while(_list.find("\n* ") != string::npos)
        {
            _list.replace(_list.find("\n* "), 3, "<li>");
        }
        _list.insert(0, "\n<ul>\n");
        _list.append("</ul>\n");
        text.replace(text.find("\n* "), list_length-1, _list);
    }
}
/*
void parseForO_Lists(string& text)
{
    auto lists_count = distance(sregex_iterator(text.begin(), text.end(), o_list), sregex_iterator());
    for(auto i = 1; i <= lists_count; ++i)
    {
        size_t pos = 0;
        string _list = "", list_elem = "";
        while(!regex_search(list_elem, o_list_end))
        {
            list_elem = text.substr(text.find("\n* ", pos), (text.find("\n", (text.find("\n* ", pos) + 1)) - text.find("\n* ", pos)) + 1);
            char c = text.at(text.find("\n", text.find("\n* ", pos) + 1) + 1);
            if(!(isdigit(c)))
                list_elem += c;
            _list += list_elem;
            pos = text.find("\n", text.find("\n* ", pos) + 1);
        }
        if(_list.at(_list.size() - 1) != '\n')
            _list.pop_back();
        size_t list_length = _list.length();
        _list = regex_replace(_list, regex("([0-9]*[0-9]+)\\.[ ]"), "<li>");
        _list.insert(0, "\n<ol>\n");
        _list.append("</ol>\n");
        text.replace(text.find("\n* "), list_length-1, _list);
    }
}
*/
void parseForClosedLi(string& text)
{
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
    parseForU_Lists(text);
    //parseForO_Lists(text);
    parseForClosedLi(text);
    cout << text << endl;
    return 0;
}
