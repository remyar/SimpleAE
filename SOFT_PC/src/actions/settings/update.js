import createAction from '../../middleware/actions';

export default createAction(async (settings, { getState, extra }) => {

    let globalState = getState();
    try {
        globalState.settings = { ...settings };
        return { ...globalState }
    } catch (ex) {

    }
    
});