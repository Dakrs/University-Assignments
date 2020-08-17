const { ipcRenderer,contextBridge } = require('electron')
const Store = require('electron-store');
import API_MAIN from './Ipc';
const store = new Store();

//window.Test = false
//store.set('GOOGLE_API_KEY',true);
//store.delete('GOOGLE_API_KEY')

/**
console.log('welel');

ipcRenderer.on('testing',(event,arg) => {
  console.log(arg);
})

ipcRenderer.send('test','ests');*/

contextBridge.exposeInMainWorld(
  'API',
  {
    getGOOGLE_KEY_STATUS: () => {
      var key = store.get('GOOGLE_API_KEY');
      return (typeof key !== 'undefined');
    },
    getOUTLOOK_KEY_STATUS: () => {
      var key = store.get('OUTLOOK_API_KEY');
      return (typeof key !== 'undefined');
    },
    getGITHUB_KEY_STATUS: () => {
      var key = store.get('GITHUB_API_KEY');
      return (typeof key !== 'undefined');
    },
    getJWT: () => {
      var key = store.get('JWT_TOKEN');
      return (typeof key !== 'undefined');
    },
    Ipc: API_MAIN,
  }
)
