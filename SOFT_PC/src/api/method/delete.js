import fetch from 'isomorphic-fetch';

export default function del(url, data, config) {
    return new Promise(async (resolve, reject) => {
        try {

            let _c = {
                method: 'DELETE',
                body: typeof data == 'string' ? data : JSON.stringify(data),
                cache: "no-cache",
                useElectronNet: false,
            };

            if (config) {
                if (config.headers) {
                    _c.headers = config.headers;
                }
            }
            const response = await fetch(url, _c);
            let r = await response.text();
            resolve(r);
        } catch (err) {
            reject(err);
        }
    });
}