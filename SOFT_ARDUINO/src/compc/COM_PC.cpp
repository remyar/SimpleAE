#include "../Low-Level/board.h"
#include "../Low-Level/Serial.h"
#include "./COM_PC.h"
#include <CircularBuffer.h>

CircularBuffer<uint8_t, 64> bufferTx;

void _putString(String str)
{
    for (int i = 0; i < str.length(); i++)
    {
        bufferTx.push(str[i]);
    }
}

void COMPC_TaskInit(void)
{
    bufferTx.clear();
}

void COMPC_TaskRun(void)
{
    if (SERIAL_Available())
    {
        switch (SERIAL_Getc())
        {
        case ('Q'):
        {
            _putString("simpleae");
            break;
        }
        case ('F'): // send serial protocol version
        {
            _putString("001");
            break;
        }
        }
    }

    if (bufferTx.size() > 0)
    {
        SERIAL_Putc(bufferTx.first());
        bufferTx.shift();
    }
}
