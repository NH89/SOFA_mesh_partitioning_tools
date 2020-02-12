

#include <boost/foreach.hpp>
#include <iostream>
#include <fstream>
using namespace std;

  
// from https://theboostcpplibraries.com/boost.regex

#include <boost/regex.hpp>
#include <string>
#include <iostream>

bool substring_match(std::string e, std::string s){
    boost::regex expr(e);
    return boost::regex_match(s, expr);
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}



int main()
{
    // 1st call gmsh to generate out.msh from out.mesh
    
    
///////////////////////////////////////////////
    
    // parse out.mesh
    
    ifstream in("file.txt");
    if(!in) { cout << "Cannot open input file.\n"; return 1; }
    string str;
    ostringstream ss;
    ss << in.rdbuf(); // reading data
    in.close();
    
    str = ss.str();
    bool elems = 0;
    
    std::vector<std::string> strings=split(str, '\n'); // split into vector of one string per line
    
    std::vector<std::pair<int,int>> elemPartition;
    
    BOOST_FOREACH(std::string line, strings )
    {
        if( substring_match(line, "$Elements") ) {elems = 1;}
        else if (substring_match(line, "$") ) {elems = 0;};
        if(!elems) continue;                                    // parse only the elems section of the file 
        
        std::vector<std::string> tokens=split(line, ' ');       //split the line into space-separated tokens 
        if(tokens.at(1) != "4") continue;                       // parse only tetrahedral elements
        
        //int numTags = stoi(tokens.at(2));
        int elemNum = stoi(tokens.at(0));
        int partition = stoi(tokens.at(4));
        std::pair<int,int> _elem(elemNum,partition);
        elemPartition.push_back(_elem);
    }
    
    // sort elements of elemPartition by elemPartionion.first , and check that every element is filled.
    // find Young's modulus for each partition.
    
    // NB materials can also have per element (i) plastic deformation properties (_plasticMaxThreshold, _plasticYieldThreshold, _plasticCreep). (Could be used in morphogenesis), (ii) VonMisesStress, (ductile plasticity) (iii) Poisson ratio.
    
    
    // write vector to file // ? what file format is needed by Sofa ? Needed as a string of space-separated YM values to insert into pyscn.
    
    

  return 0;
}
  
