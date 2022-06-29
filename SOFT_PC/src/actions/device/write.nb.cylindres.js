import createAction from '../../middleware/actions';
const { ipcRenderer } = require('electron');


export default createAction(async (value, { getState, extra }) => {
    try {
        let data = ipcRenderer.sendSync('WRITE_SERIAL_PORT', "[WNBC:" + value + "]");
        if (data) {
            return {};
        } else {
            throw new Error("Fail to open Com Port");
        }
    } catch (err) {
        throw { message: err.message };
    }
});