/*

    Copyright (C) 2024 Mauricio Bustos (m@bustos.org)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "poofer.h"
#include <Arduino.h>
#include <Wire.h>

void processAuxCommands();
void processRcCommands();

poofer poof = poofer();

#define POOFER_DEBOUNCE (20)
#define POOFER_COMMAND_START_WINDOW (500)
#define POOFER_COMMAND_WINDOW (4000)

int aux1pin = 2;
int aux2pin = 3;
int aux3pin = 4;
int aux4pin = 5;

long pooferCommandStart = 0;
long pooferCommandDetect = 0;
long pooferStartControl = 0;

void setup() {
  Serial.begin(9600);
  pinMode(aux1pin, INPUT);
  pinMode(aux2pin, INPUT);
  pinMode(aux3pin, INPUT);
  pinMode(aux4pin, INPUT);
}

void loop() {
  if (!poof.patternRunning()) {
    processRcCommands();
  } else {
    poof.iteratePattern();
  }
}

void processCommand(int pin, int pattern) {
  if (digitalRead(pin) == HIGH) {
    if (pooferStartControl > 0) {
      if (millis() - pooferStartControl > POOFER_DEBOUNCE) {
        poof.startPattern(pattern);
        pooferStartControl = 0;
        pooferCommandStart = 0;
        Serial.println(String("Command ") + String(pattern));
      }
    } else {
      pooferStartControl = millis();
    }
  }
}

void processRcCommands() {
    processCommand(aux1pin, 0);
    processCommand(aux2pin, 1);
    processCommand(aux3pin, 2);
    processCommand(aux4pin, 3);
}