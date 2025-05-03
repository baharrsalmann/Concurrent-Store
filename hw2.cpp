#include <iostream>

#include "monitor.h"
using namespace std;
enum ItemType { AAA, BBB, CCC };

/* You can define any global variable
   function and class definition here.
   Programs will first call initStore() then
   all other functions
*/
class StoreMonitor : public Monitor {
   public:
      int cap[3];
      int avail[3];
      int reserved[3];
      int maxOrder;

      Condition customerCond;
      Condition supplierCondAAA;
      Condition supplierCondBBB;
      Condition supplierCondCCC;

      StoreMonitor(int capA, int capB, int capC, int mOrder) 
        : customerCond(this), 
          supplierCondAAA(this), 
          supplierCondBBB(this), 
          supplierCondCCC(this) 
    {
        cap[AAA] = capA;
        cap[BBB] = capB;
        cap[CCC] = capC;
        avail[AAA] = capA;
        avail[BBB] = capB;
        avail[CCC] = capC;
        reserved[AAA] = 0;
        reserved[BBB] = 0;
        reserved[CCC] = 0;
        maxOrder = mOrder;
    }

    void buyMonitor(int aA, int aB, int aC){
         __synchronized__;

         if(aA>maxOrder || aB>maxOrder || aC>maxOrder){
            return;
         }
         while (aA > avail[AAA] || aB > avail[BBB] || aC > avail[CCC]) {
            customerCond.wait();
        }

        avail[AAA] -= aA;
        avail[BBB] -= aB;
        avail[CCC] -= aC;

        supplierCondAAA.notifyAll();
        supplierCondBBB.notifyAll();
        supplierCondCCC.notifyAll();
    }

   void maysupplyMonitor(int itype, int n) {
      __synchronized__;

      if(itype == AAA){
         while (avail[itype] + reserved[itype] + n > cap[itype]) {
            supplierCondAAA.wait();
         }
      }
      else if(itype == BBB){
         while (avail[itype] + reserved[itype] + n > cap[itype]) {
            supplierCondBBB.wait();
         }
      }
      else if(itype == CCC){
         while (avail[itype] + reserved[itype] + n > cap[itype]) {
            supplierCondCCC.wait();
         }
      }
      else return;

      reserved[itype] += n;
   }

   
   void supplyMonitor(int itype, int n){
      __synchronized__;

      reserved[itype] -= n;
      avail[itype] += n;

      customerCond.notifyAll();
   }

   void monitorStore(int c[3], int a[3]) {
      __synchronized__;

      c[AAA] = cap[AAA];
      c[BBB] = cap[BBB];
      c[CCC] = cap[CCC];

      a[AAA] = avail[AAA];
      a[BBB] = avail[BBB];
      a[CCC] = avail[CCC];
    }

};

static StoreMonitor* store = nullptr;

void initStore(int cA, int cB, int cC, int mO) {
   store = new StoreMonitor(cA, cB, cC, mO);
}

void buy(int aA, int aB, int aC) {
   store->buyMonitor(aA, aB, aC);
}

void maysupply(int itype, int n) {
   store->maysupplyMonitor(itype, n);
}

void supply(int itype, int n) {
   store->supplyMonitor(itype, n);
}

void monitorStore(int c[3], int a[3]) {
   store->monitorStore(c, a);
}
