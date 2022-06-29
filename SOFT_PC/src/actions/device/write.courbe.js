import createAction from '../../middleware/actions';
const { ipcRenderer } = require('electron');


export default createAction(async (courbe, { getState, extra }) => {
    try {
        let str = "[WC"
        courbe.forEach((el) => {
            str += ":";
            str += el;
        });
        str += "]";

        let data = ipcRenderer.sendSync('WRITE_SERIAL_PORT', str);
        if (data) {
            return {};
        } else {
            throw new Error("Fail to open Com Port");
        }
    } catch (err) {
        throw { message: err.message };
    }
});