const path = require('path');
const { BrowserWindow } = require('electron');
const Positioner = require('electron-positioner');

class LoadingWindow {
    constructor() {
      this.window = new BrowserWindow({
        show: false,
        width: 200,
        height: 100,
        frame: false,
        movable: false,
        resizable: false,
      })

      this.window.loadURL(LOADING_WINDOW_WEBPACK_ENTRY);

      this.window.on('show', () => {
        let positioner = new Positioner(this.window);
        positioner.move('center');
      });
    }
}


module.exports = LoadingWindow;
