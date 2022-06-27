import fetch from 'electron-fetch';

global.cookieString = "";

export default function get(url, config = {}) {
    return new Promise(async (resolve, reject) => {
        try {
            const controller = new AbortController();
            const id = setTimeout(() => controller.abort(), 30000);

            config.headers = {...config.headers, "set-cookie" : global.cookieString };
            const response = await fetch(url , { ...config , useElectronNet : false , signal: controller.signal , credentials: "same-origin" , useSessionCookies : true});

            clearTimeout(id);

            global.cookieString = response.headers.get('set-cookie');
            
            if ( response.status == 200){
                let r = undefined;
                if ( config && config.responseType && config.responseType == 'arraybuffer'){
                    r = await response.arrayBuffer();
                } else {
                    r = await response.text();
                }
                resolve(r)
            } else if ( response.status == 404 ){
                reject({message : "Not found"});
            } else if ( response.status == 500 ){
                reject({message : "Temporary unavailable"});
            }
        } catch (err) {
            reject(err);
        }
    });
}
