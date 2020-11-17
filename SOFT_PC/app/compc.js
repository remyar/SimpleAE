const ipc = require('electron').ipcRenderer;

let _idxRx = 0;
let _dataRx = [];
let rpmGauge = undefined;
let advanceGauge = undefined;
let dwellGauge = undefined;

function _getBytes(nbBytes) {
    let value = 0;
    for (let i = 0; i < nbBytes; i++) {
        value = value << 8;
        value |= _dataRx[_idxRx];
        _idxRx++;
    }
    return value;
}

module.exports = {
    init: () => {
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
                            let rpm = _getBytes(4);
                            let advance = _getBytes(4);
                            let dwell = _getBytes(4);
                            if (rpmGauge) {
                                rpmGauge.value = rpm;
                                rpmGauge.update();
                            }
                            if (advanceGauge) {
                                advanceGauge.value = advance;
                                advanceGauge.update();
                            }
                            if (dwellGauge) {
                                dwellGauge.value = dwell;
                                dwellGauge.update();
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
    write: (data) => {
        ipc.send('tx_data', data);
    }
}