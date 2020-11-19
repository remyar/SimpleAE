const ipc = require('electron').ipcRenderer;

let _idxRx = 0;
let _dataRx = [];
let _idxTx = 0;
let _dataTx = [];
let rpmGauge = undefined;
let advanceGauge = undefined;
let dwellGauge = undefined;

let config = {
    nbCylindres : undefined,
    Na : [],
    Anga : [],
};
function _getBytes(nbBytes) {
    let value = 0;
    for (let i = 0; i < nbBytes; i++) {
        value = value << 8;
        value |= _dataRx[_idxRx];
        _idxRx++;
    }
    return value;
}

function _putByte(val){
    _dataTx.push(val);
    _idxTx++;
}

module.exports = {
    init: () => {
        _idxTx = 0;

        ipc.on('rx_data', (event, data) => {
            data.forEach(element => {
                _dataRx.push(element);
                if ((_dataRx.length > 2) && _dataRx.length >= ((_dataRx[0] * 256) + _dataRx[1])) {

                    //-- process reception
                    _idxRx = 0;
                    let length = _getBytes(2);
                    let cmd = _getBytes(1);
                    switch (cmd) {
                        case (0): {
                            //-- rpm response
                            let rpm = (1 / (_getBytes(4) / 1000000)) * 60;
                            let advance = _getBytes(4);
                            let dwell = _getBytes(4);

                            if (rpmGauge) {
                                rpmGauge.value = rpm;
                            }
                            if (advanceGauge) {
                                advanceGauge.value = advance;
                            }
                            if (dwellGauge) {
                                dwellGauge.value = dwell;
                            }
                            break;
                        }
                        case (1):{
                            config.nbCylindres = _getBytes(1);
                            if ( config.nbCylindres == undefined || config.nbCylindres > 6 ){
                                config.nbCylindres = 4;
                            }

                            config.Na = [];
                            config.Anga = [];
                            for ( let i = 0 ; i < 16 ; i++ ){
                                let _v = _getBytes(2);
                                if ( _v === 65535 ){
                                    _v = ( i*500);
                                }
                                config.Na.push(_v);
                            }
                            for ( let i = 0 ; i < 16 ; i++ ){
                                let _v = _getBytes(2);
                                if ( _v === 65535 ){
                                    _v = ( i*5);
                                }
                                config.Anga.push(_v);
                            }
                            break;
                        }
                    }

                    _dataRx = [];
                }
            });
        });
    },
    setRpmGauge : (gauge ) =>{
        rpmGauge = gauge;
        rpmGauge.draw();
    },
    setAdvanceGauge : (gauge ) =>{
        advanceGauge = gauge;
        advanceGauge.draw();
    },
    setDwellGauge : (gauge ) =>{
        dwellGauge = gauge;
        dwellGauge.draw();
    },
    readConfig : async () => {
        _idxTx = 0;
        _dataTx = [];

        _putByte(0);
        _putByte(3);
        _putByte(1);

        return new Promise((resolve)=>{
            
            setTimeout(()=>{
                ipc.send('tx_data', _dataTx);
                setTimeout(()=>{
                    resolve(config);
                },500);
            },1500);

        });
    },
    changeNbCylindres : ( value )=>{
        _dataTx = [];

        _putByte(0);
        _putByte(4);
        _putByte(2);
        _putByte(parseInt(value));

        ipc.send('tx_data', _dataTx);
    },

    write: (data) => {
        ipc.send('tx_data', data);
    }
}