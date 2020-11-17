const SerialPort = require('serialport');
const { ipcMain } = require('electron');

let serialport;
let mainWindow;

module.exports = {
    findInterface: async () => {
        let serials = await SerialPort.list();
        return serials.find((s) => s.vendorId === '1A86' && s.productId === '7523');
    },
    open: async (path) => {
        serialport = new SerialPort(path, {
            baudRate: 115200
        });
    },
    process: (_mainWindow) => {
        mainWindow = _mainWindow;

        ipcMain.on('tx_data', (event, arg) => {
            serialport.write(arg);
        });

        serialport.on('data', (data) => {
            mainWindow.webContents.send('rx_data', data);
        });

    }
}