const { ipcRenderer,contextBridge } = require('electron')
import API_MAIN from '../MainWindow/Ipc';

contextBridge.exposeInMainWorld(
  'API',
  {
    GOOGLE_URL: API_MAIN.getGOOGLEURL,
    OUTLOOK_URL: API_MAIN.getOUTLOOKURL,
    GITHUB_URL: API_MAIN.getGITHUBURL,
    close_modal: () => ipcRenderer.send('close-modal'),
  }
)
