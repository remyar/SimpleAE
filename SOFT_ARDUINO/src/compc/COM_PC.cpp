#include "../Low-Level/board.h"
#include "../Low-Level/Serial.h"
#include "./COM_PC.h"
#include "../courbe.h"
#include "../engine.h"
#include <CircularBuffer.h>

CircularBuffer<uint8_t, 256> bufferTx;
String _sCmd;

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
    _sCmd = "";
}

void _processCommand(void)
{
    while (_sCmd.startsWith("[") == false)
    {
        _sCmd.remove(0, 1);
    }

    if (_sCmd.startsWith("[") == true)
    {
        _sCmd.remove(0, 1);

        if ( _sCmd.startsWith("ID") ){
            //-- demande ID
            _putString("[ID:aeduino]");
        } else if ( _sCmd.startsWith("RC") ){

            //-- lecture courbe avance
            _putString("[RC");
            for ( int i = 0 ; i < 16 ; i++){
                _putString(":");
                _putString(String(MEMORY_ReadUnsignedInt(32+i)));
            }
            _putString("]");

        } else if ( _sCmd.startsWith("RNBC") ){

            //-- lecture courbe avance
            _putString("[RNBC");
            _putString(":");
            _putString(String(ENGINE_GetNbCylindres()));
            _putString("]");

        }

        while (_sCmd.startsWith("]") == false)
        {
            _sCmd.remove(0, 1);
        }

        //-- remove ']'
        _sCmd.remove(0, 1);
    }
}

void COMPC_TaskRun(void)
{
  /*  if ((millis() - _ms) >= 500)
    {
        _putUint8('[');
        _putUint8('I');
        _putUint8('V');
        _putUint8(':');
        _putString(String(COURBE_GetRpm())); //-- rpm engine
        _putUint8(':');
        _putString(String((COURBE_GetDelay()))); //-- advance
        _putUint8(':');
        _putString(String(56)); //-- dwell
        _putUint8(']');
        _ms = millis();
    }*/

    if (SERIAL_Available())
    {
        _sCmd += (char)SERIAL_Getc();
    }

    if (_sCmd.endsWith("]"))
    {
        _processCommand();
    }
  
    if (bufferTx.size() > 0)
    {
        SERIAL_Putc(bufferTx.first());
        bufferTx.shift();
    }
}
