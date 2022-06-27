import React, { useState, useEffect } from 'react';
import ReactElectronContext from './context';
import withElectron from './withElectron';
import { withStoreProvider } from '../StoreProvider';
import { withSnackBar } from '../snackBar';
import electron from 'electron';
import { injectIntl } from 'react-intl';
import actions from '../../actions';

function ElectronProvider(props) {

    const intl = props.intl;
    let devices = props.globalState.devices;
    const settings = props.globalState.settings;

    useEffect(async () => {
        
        electron.ipcRenderer.on('update-available', (event, message) => {
            //console.log(message)
            props.snackbar.warning(intl.formatMessage({ id: 'update.available' }));
        });

        electron.ipcRenderer.on('download-progress', (event, message) => {
            //console.log(message)
            props.snackbar.info(intl.formatMessage({ id: 'update.download' }) + ' : ' + parseInt(message?.percent || "0.0") + "%");
        });

        electron.ipcRenderer.on('update-downloaded', (event, message) => {
            //console.log(message)
            props.snackbar.info(intl.formatMessage({ id: 'update.downloaded' }));

        });

        electron.ipcRenderer.on('update-quitForApply', (event, message) => {
            //console.log(message)
            props.snackbar.success(intl.formatMessage({ id: 'update.apply' }));
        });

        electron.ipcRenderer.on('update-error', (event, message) => {
            //console.log(message)
            props.snackbar.error(intl.formatMessage({ id: 'update.error' }));

        });

        electron.ipcRenderer.on('ON_DATA_SERIAL_PORT', (event, message) => {
            props.dispatch(props.Parser(message));
        });

    }, []);

    return <ReactElectronContext.Provider value={{}}>
        {props.children}
    </ReactElectronContext.Provider >;

}

export { withElectron };
export default withStoreProvider(withSnackBar(injectIntl(ElectronProvider)));