import fetch from 'electron-fetch';

export default function post(url, data, config = {}) {
    return new Promise(async (resolve, reject) => {
        try {

            let _c = {
                method: 'POST',
                body: typeof data == 'string' ? data : JSON.stringify(data),
                useElectronNet: false,
                credentials: "same-origin",
                useSessionCookies : true
            };

            if (config) {
                if (config.headers) {
                    _c.headers = config.headers;
                }
            }

            _c.headers = {..._c.headers, "cookie" : global.cookieString };

            const response = await fetch(url, _c);
            
            let r = await response.text();
            resolve(r);
        } catch (err) {
            reject(err);
        }
    });
}