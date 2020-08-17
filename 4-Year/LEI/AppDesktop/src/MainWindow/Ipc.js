//const ipcRenderer = window.ipcRenderer
const { ipcRenderer } = require('electron')

export async function store_google_api_key(key){
  const result = await ipcRenderer.invoke('store_google_api_key',key);
  return result;
}

export async function cancel_todo_id(id){
  const result = await ipcRenderer.invoke('cancel_todo_id', id);
  return result;
}

export async function complete_todo_id(id){
  const result = await ipcRenderer.invoke('complete_todo_id', id);
  return result;
}

export async function update_list_index(list){
  const result = await ipcRenderer.invoke('update_list_index', list);
  return result;
}

export async function get_all_todos(){
  const result = await ipcRenderer.invoke('get_all_todos');
  return result;
}

export async function get_git_todos(){
  const result = await ipcRenderer.invoke('get_git_todos');
  return result;
}

export async function get_google_todos(){
  const result = await ipcRenderer.invoke('get_google_todos');
  return result;
}

export async function get_outlook_todos(){
  const result = await ipcRenderer.invoke('get_outlook_todos');
  return result;
}

export async function add_todo(todo){
  const result = await ipcRenderer.invoke('add_todo',todo);
  return result;
}

export async function history(){
  const result = await ipcRenderer.invoke('history');
  return result;
}

export function triggerGOOGLE_URL(){
  ipcRenderer.send('trigger-google-url');
}

export function triggerOUTLOOK_URL(){
  ipcRenderer.send('trigger-outlook-url');
}

export function triggerGITHUB_URL(){
  ipcRenderer.send('trigger-github-url');
}

export async function getGOOGLEURL(){
  const result = await ipcRenderer.invoke('url-google');
  return result;
}

export async function getOUTLOOKURL(){
  const result = await ipcRenderer.invoke('url-outlook');
  return result;
}

export async function getGITHUBURL(){
  const result = await ipcRenderer.invoke('url-github');
  return result;
}

export async function verify_Github_Key(){
  const result = await ipcRenderer.invoke('verify-github-key');
  return result;
}

export async function verify_Outlook_Key(){
  const result = await ipcRenderer.invoke('verify-outlook-key');
  return result;
}

export async function login(email,pass){
  const result = await ipcRenderer.invoke('log-in',email,pass);
  return result;
}

export async function register(email,pass){
  const result = await ipcRenderer.invoke('register',email,pass);
  return result;
}

export async function sync(){
  const result = await ipcRenderer.invoke('sync');
  return result;
}

export default {store_google_api_key,
                cancel_todo_id,
                complete_todo_id,
                update_list_index,
                get_all_todos,
                get_git_todos,
                get_google_todos,
                get_outlook_todos,
                add_todo,
                history,
                triggerGOOGLE_URL,
                triggerOUTLOOK_URL,
                triggerGITHUB_URL,
                getGOOGLEURL,
                getOUTLOOKURL,
                getGITHUBURL,
                verify_Outlook_Key,
                verify_Github_Key,
                login,
                register,
                sync
              }
