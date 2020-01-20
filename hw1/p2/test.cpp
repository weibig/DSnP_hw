#include <iostream>
#include <string>
#include <fstream>
#include<vector>


using namespace std;


class JsonElem
{
public:
   // TODO: define constructor & member functions on your own
   JsonElem() {}
   JsonElem(const string& k, int v): _key(k), _value(v) {}

   friend ostream& operator << (ostream&, const JsonElem&);

private:
   string  _key;   // DO NOT change this definition. Use it to store key.
   int     _value; // DO NOT change this definition. Use it to store value.
};

class Json
{
public:
   // TODO: define constructor & member functions on your own
   bool read(const string&);
   void print();

private:
   vector<JsonElem>       _obj;  // DO NOT change this definition.
                                 // Use it to store JSON elements.
};



int main()
{
  fstream file;
  string word;
  string buffer_key;
  JsonElem buffer_elem;
  bool lack_value = false;

  int store_index = 0;
  

  // file.open("2.json", ios::in);
  
  // if(!file)
  //   cout << "cannot open the file";
  // else
  // {
  //   while (file >> word) 
  //   { 
  //     if(word == "{" || "}" || ":" || ","){}
  //     else if (word[0] == '"')
  //     {
  //       word = word.substr(1, word.size() - 2);
        
  //       buffer_key = word;
  //       lack_value = true;
  //     }
  //     else if (lack_value == true)
  //     {
  //        if(word[ word.size()-1 ] == ',')
  //           word = word.substr(0, word.size() - 2);

  //        buffer_elem(buffer_key,word);
  //        _obj.push_back(buffer_elem);
  //        lack_value == false;
  //        store_index++;
  //     }
  //   }
    
  //   file.close();
  // }


  string s = "Wei\" Cheng \" Chen : 124543, ";
  string b = "Chen";
  cout << s.compare(b);

}