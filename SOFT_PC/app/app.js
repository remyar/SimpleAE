
const { RadialGauge } = require('canvas-gauges');
const chart = require('./chart');
const compc = require('./compc');

window.onload = () => {
    chart.draw();
    compc.init();

    compc.setRpmGauge(new RadialGauge({
        renderTo: 'rpmEngine',
        width: 300,
        height: 300,
        units: "",
        //title: "",
    }));
    compc.setAdvanceGauge(new RadialGauge({
        renderTo: 'advanceSpark',
        width: 300,
        height: 300,
        units: "",
        //title: "",
    }));
    compc.setDwellGauge(new RadialGauge({
        renderTo: 'dwell',
        width: 300,
        height: 300,
        units: "",
        //title: "",
    }));
}