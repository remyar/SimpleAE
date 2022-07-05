import React, { useState, useEffect } from 'react';
import { Route } from 'react-router-dom';
import { injectIntl } from 'react-intl';
import { withStoreProvider } from './providers/StoreProvider';
import { withSnackBar } from './providers/snackBar';
import routeMdw from './middleware/route';
import actions from './actions';

import HomePage from './pages/home';
import SettingsPage from './pages/settings';

import Container from '@mui/material/Container';
import Box from '@mui/material/Box';

import AppBar from './components/AppBar';
import Drawer from './components/Drawer';

import electron from 'electron';

const routes = [
    { path: routeMdw.urlIndex(), name: 'HomePage', Component: HomePage },
    { path: routeMdw.urlSettings(), name: 'SettingsPage', Component: SettingsPage },
];

function App(props) {

    const intl = props.intl;
    const settings = props.globalState.settings;

    const [drawerState, setDrawerState] = useState(false);

    async function _Sleep(val){
        return new Promise((resolve)=>{
            setTimeout(()=>{
                resolve();
            } , val)
        })
    }
    async function fetchData(){
        try{
            let _r = await props.dispatch(actions.serial.findInterface());
            if ( _r.system.ports.find(el => el.path === settings.port)){
                await props.dispatch(actions.serial.open(settings.port));
                await _Sleep(3000);
                await props.dispatch(actions.device.readCourbe());
                await props.dispatch(actions.device.readNbCylindres());
                setInterval(()=>{
                    props.dispatch(actions.device.readValues());
                },200)
            } else {
                props.snackbar.error(intl.formatMessage({ id: 'interface.not.detected' }));
            }
        }catch(err){
            props.snackbar.error(err.message);
        }
    }

    useEffect(() => {

        fetchData();

    }, []);

    return <Box>
        <AppBar onClick={() => { setDrawerState(true) }} title={undefined}/>
        <Box sx={{paddingTop:'64px'}}>
            <Container maxWidth="xl" sx={{ /*height: 'calc(100vh - 64px)',*/ paddingTop: "25px"}} >
                <Drawer
                    open={drawerState}
                    onClose={() => { setDrawerState(false) }}
                />
                {routes.map(({ path, Component }) => (
                    <Route path={path} key={path} exact component={Component} />
                ))}
            </Container>
        </Box>
    </Box>;
}

export default withStoreProvider(withSnackBar(injectIntl(App)));
