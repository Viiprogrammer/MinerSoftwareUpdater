#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "OneButton.h"

#define SERIES_MENU 0
#define MODEL_MENU 1
#define SUBMODEL_MENU 3
#define SCRIPT_MENU 4

//Series 
#define SERIES9 9
#define SERIES17 17
#define SUBSERIES17_T 0
#define SUBSERIES17_S 1


//Series 17
#define S17_MODEL 0
#define S17PRO_MODEL 1
#define S17E_MODEL 2
#define S17_PLUS_MODEL 3

#define T17_MODEL 4
#define T17_PLUS_MODEL 5
#define T17EMODEL 6


//Series 9
#define S9_MODEL 0
#define S9I_MODEL 1
#define S9J_MODEL 2

//Scripts
//S9
#define S9stock 0
#define S9msk 1
#define S9vnish 2

#define S9istock 3
#define S9imsk 4
#define S9ivnish 5

#define S9jstock 6
#define S9jmsk 7
#define S9jvnish 8

//T17
#define T17stock 9
#define T17msk 10
#define T17vnish 11

//T17+
#define T17PLUSstock 12
#define T17PLUSmsk 13
#define T17vnish_ 14

//T17E
#define T17estock 15
#define T17emsk 16
#define T17evnish 17

//S17
#define S17stock 18
#define S17msk 19
#define S17vnish 20

//S17PRO
#define S17promsk 21
#define S17prostock 22
#define S17provnish 23


OneButton btnUP = OneButton(
  2,  // Input pin for the button
  true,        // Button is active LOW
  true         // Enable internal pull-up resistor
);


OneButton btnDOWN = OneButton(
  3,  // Input pin for the button
  true,        // Button is active LOW
  true         // Enable internal pull-up resistor
);

OneButton btnLEFT = OneButton(
  4,  // Input pin for the button
  true,        // Button is active LOW
  true         // Enable internal pull-up resistor
);

OneButton btnRIGHT = OneButton(
  5,  // Input pin for the button
  true,        // Button is active LOW
  true         // Enable internal pull-up resistor
);

OneButton btnSELECT = OneButton(
  7,  // Input pin for the button
  true,        // Button is active LOW
  true         // Enable internal pull-up resistor
);

String name = "";
//Series
uint8_t series = 9;
bool series_selected = false;

//Model
uint8_t model_id = 0;
bool model_selected = false;

//SubModel 17ser
uint8_t submodel_id = 0;
bool submodel_selected = false;

uint8_t script = 0;
bool script_selected = false;

uint8_t menu_step = 0;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte arrow[] = {
  B10000,
  B11000,
  B11100,
  B11110,
  B11110,
  B11100,
  B11000,
  B10000
};

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("Starting...");
  
  // initialize the LCD
  lcd.begin();
  lcd.createChar(0, arrow);
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Software updater");
  lcd.setCursor(6, 1);
  lcd.print("V1.0");
  
  btnRIGHT.attachClick([]() {
    Serial.println("Right...");
    if(menu_step == 0  && !series_selected){
        series = SERIES17;
    } else if(menu_step == MODEL_MENU && !model_selected){
        if((model_id < S9J_MODEL) && (series == SERIES9)){
          model_id++;
        } else if((series == SERIES17) && (submodel_id == SUBSERIES17_T) && (model_id < T17EMODEL)){
          model_id++;
        } else if((series == SERIES17) && (submodel_id == SUBSERIES17_S) && (model_id < S17_PLUS_MODEL)){
          model_id++;
        }
    } else if(menu_step == SUBMODEL_MENU && !submodel_selected){
        if((submodel_id < SUBSERIES17_S) && (series == SERIES17)){
          submodel_id++;
        }
    }
  });

  btnLEFT.attachClick([]() {
    Serial.println("LEFT...");
    if(menu_step == SERIES_MENU && !series_selected){
        series = SERIES9;
    } else if(menu_step == MODEL_MENU && !model_selected){
        if((model_id > S9_MODEL) && (series == SERIES9)) {
          model_id--;
        } else if((series == SERIES17) && (submodel_id == SUBSERIES17_T) && (model_id > T17_MODEL)){
          model_id--;
        } else if((series == SERIES17) && (submodel_id == SUBSERIES17_S) && (model_id > S17_MODEL)){
          model_id--;
        }
    } else if(menu_step == SUBMODEL_MENU && !submodel_selected){
        if((submodel_id > SUBSERIES17_T) && (series == SERIES17)){
          submodel_id--;
        }
    }
  });
  
  btnSELECT.attachClick([]() {
    Serial.println("Select...");
    if(menu_step == SERIES_MENU && !series_selected){
        if(series == SERIES9){
          menu_step = MODEL_MENU;
        } else if(series == SERIES17){
          menu_step = SUBMODEL_MENU;
        }
        series_selected = true;
        lcd.clear();
    } else if(menu_step == MODEL_MENU && !model_selected){
        menu_step = SCRIPT_MENU;
        model_selected = true;
        lcd.clear();
    } else if(menu_step == SUBMODEL_MENU && !submodel_selected){
        if(submodel_id == SUBSERIES17_S){
          model_id = 0;
        }
        if(submodel_id == SUBSERIES17_T){
          model_id = 4;
        }
        menu_step = MODEL_MENU;
        submodel_selected = true;
        lcd.clear();
    }
  });
  
  delay(3000);
  lcd.clear();
}


