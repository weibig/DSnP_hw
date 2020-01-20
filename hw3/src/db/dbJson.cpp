/****************************************************************************
  FileName     [ dbJson.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Json member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <cmath>
#include <string>
#include <algorithm>
#include "dbJson.h"
#include "util.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream&
operator << (ostream& os, const DBJsonElem& j)
{
   os << "\"" << j._key << "\" : " << j._value;
   return os;
}

istream& operator >> (istream& is, DBJson& j)
{
   // TODO: to read in data from Json file and store them in a DB 
   // - You can assume the input file is with correct JSON file format
   // - NO NEED to handle error file format

  // ifs >> dbjson;
  assert(j._obj.empty());
  string word;
  string buffer_key;
  int buffer_value;
  bool lack_value = false;
  bool uncomplete_word = false;
  int store_index = 0;


  while(is >> word)
  {

    if (word.compare("{") == 0 || word.compare("}") == 0 || word.compare(":") == 0 || word.compare(",") == 0){}
    else if (word[0] == '"' && uncomplete_word == false) 
    {

       if(word[word.size()-1] == '"')//complete key
       {
         word = word.substr(1, word.size() - 2);
      
       buffer_key = word;
       lack_value = true;

       }
       else
       {//not done yet
          word = word.substr(1, word.size() - 1);
          buffer_key = word;
          uncomplete_word = true;
       }      
    }
    else if (lack_value == true)
    {
       if(word[ word.size()-1 ] == ',')
       {
          word = word.substr(0, word.size() - 1);
       }

       j.add(DBJsonElem(buffer_key,stoi(word)));

       lack_value = false;
       store_index++;
    }
    else if(uncomplete_word == true)
    {
       if(word[word.size()-1] == '"')
       {
          word = word.substr(0, word.size() - 1);
          buffer_key += " ";
          buffer_key += word;
          uncomplete_word = false;
          lack_value = true;
       }
       else
       {
          buffer_key += " ";
          buffer_key += word;
       }
    }
  }
  
   
  return is;
}

ostream& operator << (ostream& os, const DBJson& j)
{
   // TODO
   
   os << "{" << endl;
     for(int i = 0; i < j.size(); i++)
     {
        os << "  ";
        os << '"' << j[i].key() << '"' << " : " << j[i].value();

        if(i != j.size() - 1)
          os << ",";

        os << endl;

     }
     os << "}" << endl;

     return os;
}
  
/**********************************************/
/*   Member Functions for class DBJsonElem    */
/**********************************************/
/*****************************************/
/*   Member Functions for class DBJson   */
/*****************************************/
void
DBJson::reset()
{
   // TODO
   _obj.clear();
}

// return false if key is repeated
bool
DBJson::add(const DBJsonElem& elm)
{
   // TODO
  for(int i = 0; i < this->size(); i++)
  {
    if (_obj[i].key().compare(elm.key()) == 0)
      return false;
  }
  this-> _obj.push_back(elm);
    return true;
}

// return NAN if DBJson is empty
float
DBJson::ave() const
{
   // TODO
   int sum = 0;
   float ave;
   if(this->size() == 0)
   {
      return NAN;
   }
   else
   {
      for(int i = 0; i < this->size(); i++)
      {
         sum += _obj[i].value();
      }

      ave = (float)sum / (float) this->size();
    }

    return ave;
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::max(size_t& idx) const
{
   // TODO
   int maxN = INT_MIN;
   idx = 0;
   if(this->size() != 0)
   {
      for(int i = 0; i < this->size(); i++)
      {
         if(_obj[i].value() > maxN)
         {
             maxN = _obj[i].value();
             idx = i;
         }  
      }
    }
   return maxN;
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::min(size_t& idx) const
{
   // TODO
   int minN = INT_MAX;
   idx = 0;

   if(this->size() != 0)
   {
      for(int i = 0; i < this->size(); i++)
      {
         if(_obj[i].value() < minN)
         {
             minN = _obj[i].value();
             idx = i;
         }
   
      }
    }


   return  minN;
}

void
DBJson::sort(const DBSortKey& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

void
DBJson::sort(const DBSortValue& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

// return 0 if empty
int
DBJson::sum() const
{
   // TODO
   int s = 0;

   if(_obj.size() != 0)
   {
      for(int i = 0; i < _obj.size(); i++)
      {
         s += _obj[i].value();
      }
    }

   return s;
}
