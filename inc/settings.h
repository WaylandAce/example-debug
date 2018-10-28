#ifndef settings_h
#define settings_h

const uint16_t LED_GREEN = GPIO_Pin_12;
const uint16_t LED_ORANGE = GPIO_Pin_13;
const uint16_t LED_RED = GPIO_Pin_14;
const uint16_t LED_BLUE = GPIO_Pin_15;

const uint16_t LEDS = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

const uint16_t USER_BUTTON = GPIO_Pin_0;

#endif /* settings_h */
