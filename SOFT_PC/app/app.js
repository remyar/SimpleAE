
const { RadialGauge } = require('canvas-gauges');
const chart = require('./chart');
const compc = require('./compc');

window.onload = async () => {
    chart.draw();

    compc.init();

    let config = await compc.readConfig();

    $("#inputGroupSelect01").val(config.nbCylindres);
    $("#inputGroupSelect01").on('change',(event) => {
        compc.changeNbCylindres(event.target.value);
    });

    chart.clear();
    
    for ( let i = 0 ; i < 16 ; i++){
        chart.addpoint(config.Na[i] , config.Anga[i]);
    }

    compc.setRpmGauge(new RadialGauge({
        renderTo: 'rpmEngine',
        width: 300,
        height: 300,
        minValue: 0,
        maxValue: 7000,
        units: "Rpm x 10",
        animation:false,
        valueDec : 0,
        //title: "",
    }));
    compc.setAdvanceGauge(new RadialGauge({
        renderTo: 'advanceSpark',
        width: 300,
        height: 300,
        minValue: 0,
        maxValue: 10000,
        units: "us",
        animation:false,
        valueDec : 0,
    }));
    compc.setDwellGauge(new RadialGauge({
        renderTo: 'dwell',
        width: 300,
        height: 300,
        units: "",
        //title: "",
    }));

    $('#loader').hide(500);
    $('#appRoot').show(500,()=>{
        chart.redraw();
    });
}