#include "./memory.h"

template <class T> int EEPROM_writeAnything(int ee, const T &value)
{
    const uint8_t *p = (const uint8_t *)(const void *)&value;
    uint8_t i;
    for (i = 0; i < sizeof(value); i++)
        EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T &value)
{
    uint8_t *p = (uint8_t *)(void *)&value;
    uint8_t i;
    for (i = 0; i < sizeof(value); i++)
        *p++ = EEPROM.read(ee++);
    return i;
}

void MEMORY_Init(void)
{
}

uint8_t MEMORY_ReadByte(uint16_t address)
{
    uint8_t val;
    EEPROM_readAnything(address, val);
    return val;
}

void MEMORY_WriteByte(uint16_t address, uint8_t val)
{
    EEPROM_writeAnything(address, val);
}

uint16_t MEMORY_ReadUnsignedInt(uint16_t address)
{
    uint16_t val = 0;
    EEPROM_readAnything(address, val);
    return val;
}

void MEMORY_WriteUnsignedInt(uint16_t address, uint16_t val)
{
    EEPROM_writeAnything(address, val);
}

float MEMORY_ReadFloat(uint16_t address){
    float val;
    EEPROM_readAnything(address, val);
    return val;
}

void MEMORY_WriteFloat(uint16_t address, float val){
    EEPROM_writeAnything(address, val);
}