/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"
#include <assert.h>

using namespace std;

class CirGate;



//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate
{
  friend class CirMgr;
public:
   CirGate() {}
   virtual ~CirGate() {}
   CirGate(int g, int l, const string& s): gateID(g), lineNo(l), gateType(s){}

   // Basic access methods
   string getTypeStr() const { return gateType; }
   unsigned getLineNo() const { return lineNo; }
   int getGateID() const { return gateID;}
   void changeName(string a){name = a;}

   void addFanin(CirGate* ID){ fanin.push_back(ID);}
   void addFaninInv(bool isInv){ faninInv.push_back(isInv);}
   

   void addFanout(CirGate* ID){ fanout.push_back(ID);}
   void addFanoutInv(bool isInv){ fanoutInv.push_back(isInv);}

   
   // Printing functions
   virtual void printGate() const {cout << gateType << " " << gateID;}
   void reportGate() const;
   void reportFanin(int level) const;
   void Fanin(int level, int Cnt) const;
   void reportFanout(int level) const;
   void Fanout(int level, int Cnt) const;

private:
  int gateID;
  int lineNo;
  string name;
  string gateType;
  bool visited;

  vector<CirGate*> fanin;
  vector<bool> faninInv;
  vector<int> faninInt;

  vector<CirGate*> fanout;
  vector<bool> fanoutInv;

protected:

};

class CirPiGate : public CirGate
{
  friend class CirGate;
public:
  CirPiGate() {}
  ~CirPiGate() {}

  CirPiGate(int g, int l, const string& s): CirGate(g, l, s){}
  void printGate() const
    {cout << gateType << " " << gateID;}
  
private:
  int gateID;
  int lineNo;
  string name;
  string gateType;
};

class CirPoGate : public CirGate
{
  friend class CirGate;
public:
  CirPoGate() {}
  ~CirPoGate() {}

  CirPoGate(int g, int l, const string& s): CirGate(g, l, s){}
  void printGate() const
    {cout << gateType << " " << gateID;}
private:
  int gateID;
  int lineNo;
  string name;
  string gateType;
};

class CirAigGate : public CirGate
{
  friend class CirGate;
public:
  CirAigGate() {}
  ~CirAigGate() {}

  CirAigGate(int g, int l, const string& s): CirGate(g, l, s){}
  void printGate() const
    {cout << gateType << " " << gateID;}
private:
  int gateID;
  int lineNo;
  string gateType;
};

class CirConstGate : public CirGate
{
  friend class CirGate;
public:
  CirConstGate() {}
  ~CirConstGate() {}

  CirConstGate(int g, int l, const string& s): CirGate(g, l, s){}
  void printGate() const
    {cout << gateType << " " << gateID;}
private:
  int gateID;
  int lineNo;
  string gateType;
};

#endif // CIR_GATE_H
