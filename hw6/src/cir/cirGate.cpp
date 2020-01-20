/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/


// ==================================================
// = AIG(19), line 12                               =
// ==================================================

void
CirGate::reportGate() const
{
	cout << "==================================================" << endl;
	cout << "= " << gateType << "(" << gateID << ")";

  int namelen = 0;
	if(name != "")
  {
		cout << '"' << name << '"';
    namelen = name.length()+2;
  }

	cout << ", line " << lineNo;


  string gateIDstr = to_string(gateID);
  string lineNostr = to_string(lineNo);

  int count = (gateIDstr+lineNostr+gateType).length()+namelen+11;
  if(count >= 49)
    cout << " ";
  else
  {
    for(int i = 0; i < 49-count; i++)
      cout << " ";  
  }
  

  cout << "=" << endl;
	cout << "==================================================" << endl;
}


// PO 8
//   AIG 3
//     AIG 6
//       PI 1
//       PI 2
//     !AIG 7
//       !PI 1
//       !PI 2

vector<int> oldGate; //literal num

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
   oldGate.clear();
   int Cnt = 1;
   Fanin(level, Cnt);
}
void
CirGate::Fanin(int level, int Cnt) const
{
   assert (level >= 0);
   
   if(level == 0)
   {
      cout << gateType << " " << gateID << endl;
      // oldGate.push_back(gateID);
      return;
   }


   cout << gateType << " " << gateID;
   

   for(int i = 0; i < oldGate.size(); i++)
   {
      if(gateID == oldGate[i])
      {
        cout << " (*)" << endl;
        return;
      }
   }
   


   cout << endl;
   oldGate.push_back(gateID);


   for(int i = 0; i < fanin.size(); i++)
   {
      
      for(int j = 0; j < Cnt ; j++)
        cout << "  ";

      if(faninInv[i])//is inverted
        cout << "!";

      fanin[i]->Fanin(level-1, Cnt+1);
   }

}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
   int Cnt = 1;
   oldGate.clear();
   Fanout(level, Cnt);
}
void
CirGate::Fanout(int level, int Cnt) const
{
   assert (level >= 0);

   if(level == 0)
   {
      cout << gateType << " " << gateID << endl;
      oldGate.push_back(gateID);
      return;
   }

   cout << gateType << " " << gateID;
   

   for(int i = 0; i < oldGate.size(); i++)
   {
      if(gateID == oldGate[i])
      {
        cout << " (*)" << endl;
        return;
      }
   }
   


   cout << endl;
   oldGate.push_back(gateID);


   for(int i = 0; i < fanout.size(); i++)
   {
      
      for(int j = 0; j < Cnt ; j++)
        cout << "  ";

      if(fanoutInv[i])//is inverted
        cout << "!";

      fanout[i]->Fanout(level-1, Cnt+1);
   }
   
}
