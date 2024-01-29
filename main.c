#include <stdio.h>
#include <unistd.h>
#include <pigpio.h>
#include <sys/time.h>

#define HIGH 0
#define LOW 1
#define barPin1 17
#define barPin2 18
#define barPin3 27
#define barPin4 22
#define barPin5 23
#define barPin6 24
#define barPin7 25
#define barPin8 2
#define barPin9 3
#define barPin10 8
#define button 12

typedef enum ButtonAction ButtonAction;
enum ButtonAction {
    INCREMENT,
    DECREMENT,
};

unsigned long millis() {
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec * 1000) + (now.tv_usec / 1000);
}

int main() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    int barPins[10] = {
            barPin1,
            barPin2,
            barPin3,
            barPin4,
            barPin5,
            barPin6,
            barPin7,
            barPin8,
            barPin9,
            barPin10
    };

    gpioSetMode(button, PI_INPUT);
    gpioSetPullUpDown(button, PI_PUD_UP);

    enum ButtonAction buttonAction = INCREMENT;
    int barPinState = 0;
    int lastButtonState = HIGH;
    unsigned long lastUpdateTime = millis();
    long captureTime = 50;


    for (int i = 0; i < 10; ++i) {
        gpioSetMode(barPins[i], PI_OUTPUT);
    }

    while (1) {
        int reading = gpioRead(button);

        if (LOW == reading) {
            continue;
        }

        if (INCREMENT == buttonAction) {
            printf("INCREMENT %d \n", barPinState);
            gpioWrite(barPins[barPinState], HIGH);

            if (9 == barPinState) {
                buttonAction = DECREMENT;
            } else {
                barPinState++;
            }

            usleep(200000);
        } else {
            printf("DECREMENT %d \n", barPinState);
            gpioWrite(barPins[barPinState], LOW);

            if (0 == barPinState) {
                buttonAction = INCREMENT;
            } else {
                barPinState--;
            }

            usleep(200000);
        }
    }
}
