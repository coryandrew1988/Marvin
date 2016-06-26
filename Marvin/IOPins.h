#ifndef IOPins_h
#define IOPins_h
enum IOPins : int {
    PIN_FRONT = 4,
    PIN_RIGHT = 2,
    PIN_LEFT = 8,
    PIN_BACK = 6,
    PIN_FRONT_RIGHT = 3,
    PIN_FRONT_LEFT = 7,
    PIN_SERVO_RIGHT = 11,
    PIN_SERVO_LEFT = 12,
    PIN_BUZZER = 5,
};
typedef enum IOPins IOPins;
#endif

