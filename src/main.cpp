#include <Arduino.h>
#include <EEPROM.h>

int eeprom_size = 1000;

byte small = true;

int numero = 1;

long longo = 9999;

String username = "Teste";
String password = "Etset";

// 2 Bytes
void EEPROMWriteInt(int address, int value);
int  EEPROMReadInt(int address);

// 4 Bytes
void EEPROMWriteLong(int address, long value);
long EEPROMReadLong(int address);

// N Bytes
void EEPROMWriteStr(int address, String value);
void EEPROMConcatStr(int address, String value);
void EEPROMClear(int size);

String EEPROMReadStr(int address);

void setup() {

    Serial.begin(9600);

    // Posicao 0 (zero) da EEPROM 1 byte
    EEPROM.write(0, small);      

    // Posicao 1 e 2 da EEPROM 2 bytes
    EEPROMWriteInt(1, numero);

    // Posicao 3, 4, 5 e 6 da EEPROM 4 bytes 
    EEPROMWriteLong(3, longo); 

    // Posicao 100, 101, 102 etc... N bytes
    EEPROMWriteStr(100, username);

    // Posicao 450, 451, 452 etc... N bytes
    EEPROMWriteStr(450, password);

}

void loop() {

    delay(2000);

    Serial.print("Username: ");
    Serial.println(EEPROMReadStr(100));

    Serial.print("Password: ");
    Serial.println(EEPROMReadStr(450));
    
    delay(2000);

    EEPROMClear(eeprom_size);

    delay(2000);

    Serial.print("Username: ");
    Serial.println(EEPROMReadStr(100));

    Serial.print("Password: ");
    Serial.println(EEPROMReadStr(450));

}

// Limpeza da EEPROM
void EEPROMClear(int size) {
    for (int nL = 0; nL < size; nL++) { 
        EEPROM.write(nL, 0);
    }
}

void EEPROMWriteInt(int address, int value) {

    byte hiByte = highByte(value);
    byte loByte = lowByte(value);

    EEPROM.write(address, hiByte);
    EEPROM.write(address + 1, loByte);   

}

int EEPROMReadInt(int address) {
   byte hiByte = EEPROM.read(address);
   byte loByte = EEPROM.read(address + 1);
   
   return word(hiByte, loByte); 
}

void EEPROMWriteLong(int address, long value) {

    byte four = (value & 0xFF);
    byte three = ((value >> 8) & 0xFF);
    byte two = ((value >> 16) & 0xFF);
    byte one = ((value >> 24) & 0xFF);

    EEPROM.write(address, four);
    EEPROM.write(address + 1, three);
    EEPROM.write(address + 2, two);
    EEPROM.write(address + 3, one);

}

long EEPROMReadLong(int address) {

    long four = EEPROM.read(address);
    long three = EEPROM.read(address + 1);
    long two = EEPROM.read(address + 2);
    long one = EEPROM.read(address + 3);

    return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);

}

void EEPROMWriteStr(int address, String value) {

    int nL;

    for (nL=0; nL < value.length(); nL++)  {
        EEPROM.write(address + nL, value.charAt(nL) );

        if (nL >= 1000) {
            break;
        }
    }

    if (nL < 1000) {
        EEPROM.write(address + nL, (char)0 );
    }

}

void EEPROMConcatStr(int address, String value) {

    int nL;
    char readByte;
    bool nullFound = false;
    int strPos = 0;

    for (nL = 0; nL < 1000; nL++)  {

        if (!nullFound) {
            readByte = EEPROM.read(address + nL);

            if (readByte == (char)0) {
                nullFound = true;
            }
        } 
        
        if (nullFound) {
            EEPROM.write(address + nL, value.charAt(strPos) );          
            strPos++;

            if ((strPos + 1) > value.length()) {
                EEPROM.write(address + nL + 1, (char)0 );
                break;
            }
        }
    }

}

String EEPROMReadStr(int address) {

    String readStr = "";
    char readByte;
    int readAddress = address;

    do {
        readByte = EEPROM.read(readAddress);
        readStr += readByte;
        readAddress++;
    } while ( (readByte != (char)0) && (readAddress < (address + 1000)) );

    return readStr;

}