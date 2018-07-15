#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

/*while(regex_search(text, u_list))
    {
        static size_t list_start = text.find("\n* ");
        while(regex.search(text, u_list_end))
        {
            text.replace(text.find("\n* "), 2, "\n<li>");
        }
    }
    if (regex_search(text, o_list))
        text = regex_replace(text, regex("([0-9]*[0-9]+)\\.[ ]"), "<li>");
    size_t pos = 0;
    while(text.find("<li>", pos) != string::npos)
    {
        text.insert(text.find("\n", text.find("<li>", pos)), "</li>");
        pos = text.find("<li>", pos)+3;
    }*/
#endif // PARSER_H_INCLUDED
