import createAction from '../../middleware/actions';
const { ipcRenderer } = require('electron');


export default createAction(async (port, { getState, extra }) => {
    try {
        let data = ipcRenderer.sendSync('OPEN_SERIAL_PORT', port);
        if ( data ){
            return {};
        } else {
            throw new Error ("Fail to open Com Port");
        }
    } catch (err) {
        throw { message: err.message };
    }
});