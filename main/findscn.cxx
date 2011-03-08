/* Copyright (c) 2008, detrox@gmail.com 
  modified by yakergong at gmail dot com
*/

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include <sys/time.h>
#include <sys/ioctl.h>
#include <getopt.h>

#include "../source/parser_factory.hxx"
#include "../source/custom_parser.hxx"
#include "../source/utf8.hxx"

using namespace std;

const char *g_config = "";
const char *g_parser = "crf_ner_nr";
bool  g_verbose = false;

bool g_linenum = false;
bool g_color = false;
bool g_ignore = false;
bool g_filename = false;

#define RED "\033[0;32;31m"
#define RED_END "\033[0m"

static string _token_format(string line, vector< bamboo::Token * >& vec)
{
    if (g_ignore) {
        stringstream ss;
        for(size_t j = 0; j < vec.size(); ++j) {
            unsigned short pos = vec[j]->get_pos();
            if(!pos)
                continue;
            if(g_color)
                ss << RED << vec[j]->get_orig_token() << RED_END; 
            else
                ss << vec[j]->get_orig_token(); 
            if(j != vec.size()-1)
                ss << ' ';
        }
        return ss.str();
    }
    else {
        string line_buf = line; 
        for(size_t j = 0; j < vec.size(); ++j) {
            unsigned short pos = vec[j]->get_pos();
            if(!pos)
                continue;
            string name_str = vec[j]->get_orig_token();
            stringstream ss;
            size_t found = 0;
            while(1) {
                size_t pos=line_buf.find(name_str,found);
                if(pos == string::npos ) {
                    ss << line_buf.substr(found);
                    break;
                }
                ss << line_buf.substr(found,pos-found);
                if(g_color)
                    ss << RED << name_str << RED_END;
                else
                    ss << name_str;
                found = pos + name_str.length();
            }
            line_buf = ss.str();
        }
        return line_buf;
    }
}

static void _do_text(bamboo::Parser *parser, string filename)
{
    unsigned long consume = 0;
    string line;
    int line_count = 1;
    istream *input;
    ifstream ifs;
    if(filename == "")
        input = &cin;
    else {
        ifs.open(filename.c_str(), ifstream::in);
        input = &ifs;
    }
    while(getline(*input, line)) {
        struct timeval tv[2];
        struct timezone tz;
        gettimeofday(&tv[0], &tz);
        vector < bamboo::Token * >vec;
        vec.clear();
        parser->setopt(BAMBOO_OPTION_TEXT, line.c_str());
        parser->parse(vec);
        gettimeofday(&tv[1], &tz);
        consume += (tv[1].tv_sec - tv[0].tv_sec) * 1000000 + (tv[1].tv_usec - tv[0].tv_usec);
        if(g_filename)
            cout << filename << ':';
        if(g_linenum)
            cout << line_count << ':';
        cout << _token_format(line, vec) << endl;
        ++line_count;
    }
    if(filename != "")
        ifs.close();
}


void show_help(char *argv[])
{
    cout << "Usage:" << endl
         << argv[0] << " [OPTION]... [FILE] ..." << endl
         << " -c Surround the found chinese name with red color." << endl
         << " -n Show line number." << endl
         << " -i ignore other words, only show name." << endl
         << "If no input file indicated, program will use stdin instead." << endl;
    exit(0);
}

int main(int argc, char *argv[])
{
    int opt;
    while((opt = getopt(argc, argv, "ncih")) != -1) 
    {
        switch(opt) {
        case 'n':
            g_linenum = true;
            break;
        case 'c':
            g_color = true;
            break;
        case 'i':
            g_ignore = true;
            break;
        case 'h':
            show_help(argv);
            exit(0);
            break;
        case '?':
            printf("ERROR:Invalid option: %c\n", optopt);
            exit(0);
            break;
        default:
            break;
        }
    }
    vector<string> filenames;
    for(; optind < argc; optind++) {
        filenames.push_back(argv[optind]);
    }        

    try {
        bamboo::Parser * parser;
        bamboo::ParserFactory * factory;
        factory = bamboo::ParserFactory::get_instance();
        parser = factory->create(g_parser, g_config, g_verbose);
        if (parser == NULL) {
            cout << "ERROR: Error loading parser:" << g_parser << endl;
            exit(1);
        }
        if(filenames.size() > 0) {
            if(filenames.size() > 1)
                g_filename = true;
            for(size_t i = 0; i < filenames.size(); ++i) {
                _do_text( parser, filenames[i] );
            }
        }
        else
            _do_text( parser, string("") );
    } 
    catch(std::exception & e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
