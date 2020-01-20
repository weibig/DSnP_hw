/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
   // TODO
   // Use try-catch to catch the bad_alloc exception

   vector<string> options;
   if (!CmdExec::lexOptions(option, options) || (options.size() != 1 && options.size() != 3))
   {
      return CmdExec::errorOption(CMD_OPT_EXTRA,option);
   }
   if(options.size() == 3 && myStrNCmp("-Array", options[1], 2) != 0) // mtn -a 3 1
   {
      string temp = options[2];
      options[2] = options[1];
      options[1] = options[0];
      options[0] = temp;
   }


   int num;
   if (!myStr2Int(options[0], num) || num <= 0) 
      return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
   bool isArr = false;
   int arrSize = 0;

   if(options.size() == 3)
   {
      isArr = true;

      if(myStrNCmp("-Array", options[1], 2) != 0)
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
      else if(!myStr2Int(options[2], arrSize) || arrSize <= 0)
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
   }
   else if(options.size() != 1)
      return CmdExec::errorOption(CMD_OPT_EXTRA,option);


   try
   {
      if(isArr == false)
         mtest.newObjs(num);
      else
         mtest.newArrs(num,arrSize);
   }
   catch(bad_alloc)
    {
        return CMD_EXEC_ERROR; ////
    }

   
   return CMD_EXEC_DONE;
}

void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
   // TODO

   // 一開始的ptr包含8byte(在8bytes前面)
   // return 不用加8 (系統會自動加)
   vector<string> options;
   if (!CmdExec::lexOptions(option, options)|| (options.size() != 2 && options.size() != 3))
      return CMD_EXEC_ERROR;


   if(options.size() == 3 && myStrNCmp("-Array", options[2], 2) != 0) // mtd -a 3 1
   {
      string temp = options[0];
      options[0] = options[1];
      options[1] = options[2];
      options[2] = temp;
   }



   int num;

   if(myStrNCmp("-Index", options[0], 2) == 0)
   {
      if (!myStr2Int(options[1], num) || num < 0) 
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);

      if(options.size() == 3) //array
      {
         if(myStrNCmp("-Array", options[2], 2) != 0)
           return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
         else if(num >= mtest.getArrListSize())
         {
            cerr << "Size of array list (" << mtest.getArrListSize() << ") is <= " << num << "!!" << endl;
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
         }
         mtest.deleteArr(num);
      }
      else //object
      {
         if(num >= mtest.getObjListSize())
         {
            cerr << "Size of object list (" << mtest.getObjListSize() << ") is <= " << num << "!!" << endl;
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
         }
         mtest.deleteObj(num);
      }
   }
   else if(myStrNCmp("-Random", options[0], 2) == 0)
   {
      if (!myStr2Int(options[1], num) || num <= 0) 
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
      

      if(options.size() == 3) //array
      {
         if(myStrNCmp("-Array", options[2], 2) != 0)
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
         else if(mtest.getArrListSize() == 0)
         {
            cerr << "Size of array list is 0!!" << endl;
            return CMD_EXEC_ERROR;
         }

         int temp;
         for(int i = 0; i < num; i++)
         {
            temp = rnGen(mtest.getArrListSize());

            // cout << i << " " << temp << endl;///
            mtest.deleteArr(temp);
         }
      }
      else //object
      {
         if(mtest.getObjListSize() == 0)
         {
            cerr << "Size of object list is 0!!" << endl;
            return CMD_EXEC_ERROR;
         }

         int temp;
         for(int i = 0; i < num; i++)
         {
            temp = rnGen(mtest.getObjListSize());
            mtest.deleteObj(temp);
         }
         
      }
   }
   else
     return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);

   return CMD_EXEC_DONE;
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


