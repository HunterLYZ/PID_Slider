#include "Arduino.h"

#ifndef PID_SERIAL
#define PID_SERIAL Serial
#endif

#ifndef PID_H
#define PID_H

class PIDobject
{
public:
    PIDobject(const float kP, const float kI, const float kD,
              const uint8_t tP, const uint8_t tI, const uint8_t tD)
    {
        this->kP = kP;
        this->kI = kI;
        this->kD = kD;
        this->tP = tP;
        this->tI = tI;
        this->tD = tD;
    }
    void clac(int target, int input, int *output)
    {
        update(target, input, *output);
        if (lazy)
        {
            *output = 0;
        }
        else
        {
            Pclac();
            Iclac();
            Dclac();
            integralPartition();
            antiwindup();
            *output = P + I + D + 0.5;
        }
    }
    int clac(int target, int input)
    {
        int output;
        update(target, input, output);
        if (lazy)
        {
            output = 0;
        }
        else
        {
            Pclac();
            Iclac();
            Dclac();
            integralPartition();
            antiwindup();
            output = P + I + D + 0.5;
        }
        return output;
    }


    //advance settings
    void setTolerance(int a, int b, int c, int d)
    {
        tolerance[0] = a;
        tolerance[1] = b;
        tolerance[2] = c;
        tolerance[3] = d;
    }
    void setLaziness(int laziness)
    {
        this->laziness = laziness;
    }

protected:
private:
    int input[16], output[16], target[16], err[16];
    float kP, kI, kD;
    // P、I、D项计算时，考虑的数据量————例如：tD等于10的时候，D项等于最近10次循环的err之和
    uint8_t tP, tI, tD;
    // P、I、D项的值
    float P, I, D;
    //当err在tolerance[0]~tolerance[3]之间时，output值会更加靠近0
    //当err在tolerance[1]~tolerance[2]之间时，output值有几率等于0
    //当laziness值越大，output值等于0的几率越大
    int8_t tolerance[4] = {-4, -1, 1, 4};
    uint8_t laziness = 4;
    void update(int target, int input, int output)
    {

        //待优化ringbuf
        uint8_t i = 16;
        while (i--)
        {
            this->err[i + 1] = this->err[i];
            this->target[i + 1] = this->target[i];
            this->input[i + 1] = this->input[i];
            this->output[i + 1] = this->output[i];
        }
        this->err[0] = target - input;
        this->target[0] = target;
        this->input[0] = input;
        this->output[0] = output;
    }
    bool lazy()
    {
        uint8_t i = tI;
        uint8_t lazy = 0;
        while (i--)
        {
            if (err[i] > tolerance[1] && err[i] < tolerance[2])
            {
                lazy++;
            }
        }
        if (lazy > tI / laziness)
            return true;
        return false;
    }
    void Pclac()
    {
        uint8_t i = tP;
        P = 0;
        while (i--)
        {
            P += err[i];
        }
        P *= kP;
        P /= tP;
    }
    void Iclac()
    {
        uint8_t i = tI;
        I = 0;
        while (i--)
        {
            I += err[i];
        }
        I *= kI;
        I /= tI;
    }
    void Dclac()
    {
        uint8_t i = tD / 2;
        D = 0;
        while (i--)
        {
            D += err[i] - err[tD - 1 - i];
        }
        D *= kD;
        D /= (tD + 1) / 2;
    }
    void integralPartition()
    {
        uint8_t i = tI;
        while (i--)
        {
            if (err[i] > tolerance[0] && err[i] < tolerance[4])
            {
                if (P * I > 0)
                {
                    P = 0;
                    return;
                }
                else
                {
                    I = 0;
                    return;
                }
            }
        }
    }
    void antiwindup()
    {
        uint8_t i = tI;
        uint8_t n = 0;
        while (i--)
        {
            if (err[i + 1] * err[i] < 0)
                n++;
        }
        if (n = 1)
        {
            I = 0;
        }
    }
};
#endif