import React, { Component } from 'react';
import CanvasJSReact from '../../utils/canvasjs/canvasjs.react';

var CanvasJS = CanvasJSReact.CanvasJS;
var CanvasJSChart = CanvasJSReact.CanvasJSChart;

class Chart extends Component {
    constructor(props) {
        super(props);

        this.state = {
            data: [
                { x: 0, y: 0 },
                { x: 500, y: 16 },
                { x: 1000, y: 10 },
                { x: 1500, y: 16 },
                { x: 2000, y: 18 },
                { x: 2500, y: 22 },
                { x: 3000, y: 26 },
                { x: 3500, y: 28 },
                { x: 4000, y: 28 },
                { x: 4500, y: 28 },
                { x: 5000, y: 29 },
                { x: 5500, y: 30 },
                { x: 6000, y: 32 },
                { x: 6500, y: 32 },
                { x: 7000, y: 32 }
            ]
        }

        this.mouseDown = false;
        this.selected = null;
        this.xSnapDistance;
        this.ySnapDistance;
        this.timerId = null;
    }

    getPosition = (e) => {

        var dim = document.getElementsByClassName('canvasjs-chart-container')[0].getBoundingClientRect();
        var relX = e.pageX - dim.left;
        var relY = e.pageY - dim.top;
        this.xValue = Math.round(this.chart.axisX[0].convertPixelToValue(relX));
        this.yValue = Math.round(this.chart.axisY[0].convertPixelToValue(relY));

    }

    searchDataPoint = () => {
        var dps = this.chart.data[0].dataPoints;

        for (var i = 0; i < dps.length; i++) {

            if ((this.xValue >= dps[i].x - this.xSnapDistance && this.xValue <= dps[i].x + this.xSnapDistance) &&
                (this.yValue >= dps[i].y - this.ySnapDistance && this.yValue <= dps[i].y + this.ySnapDistance)) {
                if (this.mouseDown) {
                    this.selected = i;
                    break;
                }
                else {
                    this.changeCursor = true;
                    break;
                }
            }
            else {
                this.selected = null;
                this.changeCursor = false;
            }
        }
    }

    componentDidMount() {
        document.getElementsByClassName('canvasjs-chart-container')[0].addEventListener('mousedown', (e) => {
            this.xSnapDistance = 30;//this.chart.axisX[0].convertPixelToValue(this.chart.get("dataPointWidth")) / 2;
            this.ySnapDistance = 3;

            this.mouseDown = true;
            this.getPosition(e);
            this.searchDataPoint();
        });

        document.getElementsByClassName('canvasjs-chart-container')[0].addEventListener('mouseup', (e) => {

            if (this.selected != null) {
                this.chart.data[0].dataPoints[this.selected].y = this.yValue;
                this.chart.render();
                this.mouseDown = false;
            }
        });

        document.getElementsByClassName('canvasjs-chart-container')[0].addEventListener('mousemove', (e) => {
            this.getPosition(e);

            if (this.mouseDown) {
                clearTimeout(this.timerId);
                this.timerId = setTimeout(() => {
                    if (this.selected != null) {
                        this.chart.data[0].dataPoints[this.selected].y = this.yValue;
                        this.setState({})
                        this.chart.render();
                    }
                }, 0);
            } else {
                this.searchDataPoint();
                if (this.changeCursor) {
                    this.chart.data[0].set("cursor", "n-resize");
                }
                else {
                    this.chart.data[0].set("cursor", "default");
                }
            }
        });
    }

    render() {

        const options = {
            animationEnabled: true,
            //exportEnabled: true,
            theme: "light2", // "light1", "dark1", "dark2"
            title: {
                text: "Courbe d'avance",
            },
            axisY: {
                minimum: 0,
                maximum: 90
            },
            axisX: {
                minimum: 0,
                maximum: 7000
            },
            dataPointWidth: 20,
            dataPointMaxWidth: 20,
            data: [
                {
                    type: "line",
                    dataPoints: this.state.data
                }
            ],

        }

        return <CanvasJSChart
            options={options}
            onRef={ref => this.chart = ref}
            ref="something"
        />
    }
}

export default Chart