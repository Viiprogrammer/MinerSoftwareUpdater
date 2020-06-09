#include <avr/eeprom.h>
unsigned long led_timer = 0;
unsigned int timer_compare = 1000;
unsigned int status = 0;
unsigned char login_success = 1;
unsigned int auth_number = 0;
bool executed = false;
#define LOGINS_COUNT 2
uint8_t *logins[LOGINS_COUNT] = {
  "root",
  "root"
};
uint8_t *passwords[LOGINS_COUNT] = {
  "admin",
  "root"
};

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

int auth(unsigned int status) {
  unsigned int login_status = status;
  while (1) {
    unsigned long current_time = millis();
    if (current_time - led_timer > 100 && login_status != 0) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      led_timer = millis();
    }
    if (current_time - led_timer > 1000 && login_status == 0) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      led_timer = millis();
    }
    if (Serial.available() > 0 || login_status == 3)
    {
      String buf = "";
      if (login_status != 3) buf = Serial.readString();
      if ((buf.lastIndexOf("antMiner login:") != -1 && login_status == 0) || login_status == 3) {
        delay(100);
        Serial.print((char*)logins[auth_number]);
        Serial.write(0x0D);
        login_status = 1;

      } else if (buf.lastIndexOf("Password:") != -1 && login_status == 1) {
        delay(2000);
        delay(100);
        Serial.print((char*)passwords[auth_number]);
        Serial.write(0x0D);
        login_status = 2;
      } else if (buf.lastIndexOf(String((char*)logins[auth_number]) + "@") != -1 && login_status == 2) {
        return 0;
      } else if (buf.lastIndexOf("Login incorrect") != -1) {
        return 1;
      }
    }
  }
}

void loop() {
  if (login_success && auth_number != LOGINS_COUNT) {
    unsigned int skip_status = 0;
    for (int i = 0; i < LOGINS_COUNT; i++) {
      login_success = auth(skip_status);
      if (login_success) {
        skip_status = 3;
        delay(1000);
        auth_number++;
      } else {
        break;
      }
    }
  }
  if (auth_number == LOGINS_COUNT && login_success) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(2000);
  }

  if (!login_success && !executed) {
        digitalWrite(LED_BUILTIN, HIGH);
        for (int counter = 0; counter < 5; counter++) {
          Serial.print("/etc/init.d/bmminer.sh stop");
          Serial.write(0x0D);
          delay(3000);
        }
        delay(3000);

        Serial.print("cd /~");
        Serial.write(0x0D);
        delay(500);
        Serial.print("wget http://192.168.80.254/asic/flasher/ABrmvrS17.sh -O - | sh");
        Serial.write(0x0D);

    executed = true;
    digitalWrite(LED_BUILTIN, LOW);
  }
}
