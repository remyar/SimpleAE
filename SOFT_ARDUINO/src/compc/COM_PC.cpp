#include "../Low-Level/board.h"
#include "../Low-Level/Serial.h"
#include "./COM_PC.h"
#include "../courbe.h"
#include "../engine.h"
#include <CircularBuffer.h>

CircularBuffer<uint8_t, 256> bufferTx;
CircularBuffer<uint8_t, 64> bufferRx;
static unsigned long _ms = millis();
static uint16_t waitLength = 0;

template <class T>
int _writeAnything(const T &value)
{
    const uint8_t *p = (const uint8_t *)(const void *)&value;
    p += sizeof(value) - 1;
    uint8_t i;
    for (i = 0; i < sizeof(value); i++)
        bufferTx.push(*p--);
    return i;
}

void _putString(String val)
{
    for (uint16_t i = 0; i < val.length(); i++)
    {
        _writeAnything((uint8_t)val[i]);
    }
}

void _putUint8(uint8_t val)
{
    _writeAnything(val);
}

void _putUint16(uint16_t val)
{
    _writeAnything(val);
}

void _putUint32(uint32_t val)
{
    _writeAnything(val);
}

void COMPC_TaskInit(void)
{
    bufferTx.clear();
    bufferRx.clear();
}

void COMPC_TaskRun(void)
{
    if ((millis() - _ms) >= 50)
    {
        _putUint16(15);
        _putUint8(0);
        _putUint32((uint32_t)COURBE_GetRpm());   //-- rpm engine
        _putUint32((uint32_t)COURBE_GetDelay()); //-- advance
        _putUint32(56);                          //-- dwell
        _ms = millis();
    }

    if (SERIAL_Available())
    {
        bufferRx.push(SERIAL_Getc());
        if ((bufferRx.size() >= 2) && (waitLength == 0))
        {
            waitLength = bufferRx.first() * 256;
            bufferRx.shift();
            waitLength += bufferRx.first();
            bufferRx.shift();
        }
        else if (bufferRx.size() >= (waitLength - 2))
        {
            uint8_t cmd = bufferRx.first();
            bufferRx.shift();
            switch (cmd)
            {
            case 1:
            {
                _putUint16(64 + 4);
                _putUint8(1);
                _putUint8(ENGINE_GetNbCylindres());
                for ( int i = 0 ; i < 32 ; i++ ){
                    _putUint8(MEMORY_ReadByte(i));
                }
                for ( int i = 0 ; i < 32 ; i++ ){
                    _putUint8(MEMORY_ReadByte(32+i));
                }
                break;
            }
            case (2):{
                ENGINE_SetNbCylindres(bufferRx.first());
                bufferRx.shift();
                break;
            }
            }
            waitLength = 0;
        }
    }

    if (bufferTx.size() > 0)
    {
        SERIAL_Putc(bufferTx.first());
        bufferTx.shift();
    }
}
