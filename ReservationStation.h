#include<iostream>
#include<vector>
using namespace std;

class reservationStation
{
    public:
    string Op;
    int Vj, Vk;
    reservationStation* Qj;
    reservationStation* Qk;
    int A;
    bool Busy;

    reservationStation()
    {
        Op = "";
        Vj = -1;
        Vk = -1;
        Qj = NULL;
        Qk = NULL;
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