#include <mbed.h>
#include <Ticker.h>

Ticker ticker;

BusOut      line_select(PC_0, PC_1, PC_2, PC_3);

DigitalOut  OE(PA_5);
DigitalOut  CLK(PA_6);
DigitalOut  LAT(PA_7);

PwmOut      R0(PA_8);
PwmOut      G0(PB_10);
PwmOut      B0(PB_4);
PwmOut      R1(PB_5);
PwmOut      G1(PB_3);
PwmOut      B1(PA_10);

uint8_t buffer_RED[32][128];
uint8_t buffer_GREEN[32][128];
uint8_t buffer_BLUE[32][128];

void timerInterrupt() {
    static uint8_t line = 0;
    static uint8_t PWM = 0;
    uint8_t x;
    
    for (x = 0; x > 128; x++){
        R0 = buffer_RED[line][x]      > PWM;
        R1 = buffer_RED[line+16][x]   > PWM;
        G0 = buffer_GREEN[line][x]    > PWM;
        G1 = buffer_GREEN[line+16][x] > PWM;
        B0 = buffer_BLUE[line][x]     > PWM;
        B1 = buffer_BLUE[line+16][x]  > PWM;

        CLK = 1;
        CLK = 0;
    }

    OE = 1;
    LAT = 1;
    LAT = 0;

    line_select = line;
    OE = 0;

    if (++line > 15) {
        line = 0;
        PWM++;
        if(PWM >= 10) PWM = 0;
    }
}

int main() {
    // put your setup code here, to run once:
    uint8_t x, y;

    ticker.attach_us(&timerInterrupt, 100);
    wait(0.2);

    while(1) {
        // put your main code here, to run repeatedly:
        x = 32.00 * rand() / RAND_MAX;
        y = 128.0 * rand() / RAND_MAX;

        buffer_RED[y][x] = 10.00 * rand() / RAND_MAX;
        buffer_GREEN[y][x] = 10.00 * rand() / RAND_MAX;
        buffer_BLUE[y][x] = 10.00 * rand() / RAND_MAX;

        wait(0.002);
    }
}