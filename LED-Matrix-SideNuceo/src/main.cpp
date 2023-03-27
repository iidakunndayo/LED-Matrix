#include <mbed.h>
#include <Ticker.h>

Ticker ticker;

BusOut      line_select(PC_0, PC_1, PC_3);
DigitalOut  line_D(PC_2);

DigitalOut  OE(PA_5);
DigitalOut  CLK(PA_6);
DigitalOut  LAT(PA_7);

DigitalOut  R0(PA_8);
DigitalOut  G0(PB_10);
DigitalOut  B0(PB_4);
DigitalOut  R1(PB_5);
DigitalOut  G1(PB_3);
DigitalOut  B1(PA_10);

void timerISR() {
    static uint8_t line = 0;
    uint8_t x;
    
    for (x = 0; x < 128; x++){
        R0 = 1;
        R1 = 1;
        G0 = 1;
        G1 = 1;
        B0 = 1;
        B1 = 1;

        CLK = 1;
        CLK = 0;
    }

    OE = 1;
    LAT = 1;
    LAT = 0;

    line_select = line & 0b0111;
    line_D = line < 0b1000 ? 1 : 0;
    wait_us(2);
    line_D = line < 0b1000 ? 0 : 1;

    OE = 0;

    line++;
    if (line > 15) {
        line = 0;
    }
}

int main() {
    ticker.attach_us(&timerISR, 100);
    wait_us(2e5);

    while(1) {
        wait_us(2e3);
    }
}