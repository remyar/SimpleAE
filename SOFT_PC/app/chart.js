
let chart = undefined;

module.exports = {
    draw: () => {

        //Better to construct options first and then pass it as a parameter
        chart = new CanvasJS.Chart("chartContainer",
            {
                axisX: {
                    minimum: 0,
                    maximum: 7000
                },
                axisY: {
                    minimum: 0,
                    maximum: 90
                },
                title: {
                    text: "Try dragging column to reposition dataPoint",
                },
                dataPointMaxWidth: 20,
                data: [
                    {
                        type: "line",
                        dataPoints: [
                            { x: 500, y: 71 },
                            { x: 1000, y: 55 },
                            { x: 1500, y: 50 },
                            { x: 2000, y: 65 },
                            { x: 2500, y: 95 },
                            { x: 3000, y: 68 },
                            { x: 3500, y: 28 },
                            { x: 4000, y: 34 },
                            { x: 4500, y: 14 }
                        ]
                    }
                ]
            });

        chart.render();

        var xSnapDistance = 30;
        var ySnapDistance = 3;

        var xValue, yValue;
        var mouseDown = false;
        var selected = null;
        var changeCursor = false;
        var timerId = null;
        function getPosition(e) {
            var parentOffset = $("#chartContainer > .canvasjs-chart-container").offset();
            var relX = e.pageX - parentOffset.left;
            var relY = e.pageY - parentOffset.top;
            xValue = Math.round(chart.axisX[0].convertPixelToValue(relX));
            yValue = Math.round(chart.axisY[0].convertPixelToValue(relY));
        }
        function searchDataPoint() {
            var dps = chart.data[0].dataPoints;
            for (var i = 0; i < dps.length; i++) {
                if ((xValue >= dps[i].x - xSnapDistance && xValue <= dps[i].x + xSnapDistance) &&
                    (yValue >= dps[i].y - ySnapDistance && yValue <= dps[i].y + ySnapDistance)) {
                    if (mouseDown) {
                        selected = i;
                        break;
                    }
                    else {
                        changeCursor = true;
                        break;
                    }
                } else {
                    selected = null;
                    changeCursor = false;
                }
            }

        }

        jQuery("#chartContainer > .canvasjs-chart-container").on({
            mousedown: function (e) {
                mouseDown = true;
                getPosition(e);
                searchDataPoint();
            },
            mousemove: function (e) {
                getPosition(e);
                if (mouseDown) {
                    clearTimeout(timerId);
                    timerId = setTimeout(function () {
                        if (selected != null) {
                            chart.data[0].dataPoints[selected].y = Math.max(Math.min(yValue,90),0);
                            chart.render();
                        }
                    }, 0);
                }
                else {
                    searchDataPoint();
                    if (changeCursor) {
                        chart.data[0].set("cursor", "n-resize");
                    } else {
                        chart.data[0].set("cursor", "default");
                    }
                }
            },
            mouseup: function (e) {
                if (selected != null) {
                    chart.data[0].dataPoints[selected].y = yValue;
                    chart.render();
                    mouseDown = false;
                }
            }
        });
    },
    clear : () => {
        chart.data[0].set("dataPoints", []);
    },
    addpoint : (x , y)=>{
        chart.data[0].addTo("dataPoints" , { x , y});
    },
    redraw : ()=>{
        chart.render();
    }
}