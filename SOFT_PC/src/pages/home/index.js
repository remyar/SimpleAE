import React, { useState, useRef } from 'react';
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

import Loader from '../../components/Loader';
import ReactRadialGauge from '../../components/Gauges/react.radial.gauge';
import Chart from '../../components/Chart';

import actions from '../../actions';

function HomePage(props) {

    const [displayLoader, setDisplayLoader] = useState(false);

    const settings = props.globalState.settings;
    const system = props.globalState.system;
    const device = props.globalState.device;

    return <Box>

        <Loader display={displayLoader} />

        <Box sx={{ paddingTop: '32px' }}>

       
            <Grid container spacing={2}>

                <Grid item xs={4} sx={{ textAlign: 'center' }}>
                    <ReactRadialGauge
                        units='Rpm x 10'
                        title='RPM'
                        value={device?.rpm/10}
                        minValue={0}
                        maxValue={700}
                        width={300}
                        highlights={[
                            { from: 0, to: 50, color: 'rgba(0,0,0,.0)' },
                            { from: 600, to: 700, color: 'rgba(255,0,0,0.5)' },
                        ]}
                        height={300}
                        animation={false}
                        valueDec={0}
                        majorTicks={['0', '100', '200', '300', '400', '500', '600', '700']}
                        minorTicks={2}
                    ></ReactRadialGauge>
                </Grid>

                <Grid item xs={4} sx={{ textAlign: 'center' }}>
                    <ReactRadialGauge
                        units='°'
                        title='Advance'
                        value={device?.advance}
                        minValue={0}
                        maxValue={50}
                        width={300}
                        height={300}
                        majorTicks={['0', '5', '15', '20', '25', '30', '35', '40', '45', '50']}
                        minorTicks={2}
                    ></ReactRadialGauge>
                </Grid>

                <Grid item xs={4} sx={{ textAlign: 'center' }}>
                    <ReactRadialGauge
                        units='ms'
                        title='Dwell'
                        value={device?.dwell}
                        minValue={0}
                        maxValue={50}
                        width={300}
                        height={300}
                        majorTicks={['0', '5', '15', '20', '25', '30', '35', '40', '45', '50']}
                        minorTicks={2}
                    ></ReactRadialGauge>
                </Grid>

            </Grid>
            <br />
            <Grid container spacing={2}>
                <Grid item xs={2} />
                <Grid item xs={8} sx={{ textAlign: 'center' }}>
                    <Chart />
                </Grid>
                <Grid item xs={2} />
            </Grid>
        </Box>
    </Box >

}

export default withStoreProvider(withSnackBar(withNavigation(injectIntl(HomePage))));