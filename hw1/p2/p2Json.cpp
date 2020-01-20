/****************************************************************************
  FileName     [ p2Json.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Json and JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include "p2Json.h"
#include <fstream>
#include <iomanip>


using namespace std;

// Implement member functions of class Row and Table here
bool Json::read(const string& jsonFile)
{
  fstream file;
  string word;
  string buffer_key;
  int buffer_value;
  bool lack_value = false;
  bool uncomplete_word = false;

  int store_index = 0;
  

  file.open(jsonFile, ios::in);


  if(!file)
  {
    // cout << "cannot open the file";
    return false;
  }
  else
  {
    while (file >> word) 
    { 
      // cout << word << endl;

      if (word.compare("{") == 0 || word.compare("}") == 0 || word.compare(":") == 0 || word.compare(",") == 0){}
      else if (word[0] == '"' && uncomplete_word == false) 
      {
         // cout << "@";
         if(word[word.size()-1] == '"')//complete key
         {
           word = word.substr(1, word.size() - 2);
        
         buffer_key = word;
         lack_value = true;
         // cout << "QQ";

         // cout << "2" << endl;
         }
         else
         {//not done yet
            word = word.substr(1, word.size() - 1);
            buffer_key = word;
            uncomplete_word = true;
            // cout << word << "!!";


         }

        
      }
      else if (lack_value == true)
      {
         if(word[ word.size()-1 ] == ',')
         {
            // cout << ", is here!\n";
            // cout << word << "!!";
            word = word.substr(0, word.size() - 1);
            // cout << word << "!!";
         }

         JsonElem buffer_elem(buffer_key,stoi(word));
         this-> _obj.push_back(buffer_elem);
         lack_value = false;
         store_index++;
         // cout << "3" << endl;
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
            // cout << word << "~~\n";
         }
         else
         {
            buffer_key += " ";
            buffer_key += word;
         }
      }
    }
    // cout << store_index << endl;

    // while(getline(file,word))
    // {
    //    cout << word << endl;
    //    if (word.compare("{") >= 0 || word.compare("}") >= 0 ){}
    //    else
    //    {
    //        int pos = word.find('"');
    //        int pos2 = word.find('"', pos+1);

    //        buffer_key = word.substr(pos+1, pos2-pos-1);
           


    //        int pos3 = word.find(":");   
    //        string value = word.substr(pos3 + 1);    // after :


    //        buffer_value = stoi(value);

    //        JsonElem buffer_elem(buffer_key,buffer_value);
    //        this-> _obj.push_back(buffer_elem);
    //     }

    // }
    file.close();
    return true;
  }
}

void Json::print()
{
   cout << "{" << endl;
   for(int i = 0; i < _obj.size(); i++)
   {
      cout << "  ";
      cout << '"' << _obj[i].getKey() << '"' << " : " << _obj[i].getValue();

      if(i != _obj.size() - 1)
        cout << ",";

      cout << endl;

   }
   cout << "}" << endl;

}


int Json::sum()
{
   int sum = 0;
   if(_obj.size() == 0)
   {
      cerr << "Error: No element found!!" << endl;
   }
   else
   {
      for(int i = 0; i < _obj.size(); i++)
      {
         sum += _obj[i].getValue();
   
      }
      cout << "The summation of the values is: " << sum;
      cout << "." << endl;
    }
    return sum;

}

double Json::ave()
{
   int sum = 0;
   if(_obj.size() == 0)
   {
      cerr << "Error: No element found!!" << endl;
   }
   else
   {
      for(int i = 0; i < _obj.size(); i++)
      {
         sum += _obj[i].getValue();
   
      }
      // cout << sum << " " << _obj.size() << endl;
      cout << "The average of the values is: ";
      double ave;
      ave = (double)sum / (double) _obj.size();
      cout  << fixed  <<  setprecision(1) << ave;
      cout << "." << endl;
      return ave;
    }

}

int Json::max()
{
   int max = -2147483648;
   int obj_index = -1;
   if(_obj.size() == 0)
   {
      cerr << "Error: No element found!!" << endl;
   }
   else
   {
      for(int i = 0; i < _obj.size(); i++)
      {
         if(_obj[i].getValue() > max)
         {
             max = _obj[i].getValue();
             obj_index = i;
         }
   
      }
      cout << "The maximum element is: { \"" << _obj[obj_index].getKey() << "\" : " << _obj[obj_index].getValue() << " }.";
      cout << endl;
    }

   
   return obj_index;

}

int Json::min()
{
   int min = 2147483647;
   int obj_index = -1;
   if(_obj.size() == 0)
   {
      cerr << "Error: No element found!!" << endl;
   }
   else
   {
      for(int i = 0; i < _obj.size(); i++)
       {
          if(_obj[i].getValue() < min)
          {
              min = _obj[i].getValue();
              obj_index = i;
          }
       }
       cout << "The minimum element is: { \"" << _obj[obj_index].getKey() << "\" : " << _obj[obj_index].getValue() << " }.";
       cout << endl;
       return obj_index;
   }
}

void Json::add()
{
   string add_key;
   int add_value;
   cin >> add_key >> add_value;

   JsonElem add_elem(add_key,add_value);
   this-> _obj.push_back(add_elem);
}



ostream&
operator << (ostream& os, const JsonElem& j)
{
   return (os << "\"" << j._key << "\" : " << j._value);
}