void loop()
{
  btnUP.tick();
  btnDOWN.tick();
  btnRIGHT.tick();
  btnLEFT.tick();
  btnSELECT.tick();
  
  if(menu_step == SERIES_MENU){
    lcd.setCursor(5, 0);
    lcd.print("Series");
    lcd.setCursor(3, 1);
    
    if(series == SERIES9){
      lcd.write(0);
    } else lcd.print(" ");
    
    lcd.setCursor(4, 1);
    lcd.print("9");
    lcd.setCursor(9, 1);
    
    if(series == SERIES17){
      lcd.write(0);
    } else lcd.print(" ");
    
    lcd.setCursor(10, 1);
    lcd.print("17");
  } else if(menu_step == MODEL_MENU){
     lcd.setCursor(6, 0);
     lcd.print("Model");
     if(series == SERIES9){
       if(model_id == S9_MODEL){
           lcd.setCursor(7, 1);
           lcd.print("S9 ");
       } else if(model_id == S9I_MODEL){
           lcd.setCursor(7, 1);
           lcd.print("S9i");
       } else if(model_id == S9J_MODEL){
           lcd.setCursor(7, 1);
           lcd.print("S9j");
       }
     } else if(series == SERIES17){
       if(model_id == S17_MODEL){
           lcd.setCursor(7, 1);
           lcd.print("S17   ");
       } else if(model_id == S17PRO_MODEL){
           lcd.setCursor(7, 1);
           lcd.print("S17PRO");
       } else if(model_id == S17E_MODEL){
           lcd.setCursor(7, 1);
           lcd.print("S17e  ");
       } else if(model_id == S17_PLUS_MODEL){
           lcd.setCursor(7, 1);
           lcd.print("S17+  ");
       } else if(model_id == T17_MODEL){
           lcd.setCursor(7, 1);
           lcd.print("T17 ");
       } else if(model_id == T17_PLUS_MODEL){
           lcd.setCursor(7, 1);
           lcd.print("T17+");
       }
        else if(model_id == T17EMODEL){
           lcd.setCursor(7, 1);
           lcd.print("T17e");
       }
       
     }
  } else if(menu_step == SUBMODEL_MENU){
     lcd.setCursor(4, 0);
     lcd.print("Sub Model");
     if(series == SERIES17){
       if(submodel_id == SUBSERIES17_T){
           lcd.setCursor(7, 1);
           lcd.print("T");
       } else if(submodel_id == SUBSERIES17_S){
           lcd.setCursor(7, 1);
           lcd.print("S");
       }
     }
  } else if(menu_step == SCRIPT_MENU){
     script = 1;
     switch(script){
       case S9stock:
          name = "S9stock";      
         break;

       case S9msk:
          name = "S9msk";      
               lcd.setCursor(0, 0);
     lcd.print(name);
    lcd.setCursor(3, 1);
    
    if(series == SERIES9){
      lcd.write(0);
    } else lcd.print(" ");
    
    lcd.setCursor(4, 1);
    lcd.print("YES");
    lcd.setCursor(9, 1);
    
    if(series == SERIES17){
      lcd.write(0);
    } else lcd.print(" ");
    
    lcd.setCursor(10, 1);
    lcd.print("NO");
         break;

       case S9vnish:
          name = "S9vnish";      
         break;

       case S9istock:
          name = "S9istock";      
         break;

       case S9imsk:
          name = "S9imsk";      
         break;

       case S9ivnish:
          name = "S9ivnish";      
         break;

       case S9jstock:
          name = "S9jstock";      
         break;

       case S9jmsk:
          name = "S9jmsk";      
         break;

       case S9jvnish:
          name = "S9jvnish";      
         break;

       case T17msk:
          name = "T17msk";      
         break;

       case T17vnish:
          name = "T17vnish";      
         break;

       case T17PLUSstock:
          name = "T17+stock";      
         break;

       case T17PLUSmsk:
          name = "T17+msk";      
         break;

       case T17vnish_:
          name = "T17vnish";      
         break;

       case T17estock:
          name = "T17estock";      
         break;

       case T17emsk:
          name = "T17emsk";      
         break;

       case T17evnish:
          name = "T17evnish";      
         break;

       case S17stock:
          name = "S17stock";      
         break;

       case S17msk:
         name = "S17msk";      
         break;

       case S17vnish:
         name = "S17vnish";      
         break;

       case S17promsk:
         name = "S17promsk";      
         break;

       case S17prostock:
         name = "S17prostock";      
         break;

       case S17provnish:
         name = "S17provnish";      
         break;         
     }

  }
}
