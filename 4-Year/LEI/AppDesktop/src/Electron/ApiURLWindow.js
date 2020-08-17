const path = require('path');
const { BrowserWindow } = require('electron');
const Positioner = require('electron-positioner');


class ApiURLWindow {
    constructor(type,par) {
      this.window = new BrowserWindow({
        width: 600,
        height: 450,
        titleBarStyle: 'hidden',
        movable: true,
        resizable: false,

        webPreferences: {
          //nodeIntegration: false,
          //preload: MAIN_WINDOW_PRELOAD_WEBPACK_ENTRY,
          preload: OUTLOOK_MODAL_PRELOAD_WEBPACK_ENTRY,
          webviewTag: true,
          contextIsolation: true,
        },
        show: true,
        modal: true,
        parent: par,
      })

      switch (type) {
        case 0:
          this.window.loadURL(GITHUB_MODAL_WEBPACK_ENTRY);
          break;
        case 1:
          this.window.loadURL(GOOGLE_MODAL_WEBPACK_ENTRY);
          break;
        case 2:
          this.window.loadURL(OUTLOOK_MODAL_WEBPACK_ENTRY);
          break;
        default:

      }
      //this.window.webContents.openDevTools();
      /**
      this.window.once('ready-to-show', () => {
        this.window.show();
        par.window.hide();
      });

      this.window.on('closed',() => {
        par.window.show();
      })*/
    }
}


module.exports = ApiURLWindow;
