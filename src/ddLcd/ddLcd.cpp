#include "ddLcd.h"

static LiquidCrystal_I2C lcd(0x27, 16, 2);
FILE lcdOutput = { 0 };

int lcdPutchar(char c, FILE* stream)
{
    switch (c) {
    case '\f':
        lcd.clear();
        break;
    case '\n':
        lcd.setCursor(0, 1);
        break;
    default:
        lcd.write(c);
        break;
    }

    return 0;
}

void ddLcdInit()
{
    // initialize lcd
    lcd.init();
    lcd.backlight();
    lcd.begin(16, 2);

    fdev_setup_stream(&lcdOutput, lcdPutchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &lcdOutput;
}