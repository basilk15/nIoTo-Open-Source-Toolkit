#ifndef LCD_MODULE_H
#define LCD_MODULE_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Adjust the address if your I2C module uses something different
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS    2

// Declare LCD object
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// Call this in setup()
void initLCD() {
  lcd.init();         // Initialize the LCD
  lcd.backlight();    // Turn on backlight
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LCD Ready");
}

void lcdClearLine(int row) {
  if (row >= 0 && row < LCD_ROWS) {
    lcd.setCursor(0, row);
    for (int i = 0; i < LCD_COLUMNS; i++) {
      lcd.print(" ");
    }
    lcd.setCursor(0, row); // Optional: reset cursor to line start
  }
}


// Utility function to print a message at (col, row)
void lcdPrintAt(int col, int row, const char* message) {
  lcdClearLine(row);
  lcd.setCursor(col, row);
  lcd.print(message);
}

// Optional: Clear LCD
void lcdClear() {
  lcd.clear();
}



#endif
