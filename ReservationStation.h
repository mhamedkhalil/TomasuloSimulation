#include<iostream>
#include<vector>
using namespace std;

#define ROB_ENTRIES 6

struct ROBEntry 
{
    int id;                  
    string Op;              
    int dest;                 
    int value;                
    bool ready;               
    bool free = 1;               
};
vector<ROBEntry> ROB(ROB_ENTRIES);
int ROB_head = 0, ROB_tail = 0;

class reservationStation
{
    public:
    string Op;
    int Vj, Vk;
    ROBEntry* Qj;
    ROBEntry* Qk;
    int A;
    bool Busy;

    reservationStation()
    {
        Op = "";
        Vj = -1;
        Vk = -1;
        Qj = nullptr;
        Qk = nullptr;
        A  = -1;
        Busy  = 0;
    }

    bool execute()
    {
        if(this->Qj == NULL && this->Qk == NULL)
            return 1;
        return 0;
    }

    void flush()
    {
        this->Op = "";
        this->Vj = -1;
        this->Vk = -1;
        this->Qj = NULL;
        this->Qk = NULL;
        this->A  = -1;
        this->Busy  = 0;
    }

};