
// from https://theboostcpplibraries.com/boost.regex

#include <boost/regex.hpp>
#include <string>
#include <iostream>

bool substring_match(std::string e, std::string s){
    boost::regex expr(e);
    return boost::regex_match(s, expr);
}
