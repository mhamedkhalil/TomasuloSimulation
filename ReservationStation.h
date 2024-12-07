#include<iostream>
#include<vector>
using namespace std;

int ROB_ENTRIES;

struct ROBEntry 
{
    int id;                  
    string Op;              
    int dest;                 
    int value;                
    bool ready;               
    bool free = 1;
    bool speculative = 0;               
};
vector<ROBEntry> ROB;
int ROB_head = 0, ROB_tail = 0;
void clearROB(ROBEntry* entry)
{
    entry->Op = "";
    entry->free = 1;
    entry->ready = 0;
    entry->speculative = 0;
    entry->value = 0;
}

class reservationStation
{
    public:
    string Op;
    int Vj, Vk;
    ROBEntry* Qj;
    ROBEntry* Qk;
    int A;
    bool Busy;
    bool speculative;
    int remainingTime;

    reservationStation()
    {
        Op = "";
        Vj = -1;
        Vk = -1;
        Qj = nullptr;
        Qk = nullptr;
        A  = -1;
        Busy = 0;
        speculative = 0;
        remainingTime = 0;
    }

    bool execute()
    {
        if(this->Busy && this->Qj == nullptr && this->Qk == nullptr)
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
        this->speculative = 0;
        this->remainingTime = 0;
    }
};