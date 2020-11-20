const electron = require('electron');
const Serial = require('./serial');
// Module to control application life.
const app = electron.app

// Module to create native browser window.
const BrowserWindow = electron.BrowserWindow;

//require('electron-reload')(__dirname);

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
let mainWindow

async function  createWindow() {

    let serial = await Serial.findInterface();
    if ( serial ){
        await Serial.open(serial.path);
    }
    // Create the browser window.
    mainWindow = new BrowserWindow({
        width: 1024, height: 768, webPreferences: {
            nodeIntegration: true,
            devTools: true,
        },
    });

    if ( serial ){
        Serial.process(mainWindow);
        mainWindow.setTitle("Online Mode");
    } else {
        mainWindow.setTitle("Demonstration Mode");
    }

   
    mainWindow.setMenu(null);

    //mainWindow.maximize();

    // and load the index.html of the app.
    mainWindow.loadURL(`file://${__dirname}/app/index.html`)

    // Open the DevTools.
    mainWindow.webContents.openDevTools();

    // Emitted when the window is closed.
    mainWindow.on('closed', function () {
        // Dereference the window object, usually you would store windows
        // in an array if your app supports multi windows, this is the time
        // when you should delete the corresponding element.
        mainWindow = null
    })

}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on('ready', createWindow)

// Quit when all windows are closed.
app.on('window-all-closed', function () {
    // On OS X it is common for applications and their menu bar
    // to stay active until the user quits explicitly with Cmd + Q
    if (process.platform !== 'darwin') {
        app.quit()
    }
})

app.on('activate', function () {
    // On OS X it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (mainWindow === null) {
        createWindow()
    }
})