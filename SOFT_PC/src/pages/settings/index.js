import React, { useState, useEffect } from 'react';
import { injectIntl } from 'react-intl';
import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';
import { withSnackBar } from '../../providers/snackBar';

import Box from '@mui/material/Box';
import Grid from '@mui/material/Grid';
import Select from '@mui/material/Select';
import MenuItem from '@mui/material/MenuItem';
import Typography from '@mui/material/Typography';
import Divider from '@mui/material/Divider';
import Button from '@mui/material/Button';

import actions from '../../actions';

function SettingsPage(props) {

    const settings = props.globalState.settings;
    const system = props.globalState.system;
    const device = props.globalState.device;
    const intl = props.intl;

    const [serialPorts, setSerialPorts] = useState(system.ports);
    const [nbCylindres, setNbCylindres] = useState(undefined);

    if ((nbCylindres == undefined) && (device?.nbCylindres != undefined)) {
        setNbCylindres(device?.nbCylindres);
    }

    async function fetchData() {
        try {
            let _system = await props.dispatch(actions.serial.findInterface());
            setSerialPorts(_system.system.ports);
        } catch (err) {

        }
    }

    useEffect(() => {
        fetchData();
    }, []);

    return <Box>
        <Box sx={{ paddingTop: '32px' }}>
            <Typography variant="h6" component="h6">
                {props.intl.formatMessage({ id: "settings.detection" })}
            </Typography>
            <Divider />
            <Grid container sx={{ paddingTop: "16px" }}>
                <Grid item xs={10} sx={{ marginTop: "auto", marginBottom: "auto", textAlign: 'left' }}>
                    <Typography variant="h7" component="h7"> {props.intl.formatMessage({ id: "settings.portCom" })}  </Typography>
                </Grid>
                <Grid item xs={2} sx={{ textAlign: 'right', paddingRight: '5px', marginTop: "auto", marginBottom: "auto" }}>
                    <Select
                        variant="standard"
                        labelId="select-detection-label"
                        id="select-detection"
                        value={settings?.port}
                        onChange={async (event) => {
                            settings.port = event.target.value;
                            await props.dispatch(actions.settings.update(settings));
                        }}
                        label="Detection"
                        sx={{ width: '100%' }}
                    >
                        {serialPorts.map((port) => {
                            return <MenuItem value={port.path} key={"port_" + port.path}>{port.path}</MenuItem>
                        })}
                    </Select>
                </Grid>
            </Grid>
            <br /><br />
            <Typography variant="h6" component="h6">
                {props.intl.formatMessage({ id: "settings.device" })}
            </Typography>
            <Divider />
            <Grid container sx={{ paddingTop: "16px" }}>
                <Grid item xs={10} sx={{ marginTop: "auto", marginBottom: "auto", textAlign: 'left' }}>
                    <Typography variant="h7" component="h7"> {props.intl.formatMessage({ id: "settings.NBCylindres" })}  </Typography>
                </Grid>
                <Grid item xs={2} sx={{ textAlign: 'right', paddingRight: '5px', marginTop: "auto", marginBottom: "auto" }}>
                    <Select
                        variant="standard"
                        labelId="select-detection-label"
                        id="select-detection"
                        value={nbCylindres}
                        onChange={async (event) => {
                            try {
                               // await props.dispatch(actions.device.writeNbCylindres(event.target.value));
                                //device.nbCylindres = event.target.value;
                                setNbCylindres(event.target.value);
                            } catch (err) {

                            }
                        }}
                        label="Detection"
                        sx={{ width: '100%' }}
                    >
                        <MenuItem value={1} >Un</MenuItem>
                        <MenuItem value={2} >Deux</MenuItem>
                        <MenuItem value={3} >Trois</MenuItem>
                        <MenuItem value={4} >Quatre</MenuItem>
                    </Select>
                </Grid>
            </Grid>

        </Box>
        <Box sx={{ paddingTop: '32px' }}>
            <Grid container sx={{ paddingTop: "16px" }}>
                <Grid item xs={9}></Grid>
                <Grid item xs={3} sx={{ marginTop: "auto", marginBottom: "auto", textAlign: 'right' }}>
                    <Button variant="contained" onClick={async () => {
                        try {
                            await props.dispatch(actions.serial.open(settings?.port));
                            await props.dispatch(actions.device.writeNbCylindres(nbCylindres));
                            //await props.dispatch(actions.electron.SetDebugMode(settings.debugMode));
                        } catch (err) {
                            props.snackbar.error(intl.formatMessage({ id: err }));
                        }
                    }}>{props.intl.formatMessage({ id: "settings.validate" })}</Button>
                </Grid>
            </Grid>
        </Box>
    </Box>;
}

export default withSnackBar(withNavigation(withStoreProvider(injectIntl(SettingsPage))));