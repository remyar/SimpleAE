#include "./COM_PC.h"
#include "../aepl.h"

String _sCmdTx = "";
String _sCmd = "";

static unsigned long _ms = millis();

void _putString(String val)
{
    _sCmdTx += val;
}

void COMPC_TaskInit(void)
{
    _sCmdTx = "";
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

        if (_sCmd.startsWith("ID"))
        {
            //-- demande ID
            _putString("[ID:aeduino]");
        }
        else if (_sCmd.startsWith("RC"))
        {
            //-- lecture courbe avance
            _putString("[RC");
            for (int i = 0; i < 14; i++)
            {
                _putString(":");
                _putString(String(Anga[i]));
            }
            _putString("]");
        }
        else if (_sCmd.startsWith("WC"))
        {
            _sCmd.remove(0, 3);

            //-- ecriture courbe d'avance
            for (int i = 0; i < 13; i++)
            {
                Anga[i] = _sCmd.toInt();
                while (_sCmd.startsWith(":") == false)
                {
                    _sCmd.remove(0, 1);
                }
                //-- remove ':'
                _sCmd.remove(0, 1);
            }

            Anga[13] = _sCmd.toInt();

            for ( int i = 0 ; i < 14 ; i++){
                MEMORY_WriteUnsignedInt(ADDR_ADV + (i * 2) , Anga[i]);
            }

            Init();

            //-- lecture courbe avance
            _putString("[RC");
            for (int i = 0; i < 14; i++)
            {
                _putString(":");
                _putString(String(Anga[i]));
            }
            _putString("]");
        }
        else if (_sCmd.startsWith("RNBC"))
        {
            //-- lecture courbe avance
            _putString("[RNBC");
            _putString(":");
            _putString(String(Ncyl));
            _putString("]");
        } 
        else if ( _sCmd.startsWith("WNBC")){
            //-- ecriture nombre de cylindres
            _sCmd.remove(0, 5);

            Ncyl = _sCmd.toInt();

            MEMORY_WriteUnsignedInt(ADDR_NB_CYLINDRES,Ncyl);

            Init();

            _putString("[RNBC");
            _putString(":");
            _putString(String(Ncyl));
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
    if (Serial.available())
    {
        _sCmd += (char)Serial.read();
    }

    if (_sCmd.endsWith("]"))
    {
        _sCmdTx = "";
        _processCommand();
    }

    if (_sCmdTx.length() > 0)
    {
        Serial.print(_sCmdTx);
        _sCmdTx = "";
    }
}
