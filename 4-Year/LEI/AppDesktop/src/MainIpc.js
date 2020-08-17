const { ipcMain   } = require('electron');
const axios = require('axios');
var nanoid = require('nanoid');

const Store = require('electron-store');
const store = new Store();


function getTrue() {
  return new Promise(resolve => {
    setTimeout(() => {
      resolve(true);
    }, 200);
  });
}

export default function setIpc(){

  async function sync(){

    if(!store.has('JWT_TOKEN')) {
      return null;
    }

    const token = store.get('JWT_TOKEN');

    var response;
    var register;

    try {
      response = await axios.put('https://amcyni.herokuapp.com/api/lock',null,{headers: {'x-access-token': token}});
    }
    catch (err){
      console.log('lock error: ' + err.response.status);
      return null;
    }

    try{
      response = await axios.get('http://localhost:4545/api/register');
      register = response.data[0];
    }
    catch(err){
      //libertar o lock
      return null;
    }
    // buscar as transações remote

    var transactions_from_global = [];
    try{
      response = await axios.get('https://amcyni.herokuapp.com/api/transaction/' + register.global,{headers: {'x-access-token': token}});
      transactions_from_global = response.data;
    }
    catch(err){
      return null;
    }

    var list_trans_uncommited = [];
    try{
      response = await axios.get('http://localhost:4545/api/transactions',register);
      list_trans_uncommited = response.data;
    }
    catch(err){
      return null;
    }

    var transactions_to_update = [];
    var transactions_to_perform = [];

    for(var i = 0; i < transactions_from_global.length; i++){
      var dependency = false;
      for(var j = 0; j < list_trans_uncommited.length; j++){
        if(transactions_from_global[i].idOrigin === list_trans_uncommited[j].idOrigin){
          switch (transactions_from_global[i].type) {
            case 'Post':
              if (list_trans_uncommited[j].type === 'Post'){
                dependency = true;
                list_trans_uncommited.splice(j,1);
                console.log('entrei');
              }
              break;
            case 'confirm':
              if (list_trans_uncommited[j].type === 'confirm') {
                dependency = true;
                list_trans_uncommited.splice(j,1);
              }
              else if (list_trans_uncommited[j].type === 'cancel') {
                dependency = true;
                transactions_to_update.push({dep: transactions_from_global[i], id: list_trans_uncommited[j]._id});
                list_trans_uncommited.splice(j,1);
              }
              break;
            case 'cancel':
              if (list_trans_uncommited[j].type === 'cancel') {
                dependency = true;
                list_trans_uncommited.splice(j,1);
              }
              else if (list_trans_uncommited[j].type === 'confirm') {
                dependency = true;
                transactions_to_update.push({dep: transactions_from_global[i], id: list_trans_uncommited[j]._id});
                list_trans_uncommited.splice(j,1);
              }
              break;
            default:
          }
        }
        if (dependency){
          break;
        }
      }

      if(!dependency){
        transactions_to_perform.push(transactions_from_global[i]);
      }
    }

    for(var i = 0; i < transactions_to_perform.length; i++){
      try{
        await axios.post('http://localhost:4545/api/transactionTotask',transactions_to_perform[i]);
      }
      catch(err){
        console.log(err);
      }
    }

    for(var i = 0; i < transactions_to_update.length; i++){
      try{
        await axios.put('http://localhost:4545/api/updatetransaction',transactions_to_update[i]);
      }
      catch(err){
        console.log(err);
      }
    }

    var gs;
    try{
      response = await axios.post('https://amcyni.herokuapp.com/api/transactions',{transactions: list_trans_uncommited},{headers: {'x-access-token': token}});
      gs = response.data;
    }
    catch(err){
      console.log(err);
      return null;
    }

    try{
      response = await axios.put('http://localhost:4545/api/register/'+ register._id,{global: gs, local: gs});
    }
    catch(err){
      console.log(err);
      return null;
    }

    console.log('Sync Completed');
    return true;
  }


  ipcMain.handle('verify-outlook-key',async (event,arg) => {
    //perguntar à api se a key do outlook existe
    var response=false
    try {
      var aux = await axios.get('http://localhost:4545/outlook/verify',)
      response=aux.data;
    }
    catch(err){
      console.error("Erro",err)
    }
    if (response === true){
      store.set('OUTLOOK_API_KEY',true);

      if(store.has('JWT_TOKEN')){
          var token = store.get('JWT_TOKEN');
          console.log(token);
          var resp2;
          try{
            resp2 = await axios.get('https://amcyni.herokuapp.com/outlook/credentials',{headers: {'x-access-token': token}});
            if (resp2.data.length === 0){
              resp2 = await axios.get('http://localhost:4545/api/outlookToken');
              await axios.post('https://amcyni.herokuapp.com/api/outlook/token',{token: resp2.data.token},{headers: {'x-access-token': token}});
            }
          }
          catch(err){
            console.log(err);
          }
      }
    }

    return response;
   })

   ipcMain.handle('verify-github-key',async (event,arg) => {
    var response=false
    try {
      var aux = await axios.get('http://localhost:4545/github/verify',)
      response=aux.data;
    }
    catch(err){
      console.error("Erro",err)
    }

    if (response === true){
        store.set('GITHUB_API_KEY',true);

        if(store.has('JWT_TOKEN')){
          var token = store.get('JWT_TOKEN');
          var resp2;
          try{
            resp2 = await axios.get('https://amcyni.herokuapp.com/github/credentials',{headers: {'x-access-token': token}});
            if (resp2.data.length === 0){
              resp2 = await axios.get('http://localhost:4545/api/githubToken');
              await axios.post('https://amcyni.herokuapp.com/api/github/token',{token: resp2.data.token},{headers: {'x-access-token': token}});
            }
          }
          catch(err){
            console.log(err);
          }
      }
    }

    return response;
   });

  ipcMain.handle('url-outlook',async (event,arg) => {
    let response = await axios.get('http://localhost:4545/outlook/url');
    var url = response.data;
    return url
  });

  ipcMain.handle('url-google',async (event,arg) => {
    let response = await axios.get('http://localhost:4545/google/url');
    var url = response.data;
    return url
  });

  ipcMain.handle('url-github',async (event,arg) => {
    let response = await axios.get('http://localhost:4545/github/url');
    var url = response.data;
    console.log(url);
    return url
  });

  ipcMain.handle('store_google_api_key', async (event, ...args) => {
    var response=false
    try {
       var aux = await axios.post('http://localhost:4545/google/code',{
         code : args[0]
       })
       response = aux.data
       // 0 - por fazer // 1 - completa  // 2 - cancelada
       store.set('GOOGLE_API_KEY',true);

       if(store.has('JWT_TOKEN')){
         var token = store.get('JWT_TOKEN');
         var resp2;
         try{
           resp2 = await axios.get('https://amcyni.herokuapp.com/google/credentials',{headers: {'x-access-token': token}});
           if (resp2.data.length === 0){
             resp2 = await axios.get('http://localhost:4545/api/googleToken');
             await axios.post('https://amcyni.herokuapp.com/api/google/token',{token: resp2.data.token},{headers: {'x-access-token': token}});
           }
         }
         catch(err){
           console.log(err);
         }
     }
    }
    catch(err) {
          console.error("Erro",err)
      }
    return response
  });

  ipcMain.handle('complete_todo_id', async (event, ...id) => {

    var response=false
    try {
       response = await axios.put('http://localhost:4545/api/state/'+id+'?state=1')
       // 0 - por fazer // 1 - completa  // 2 - cancelada
       response=true;

       if(store.has('JWT_TOKEN')){
         await sync();
       }
    }
    catch(err) {
      console.error("Erro",err)
      }

    return response;

  });

  ipcMain.handle('cancel_todo_id', async (event, ...id) => {

    var response =false
    try {
        await axios.put('http://localhost:4545/api/state/'+id+'?state=2')
       // 0 - por fazer // 1 - completa  // 2 - cancelada
       response =true

       if(store.has('JWT_TOKEN')){
         await sync();
       }
    }
    catch(err) {
          console.error("Erro",err)
    }

    return response;

  });

  ipcMain.handle('update_list_index', async (event, ...todos) => {

    var response
    try {
        response = await axios.put('http://localhost:4545/api',{
          todos
        }) // 0 - por fazer // 1 - completa  // 2 - cancelada
    }

    catch(err) {
           return false
       }

     return true
  });

  ipcMain.handle('get_all_todos', async (event, ...args) => {

    let response = await axios.get('http://localhost:4545/api')
    response.data.forEach(element => {
          if(element.date)
              element.date= new Date(element.date)
        });

    return response.data

  });

  ipcMain.handle('get_git_todos', async (event, ...args) => {
    await axios.get('http://localhost:4545/github/issues')

    if(store.has('JWT_TOKEN')){
      await sync();
    }

    let response = await axios.get('http://localhost:4545/api')
    response.data.forEach(element => {
         if(element.date)
             element.date= new Date(element.date)
       });
    return response.data
  });

  ipcMain.handle('get_outlook_todos', async (event, ...args) => {

    await axios.get('http://localhost:4545/outlook/calendar')
    await axios.get('http://localhost:4545/outlook/emails')

    if(store.has('JWT_TOKEN')){
      await sync();
    }

   let response = await axios.get('http://localhost:4545/api')
   response.data.forEach(element => {
         if(element.date)
             element.date= new Date(element.date)
       });
     return response.data
  });


  ipcMain.handle('get_google_todos', async (event, ...args) => {

    await axios.get('http://localhost:4545/google/tasks')
    await axios.get('http://localhost:4545/google/calendar')
    await axios.get('http://localhost:4545/google/emails')

    if(store.has('JWT_TOKEN')){
      await sync();
    }
   let response = await axios.get('http://localhost:4545/api')
   response.data.forEach(element => {
         if(element.date)
             element.date= new Date(element.date)
       });
   return response.data
  });

  ipcMain.handle('add_todo', async (event, ...args) => {
    var response

    try {

          response = await axios.post('http://localhost:4545/api',{
          name : args[0].name,
          priority : args[0].priority,
          description : args[0].description,
          origin : "metodo"

          })

          if(store.has('JWT_TOKEN')){
            await sync();
          }
    }
    catch(err) {
           return null
       }
       return response.data;

  });

  ipcMain.handle('log-in', async (event, ...args) => {
    var response;
    var response2;

    try{
      response = await axios.post('https://amcyni.herokuapp.com/login',{
        email: args[0],
        pwd: args[1]
      })
    }
    catch(err){
      return err.response.status;
    }

    store.set('JWT_TOKEN',response.data.token);

    console.log(response.data);

    if (store.has('GOOGLE_API_KEY') && response.data.google.length === 0){
      try{
        response2 = await axios.get('http://localhost:4545/api/googleToken');
        await axios.post('https://amcyni.herokuapp.com/api/google/token',{token: response2.data.token},{headers: {'x-access-token': response.data.token}});
      }
      catch(err){
        console.log(err);
      }
    }

    if (store.has('OUTLOOK_API_KEY') && response.data.outlook.length === 0){
      try{
        response2 = await axios.get('http://localhost:4545/api/outlookToken');
        await axios.post('https://amcyni.herokuapp.com/api/outlook/token',{token: response2.data.token},{headers: {'x-access-token': response.data.token}});
      }
      catch(err){
        console.log(err);
      }
    }

    if (store.has('GITHUB_API_KEY') && response.data.github.length === 0){
      try{
        response2 = await axios.get('http://localhost:4545/api/githubToken');
        await axios.post('https://amcyni.herokuapp.com/api/github/token',{token: response2.data.token},{headers: {'x-access-token': response.data.token}});
      }
      catch(err){
        console.log(err);
      }
    }

    if (!store.has('GOOGLE_API_KEY') && response.data.google.length !== 0){
      try{
        response2 = await axios.post('http://localhost:4545/api/google/token',response.data.google[0]);
        store.set('GOOGLE_API_KEY',true);
      }
      catch(err){
        console.log(err);
      }
    }

    if (!store.has('OUTLOOK_API_KEY') && response.data.outlook.length !== 0){
      try{
        response2 = await axios.post('http://localhost:4545/api/outlook/token',response.data.outlook[0]);
        store.set('OUTLOOK_API_KEY',true);
      }
      catch(err){
        console.log(err);
      }
    }

    if (!store.has('GITHUB_API_KEY') && response.data.github.length !== 0){
      try{
        response2 = await axios.post('http://localhost:4545/api/github/token',response.data.github[0]);
        store.set('GITHUB_API_KEY',true);
      }
      catch(err){
        console.log(err);
      }
    }

    var res = await sync();

    return response.status;
  });
  ipcMain.handle('register', async (event, ...args) => {
    var response;

    try{
      response = await axios.post('https://amcyni.herokuapp.com/register',{
        email: args[0],
        pwd: args[1]
      });
    }
    catch(err){
      return err.response.status;
    }

    return response.status;
  });

  ipcMain.handle('sync', async (event,...args)=>{

    if(!store.has('JWT_TOKEN')) {
      return null;
    }

    return await sync();
  })
}
