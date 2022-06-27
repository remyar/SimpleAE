import React, { Component } from 'react';

import ReactElectronContext from './context';

export default (WrappedComponent) => {

    class WithElectronComponent extends Component {

        renderWrappedComponent = ({ navigation }) => (
            <WrappedComponent {...this.props} navigation={navigation} />
        );

        render() {
            return <ReactElectronContext.Consumer>{this.renderWrappedComponent}</ReactElectronContext.Consumer>;
        }
    }

    return WithElectronComponent;
}

