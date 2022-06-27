import React, { useState, useEffect } from 'react';
import { injectIntl } from 'react-intl';
import { withNavigation } from '../../providers/navigation';

import Drawer from '@mui/material/Drawer';
import List from '@mui/material/List';
import ListItem from '@mui/material/ListItem';
import ListItemIcon from '@mui/material/ListItemIcon';
import ListItemText from '@mui/material/ListItemText';

import MemoryIcon from '@mui/icons-material/Memory';
import FileCopyIcon from '@mui/icons-material/FileCopy';
import HomeIcon from '@mui/icons-material/Home';
import PeopleAltIcon from '@mui/icons-material/PeopleAlt';
import EditLocationIcon from '@mui/icons-material/EditLocation';
import SettingsIcon from '@mui/icons-material/Settings';
import DirectionsCarIcon from '@mui/icons-material/DirectionsCar';
import InventoryIcon from '@mui/icons-material/Inventory';
import AddCardIcon from '@mui/icons-material/AddCard';

import routeMdw from '../../middleware/route';

function MyDrawer(props) {
    const intl = props.intl;
    return <Drawer open={props.open} onClose={() => { props.onClose && props.onClose() }}>
        <List>

            <ListItem button onClick={() => {
                props.navigation.push(routeMdw.urlIndex());
                props.onClose && props.onClose();
            }}>
                <ListItemIcon>
                    <HomeIcon />
                </ListItemIcon>
                <ListItemText primary={intl.formatMessage({ id: 'url.index' })} secondary={intl.formatMessage({ id: 'url.index.desc' })} />
            </ListItem>
           
        </List>
        <List sx={{ position: "absolute", bottom: "0px", width: "100%" }}>
            <ListItem button onClick={() => {
                props.navigation.push(routeMdw.urlSettings());
                props.onClose && props.onClose();
            }} >
                <ListItemIcon>
                    <SettingsIcon />
                </ListItemIcon>
                <ListItemText primary={intl.formatMessage({ id: 'url.settings' })} secondary={intl.formatMessage({ id: 'url.settings.desc' })} />
            </ListItem>
        </List>
    </Drawer>;
}


export default withNavigation(injectIntl(MyDrawer));