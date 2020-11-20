const { data } = require('jquery');

const ipc = require('electron').ipcRenderer;

let _idxRx = 0;
let _dataRx = [];
let _idxTx = 0;
let _dataTx = [];
let rpmGauge = undefined;
let advanceGauge = undefined;
let dwellGauge = undefined;

let config = {
    simulationMode: false,
    nbCylindres: undefined,
    Na: [],
    Anga: [],
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

function _putByte(val) {
    _dataTx.push(val);
    _idxTx++;
}

function _checkAndInitConfig() {
    if ( config.nbCylindres === undefined && config.Na.length === 0 && config.Anga.length === 0 ){
        config.simulationMode = true;
    }

    if (config.nbCylindres == undefined || config.nbCylindres > 6) {
        config.nbCylindres = 4;
    }

    if (config.Na.length === 0) {
        for (let i = 0; i < 16; i++) {
            config.Na.push(65535);
        }
    }

    if (config.Anga.length === 0) {
        for (let i = 0; i < 16; i++) {
            config.Anga.push(65535);
        }
    }

    config.Na = config.Na.map((val, i) => val === 65535 ? i * 500 : val);
    config.Anga = config.Anga.map((val, i) => val === 65535 ? i * 5 : val);
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
                        case (1): {
                            config.nbCylindres = _getBytes(1);
                            config.Na = [];
                            config.Anga = [];
                            for (let i = 0; i < 16; i++) {
                                config.Na.push(_getBytes(2));
                            }
                            for (let i = 0; i < 16; i++) {
                                config.Anga.push(_getBytes(2));
                            }
                            break;
                        }
                    }

                    _dataRx = [];
                }
            });
        });
    },
    setRpmGauge: (gauge) => {
        rpmGauge = gauge;
        rpmGauge.draw();
    },
    setAdvanceGauge: (gauge) => {
        advanceGauge = gauge;
        advanceGauge.draw();
    },
    setDwellGauge: (gauge) => {
        dwellGauge = gauge;
        dwellGauge.draw();
    },
    readConfig: async () => {
        _idxTx = 0;
        _dataTx = [];

        _putByte(0);
        _putByte(3);
        _putByte(1);

        return new Promise((resolve) => {

            setTimeout(() => {
                ipc.send('tx_data', _dataTx);
                setTimeout(() => {
                    _checkAndInitConfig();
                    resolve(config);
                }, 500);
            }, 1500);

        });
    },
    changeNbCylindres: (value) => {
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