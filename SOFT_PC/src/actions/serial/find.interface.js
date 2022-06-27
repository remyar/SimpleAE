import createAction from '../../middleware/actions';
const { ipcRenderer } = require('electron');

export async function findInterface({ extra, getState }) {

    try {
        let state = getState().system;

        let data = ipcRenderer.sendSync('GET_ALL_SERIAL_PORT');
        let _d = data.filter((d) => {
            if (d.vendorId == '1A86' && d.productId == '7523') {
                return d;
            }
        });

        state.ports = [..._d];

        return {
            system: { ...state }
        };

    } catch (err) {
        throw { message: err.message };
    }
}

export default createAction(findInterface);