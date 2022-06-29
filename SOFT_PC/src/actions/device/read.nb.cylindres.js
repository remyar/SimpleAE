import createAction from '../../middleware/actions';
const { ipcRenderer } = require('electron');


export default createAction(async ( { getState, extra }) => {
    try {
        let data = ipcRenderer.sendSync('WRITE_SERIAL_PORT', "[RNBC]");
        if ( data ){
            return {};
        } else {
            throw new Error ("Fail to open Com Port");
        }
    } catch (err) {
        throw { message: err.message };
    }
});