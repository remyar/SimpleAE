#include "../Low-Level/board.h"
#include "../Low-Level/Serial.h"
#include "./COM_PC.h"
#include <CircularBuffer.h>

CircularBuffer<uint8_t, 64> bufferTx;
static unsigned long _ms = millis();

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
}

void COMPC_TaskRun(void)
{
    if ((millis() - _ms) >= 50)
    {
        _putUint16(15);
        _putUint8(0);
        _putUint32(0);  //-- rpm engine
        _putUint32(8);  //-- advance
        _putUint32(56); //-- dwell
        _ms = millis();
    }

    if (SERIAL_Available())
    {
    }

    if (bufferTx.size() > 0)
    {
        SERIAL_Putc(bufferTx.first());
        bufferTx.shift();
    }
}
