/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include <fstream>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"
#include <assert.h>
#include <stdlib.h>

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine constant (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/

CirMgr::~CirMgr() {
  lineNo = 0;
}

bool
CirMgr::readCircuit(const string& fileName)
{
  fstream file;
  string word;
  int temp;
  lineNo = 1;

  file.open(fileName, ios::in);

  if(!file)
  {
    return false;
  }
  else
  {
    
    file >> word;

    for(int i = 0; i < 5; i++) 
    {
       //read Header lineNO=1
       file >> word;
       // cout << word << endl;///
       temp = stoi(word);
       header[i] = temp;
    }
    lineNo++;


// +++++++++++++++++++++++++++++++
    //read pi
    for(int i = 0; i < header[1]; i++)
    {
        file >> word;

        temp = stoi(word);
        CirGate* pi = new CirPiGate(temp/2, lineNo, "PI"); 
        addPiList(pi);
        // cout << "PI = " << pi << endl;
        lineNo++;
    }

    //read po
    for(int i = 0; i < header[3]; i++)
    {
        file >> word;
        CirGate* po = new CirPoGate(header[0]+1+i, lineNo, "PO");
        temp = stoi(word);


        po->faninInt.push_back(temp/2);
        po->faninInv.push_back(temp%2);

        addPoList(po);
        // cout << "PO = " << po << endl;
        lineNo++;
    }
    
    //read aig
    for(int i = 0; i < header[4]; i++)
    {
        file >> word;
        temp = stoi(word);
        CirGate* aig = new CirAigGate(temp/2, lineNo, "AIG");
        
        file >> word;
        temp = stoi(word);

        aig->faninInt.push_back(temp/2);
        aig->faninInv.push_back(temp%2);


        file >> word;
        temp = stoi(word);
        
        aig->faninInt.push_back(temp/2);
        aig->faninInv.push_back(temp%2);

        // cout << "AIG = " << aig << endl;
        addAigList(aig);
        lineNo++;


    }


// +++++++++++++++++++++++++++++++

    //read name & comment
    while(file >> word)
    {
        bool isComment = false;
        bool isPI = false;
        int index;

        if(word[0] == 'i')
          isPI = true;
        else if (word[0] == 'c')
          isComment = true;
        // cout << "ha" << endl;

        if(isComment == false)
        {
            word.erase(word.begin());
            index = stoi(word);

            // cout << word << " " << index << endl;
            string name;
            getline(file,name);
            name.erase(name.begin());

            if(isPI == true)
               _piList[index]->changeName(name);
            else
               _poList[index]->changeName(name);

        }
        else
            while(file >> word){}
          

    }
    for(int j = 0; j < _poList.size(); j++)
    {
      
      for(int i = 0; i < _poList[j]->faninInt.size(); i++)
      {
        

        if(_poList[j]->faninInt[i] == 0)
        {
          // cout << "CONST0" << endl;
          CirGate *tmp = new CirConstGate(_poList[j]->faninInt[i], 0, "CONST");
          _poList[j]->addFanin(tmp);
          addConstList(tmp);
        }
        else
        {
          // cout << getGate(_poList[j]->faninInt[i]) << " " << _poList[j]->faninInt[i] << endl;
          if(getGate(_poList[j]->faninInt[i]) != 0)
          {
            _poList[j]->addFanin(getGate(_poList[j]->faninInt[i]));
          }
          else
          {
            CirGate *tmp = new CirConstGate(_poList[j]->faninInt[i], 0, "UNDEF");
            _poList[j]->addFanin(tmp);
            addConstList(tmp);
            // cout << "hey1" << endl;
          }
        }
      }
      // cout << "_poList[j]->fanin.size() = " << _poList[j]->fanin.size() << endl;
      for(int i = 0; i < _poList[j]->fanin.size(); i++) 
      {
        if(_poList[j]->fanin[i]->gateType != "CONST")
          connect(_poList[j]->fanin[i], _poList[j], _poList[j]->faninInv[i]);
      }
    }
    // cout << ":))" << endl;
    for(int j = 0; j < _aigList.size(); j++)
    {
      
      for(int i = 0; i < _aigList[j]->faninInt.size(); i++)
      {
        

        if(_aigList[j]->faninInt[i] == 0)
        {
          // cout << "CONST0" << endl;
          if(_constList.size() == 0)
          {
            CirGate *tmp = new CirConstGate(_aigList[j]->faninInt[i], 0, "CONST");
            addConstList(tmp);
          }
          _aigList[j]->addFanin(_constList[0]);
          
        }
        else
        {
          // cout << getGate(_aigList[j]->faninInt[i]) << " " << _aigList[j]->faninInt[i] << endl;
          if(getGate(_aigList[j]->faninInt[i]) != 0)
          {
            _aigList[j]->addFanin(getGate(_aigList[j]->faninInt[i]));
          }
          else
          {
            CirGate *tmp = new CirConstGate(_aigList[j]->faninInt[i], 0, "UNDEF"); //Gates with floating fanin(s)
            _aigList[j]->addFanin(tmp);
            // cout << "b: " << _constList.size() << endl;
            addConstList(tmp);
            // cout << "a: " << _constList.size() << endl;
            // cout << "hey2" << endl;

          }
        }
      }
      // cout << "_aigList[j]->fanin.size() = " << _aigList[j]->fanin.size() << endl;
      for(int i = 0; i < _aigList[j]->fanin.size(); i++) // _poList[j]->fanin.size() = 0
      {
        if(_aigList[j]->fanin[i]->gateType != "CONST")
          connect(_aigList[j]->fanin[i], _aigList[j], _aigList[j]->faninInv[i]);
      }
    }
  }
   return true;
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
    cout << endl << "Circuit Statistics" << endl;
    cout << "==================" << endl;
    cout << "  " << "PI" << right << setw(12) << PiListLen() << endl;
    cout << "  " << "PO" << right << setw(12) << PoListLen() << endl;
    cout << "  " << "AIG" << right << setw(11) << AigListLen() << endl;
    cout << "------------------" << endl;
    cout << "  " << "Total" << right << setw(9) << PiListLen()+PoListLen()+AigListLen() << endl;

    assert(PiListLen() == header[1]);
    assert(PoListLen() == header[3]);
    assert(AigListLen() == header[4]);

}

CirGate* 
CirMgr::getGate(unsigned gid) const 
{
  for(int i = 0; i < PiListLen(); i++)
  {
     // cout << _piList[i]->getGateID() << endl;
     if(_piList[i]->getGateID() == gid)
        return _piList[i];
  }
  for(int i = 0; i < PoListLen(); i++)
  {
     // cout << _poList[i]->getGateID()<< endl;
     if(_poList[i]->getGateID() == gid)
        return _poList[i];
  }
  for(int i = 0; i < AigListLen(); i++)
  {
     // cout << _aigList[i]->getGateID()<< endl;
     if(_aigList[i]->getGateID() == gid)
        return _aigList[i];
  }
  for(int i = 0; i < _constList.size(); i++)
  {
     // cout << _constList[i]->getGateID()<< endl;
     if(_constList[i]->getGateID() == gid)
        return _constList[i];
  }
  return 0; 
}

void
CirMgr::printNetlist() 
{
    cout << endl;
    int index = 0;
    genDFSList();
    for(int i = 0; i < _dfsList.size(); i++)
    {
       cout << "[" << index++ << "] ";
       if(_dfsList[i]->gateType == "PI")
       {
          cout << "PI  " << _dfsList[i]->getGateID();
       }
       else if(_dfsList[i]->gateType == "AIG")
       {
          cout << "AIG " << _dfsList[i]->getGateID();

          for(int j = 0; j < _dfsList[i]->fanin.size(); j++)
          {
              cout << " ";
              if(_dfsList[i]->fanin[j]->gateType == "UNDEF")
                cout << "*";

              if(_dfsList[i]->faninInv[j])
                cout << "!";
              cout << _dfsList[i]->fanin[j]->getGateID();
          }
       }
       else if(_dfsList[i]->gateType == "PO")
       {
          cout << "PO  " << _dfsList[i]->getGateID();
          for(int j = 0; j < _dfsList[i]->fanin.size(); j++)
          {
              cout << " ";
              if(_dfsList[i]->fanin[j]->gateType == "UNDEF")
                cout << "*";

              if(_dfsList[i]->faninInv[j])
                cout << "!";
              cout << _dfsList[i]->fanin[j]->getGateID();
          }
       }
       else if(_dfsList[i]->gateType == "CONST")
       {
          cout << "CONST0";
       }
       if(_dfsList[i]->name != "")
          cout << " (" << _dfsList[i]->name << ")";
       

       cout << endl;
    }
    
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit:";
   for(int i = 0; i < PiListLen(); i++)
      cout << " " << _piList[i]->getGateID();

   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit:";
   for(int i = 0; i < PoListLen(); i++)
      cout << " " << _poList[i]->getGateID();

   cout << endl;
}
void
CirMgr::genDFSList()
{
  //initialize
  for(int i = 0; i < PoListLen(); i++)
      _poList[i]->visited = false;
  for(int i = 0; i < PiListLen(); i++)
      _piList[i]->visited = false;
  for(int i = 0; i < AigListLen(); i++)
      _aigList[i]->visited = false;


  for(int i = 0; i < PoListLen(); i++)
  {
      addDFS(_poList[i]);
  }
}

void 
CirMgr::addDFS(CirGate* root)
{
   if(root == NULL)
      return;

   for(int i = 0; i < root->fanin.size(); i++)
   {
      if(root->fanin[i]->gateType != "UNDEF" && root->fanin[i]->visited == false)
      {
        addDFS(root->fanin[i]);
        root->fanin[i]->visited = true;
      }
   }
   _dfsList.push_back(root);
}

void 
CirMgr::connect(CirGate* A, CirGate* B, bool isInv)
{
  A->addFanout(B);
  A->addFanoutInv(isInv);
}


void
CirMgr::printFloatGates() const
{
  bool found1 = false;
  bool found2 = false;
  bool *hasfloating = new bool [header[0]+header[3]+1]();
  bool *notused = new bool [header[0]+header[3]+1]();

  // cout << _constList[0]->gateType << endl; 

  for(int i = 0; i < _constList.size(); i++)
  {
    // cout << i << ": " << _constList[i]->getGateID() << endl;
    if(_constList[i]->gateType == "UNDEF")
    {
      for(int j = 0; j < _constList[i]->fanout.size(); j++)
      {
        hasfloating[_constList[i]->fanout[j]->getGateID()] = true;
        found1 = true;
        // cout << "found1 " << hasfloating[_constList[i]->fanout[j]->getGateID()] << endl;
      }
    }
  }
  for(int i = 0; i < PiListLen(); i++)
  {
    if(_piList[i]->fanout.size() == 0)
    {
      notused[_piList[i]->getGateID()] = true;
      found2 = true;
    }
  }
  for(int i = 0; i < AigListLen(); i++)
  {
    if(_aigList[i]->fanout.size() == 0)
    {
      notused[_aigList[i]->getGateID()] = true;
      found2 = true;
    }
  }

  if(found1 == true)
  {
     cout << "Gates with floating fanin(s):"; //Gates defined but not used  :

     for(int i = 0; i <= header[0]+header[3]; i++)
     {
        if(hasfloating[i] == true)
          cout << " " << i;
     }
     cout << endl;
  }
  if(found2 == true)
  {
     cout << "Gates defined but not used  :";
     for(int i = 0; i <= header[0]+header[3]; i++)
     {
        if(notused[i] == true)
          cout << " " << i;
     }
     cout << endl;
  }
}

void
CirMgr::writeAag(ostream& outfile)
{
    //write header
    cout << "aag";
    for(int i = 0; i < 5; i++)
       cout << " " << header[i];

    cout << endl;

    int tmp;
    //write pi po
    for(int i = 0; i < _piList.size(); i++)
    {
       cout << _piList[i]->getGateID()*2 << endl;
    }

    for(int i = 0; i < _poList.size(); i++)
    {
       tmp = _poList[i]->fanin[0]->getGateID()*2 + (int)_poList[i]->faninInv[0];
       cout << tmp << endl;
    }

    //write aig
    // genDFSList();
    for(int i = 0; i < _dfsList.size(); i++)
    {
       // cout << _dfsList[i]->gateType << " ";
       if(_dfsList[i]->gateType == "AIG")
       {
           cout << _dfsList[i]->getGateID()*2;

           for(int j = 0; j < _dfsList[i]->fanin.size(); j++)
           {
              tmp = _dfsList[i]->fanin[j]->getGateID()*2 + (int)_dfsList[i]->faninInv[j];
              cout << " " << tmp;
           }

           cout << endl;
       }

    }

    //write name
    for(int i = 0; i < _piList.size(); i++)
    {
       if(_piList[i]->name != "")
       {
          cout << "i" << i << " " << _piList[i]->name << endl;
       }
    }

    for(int i = 0; i < _poList.size(); i++)
    {
       if(_poList[i]->name != "")
       {
          cout << "o" << i << " " << _poList[i]->name << endl;
       }
    }
}
