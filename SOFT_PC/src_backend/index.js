const { ipcMain } = require('electron');
const { SerialPort } = require('serialport');
const MessageBuffer = require('./MessageBuffer');
let received = new MessageBuffer("]");

let mainWindow = undefined;
let client = undefined;

module.exports = {
    setMainWindows: async (_mainWindow) => {
        mainWindow = _mainWindow;
    },
    start: () => {
        ipcMain.on('OPEN_DEV_TOOLS', (event, value) => {
            if (value) {
                mainWindow.webContents.openDevTools();
            } else {
                mainWindow.webContents.closeDevTools();
            }
        });

        ipcMain.on('GET_ALL_SERIAL_PORT', async (event, value) => {
            let result = await SerialPort.list();
            event.returnValue = result;
        });

        ipcMain.on('OPEN_SERIAL_PORT', async (event, value) => {

            if (client != undefined ){
                if ( client.path != value) {
                    client.close();
                } else {
                    event.returnValue = true;
                    return;
                }
            }

            try {
                client = new SerialPort({ path: value, autoOpen: false, baudRate: 115200, rtscts: false });

                function _open(){
                    return new Promise((resolve,reject)=>{
                        client.open((err)=>{
                            if ( err ){
                                reject(err);
                            } else {
                                resolve();
                            }
                        });
                    });
                }

                await _open();

                client.on('data', (data) => {
                    received.push(data);
                    while (!received.isFinished()) {
                        const message = received.handleData();
                        console.log("SERIAL Read : " , message + "]");
                        mainWindow && mainWindow.webContents.send('ON_DATA_SERIAL_PORT', message + "]");
                    }
                });

                event.returnValue = true;

            } catch (err) {
                event.returnValue = false;
            }

        });

        ipcMain.on('WRITE_SERIAL_PORT', async (event, value) => {
            if (client != undefined) {
                try {
                    await client.write(value);
                    console.log("SERIAL Write : " , value);
                    event.returnValue = true;
                } catch (err) {
                    event.returnValue = false;
                }
            }
        });
    }
}