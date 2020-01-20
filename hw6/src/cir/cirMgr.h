/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
  friend class CirGate;
public:
   CirMgr(){}
   ~CirMgr();

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const;

   // Member functions about circuit construction
   bool readCircuit(const string&);////
   void addPiList(CirGate* x){ _piList.push_back(x);}
   void addPoList(CirGate* x){ _poList.push_back(x);}
   void addAigList(CirGate* x){ _aigList.push_back(x);}
   void addConstList(CirGate* x){ _constList.push_back(x);}

   int PiListLen() const {return _piList.size();}
   int PoListLen() const {return _poList.size();}
   int AigListLen() const {return _aigList.size();}

   void genDFSList();
   void addDFS(CirGate* root);

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() ;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&);

   void connect(CirGate* A, CirGate* B, bool isInv);
   

private:
   int header[5];
   vector<CirGate*> _piList;
   vector<CirGate*> _poList;
   vector<CirGate*> _aigList;

   vector<CirGate*> _dfsList;

   vector<CirGate*> _constList;

   
};

#endif // CIR_MGR_H
