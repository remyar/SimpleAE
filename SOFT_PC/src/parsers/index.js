import createAction from '../middleware/actions';
import { formatDateTimeRange } from '@formatjs/intl/src/dateTime';

export default createAction(async (message, { getState, extra }) => {

    let globalState = getState();

    try {
        console.log("message ", message);
        message = message.replace('[', '').replace(']', '');

        let topic = message.split(':')[0];

        console.log("message ", topic);

        message = message.replace(topic, '').replace(':', '');
        console.log("value ", message);

        switch (topic) {

            case "IV": {
                //-- Immediate Value
                let values = message.split(':');
                let device = {
                    rpm: values[0],
                    advance: values[1],
                    dwell: values[2],
                };
                if (globalState.device == undefined) {
                    globalState.device = {};
                }
                globalState.device = { ...device };
                break;
            }
            case "RC": {
                let device = {
                    advanceTable: []
                }
                let values = message.split(':').map((el) => parseInt(el));
                device.advanceTable = [...values];

                globalState.device = {
                    ...globalState.device,
                    ...device
                }
                break;
            }
            case "RNBC": {
                let device = {
                    nbCylindres: message,
                }

                globalState.device = {
                    ...globalState.device,
                    ...device
                }
                break;
            }
            /*
            case "ID":
                //-- on récupere les infos des differents devices en relation avec le dongle
                let result = message.indexOf(":");
                let id = message.substr(0,result);
                message = message.replace(id+":","");
                let mac = message;
                let devices = globalState.devices;
                let _d = devices.find((el) => el.mac == mac);
                if ( _d == undefined ){
                    devices.push({
                        id,
                        mac,
                        connected : true,
                        timeout : new Date().getTime()
                    });
                } else {
                    _d.timeout = new Date().getTime();
                }

                break;
            case "SF":
                let line = 0;
                let time = undefined;
                if (message.includes('?')) {
                    line = message.split('?')[0];
                } else if (message.includes('$')) {
                    line = message.split('$')[0];
                    time = parseInt(message.split('$')[1]);
                } else {
                    line = parseInt(message);
                }

                if (globalState.race == undefined) {
                    globalState.race = { lines: [] };
                }
                let race = globalState.race;

                let find = race?.lines?.find((el) => el.line == line);
                if (find == undefined) {
                    race.lines.push({ line: line, laps: -1, bestTime: time });
                    find = race.lines.find((el) => el.line == line);
                }

                if (message.includes('?')) {
                    time = parseInt(message.split('?')[1]);
                } else if (message.includes('$')) {
                    time = parseInt(message.split('$')[1]);
                } else {
                    time = new Date().getTime() - find.lastTime;
                }


                find.laps += 1;
                find.lastTime = new Date().getTime();
                find.time = time;
                find.bestTime = Math.min(find.time, find.bestTime == 0 ? 9999999 : find.bestTime);

                break;
                */
        }
        return { ...globalState };

    } catch (ex) {

    }
});