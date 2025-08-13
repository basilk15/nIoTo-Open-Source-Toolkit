#ifndef LCD_MODULE_H
#define LCD_MODULE_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS    2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void initLCD() {
  lcd.init();        
  lcd.backlight();    
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
    lcd.setCursor(0, row); 
  }
}

void lcdPrintAt(int col, int row, const char* message) {
  lcdClearLine(row);
  lcd.setCursor(col, row);
  lcd.print(message);
}


void lcdClear() {
  lcd.clear();
}



#endif
