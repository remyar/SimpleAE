import React, { useState, useEffect , useRef } from 'react';
import { RadialGauge } from 'canvas-gauges'


function ReactRadialGauge(props) {

    const nodeRef = useRef();
    const [gauge , setGauge] = useState(undefined);

    useEffect(() => {
        const options = Object.assign({}, props, { renderTo : nodeRef.current});
        setGauge(new RadialGauge(options).draw());
    }, []);

    if ( gauge != undefined ){
        gauge.value = props.value;
        gauge.update(props);
    }

    return <canvas ref={nodeRef} />
}


export default ReactRadialGauge;