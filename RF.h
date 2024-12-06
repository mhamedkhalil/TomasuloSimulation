#include"ReservationStation.h"

class RF
{
    private:
    struct reg
    {
        reservationStation* Qi = nullptr;
        int value = 0;
        bool busy = false;
    };

    public:
    vector<reg> registers;
    int regNumber = 16;

    RF()
    {
        registers.resize(regNumber);
    }
};