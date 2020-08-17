var express = require('express');
var router = express.Router();
var authHelper = require('../authOutlook');
var graph = require('@microsoft/microsoft-graph-client');
require('isomorphic-fetch');
var nanoid = require('nanoid')
var Task = require('../controllers/tasks')
var Credential = require('../controllers/credentials')
var Register = require('../controllers/register')
var Transaction = require('../controllers/transactions')
var Utility = require('../utility')
let promise = Promise.resolve(); 





router.get('/url', async function(req,res){
  var url = authHelper.getAuthUrl()
  res.jsonp(url)

})

router.get('/verify', function(req,res){
  Credential.get("OUTLOOK")
  .then(dados =>{
      if(dados.length>0)
    res.jsonp(true)
      else
      res.jsonp(false)
  })
  .catch(erro => res.jsonp(erro))

})



router.get('/emails', async function(req, res, next) {

  const accessToken = await authHelper.getAccessToken();
  if (accessToken) {
    // Initialize Graph client
    const client = graph.Client.init({
      authProvider: (done) => {
          done(null, accessToken);
        }
    });

      // Get the 10 newest messages from inbox
      const result = await client
        .api('/me/mailfolders/inbox/messages')
        .top(1000000000)
        .orderby('receivedDateTime DESC')
        .get();
    
      var emails =  result.value;
      emails.forEach(element => {
        promise = promise.then(() => {
          return addTaskEmails(element)
        })
      })
      promise.then(data => {
        console.log(data)
      })
      res.jsonp(result.value)  
  } 
  else {
    res.redirect(authHelper.getAuthUrl())
  }

})


router.get('/calendar',async function(req, res, next){
  const accessToken = await authHelper.getAccessToken();
  console.log(accessToken)
  if (accessToken) {
    // Initialize Graph client
    const client = graph.Client.init({
      authProvider: (done) => {
        done(null, accessToken);
      }
    });

    // Set start of the calendar view to today at midnight
    const start = new Date(new Date().setHours(0,0,0));
    // Set end of the calendar view to 365 days from start
    const end = new Date(new Date(start).setDate(start.getDate() + 365));
    
      // Get the first 10 events for the coming week
    const result = await client
      .api(`/me/calendarView?startDateTime=${start.toISOString()}&endDateTime=${end.toISOString()}`)
      .get();

    var events =  result.value;

    events.forEach( async element => {

      promise= promise.then(() => {
            return addTasksCalendar(element)
      })
    })
     
    promise.then(data => {
        console.log(data)
    })
      
    res.jsonp(result.value)
      
  } else {
    // Redirect to home)
  res.redirect(authHelper.getAuthUrl())
  }
  
})


function addTaskEmails(element){
  return new Promise (async (resolve,reject)=> {
  var bool = Utility.todoRegex(element.subject)
  if(bool){
      var response = await Task.findByIdOrigin(element.id,"Outlook emails")
     
      if(response.length===0){
        var task = {}
        task._id = nanoid()
        task.idOrigin = element.id
        task.name = element.subject
        task.priority = 3
        task.origin = "Outlook emails"
        task.owner = "me"
        task.state = 0
    
        var aux = await Task.insert(task)
        var register = await Register.get() 

        await createTransaction(aux,"Post",register[0].local)
        await Register.incLocal(register[0]._id)
        resolve(aux);  
      }
      else
        resolve(false)  
  }
  else
    resolve(false)   

})
}

function addTasksCalendar(element){
  return new Promise (async (resolve,reject)=> {
    var bool = Utility.todoRegex(element.subject)
    if (bool){
      var response = await Task.findByIdOrigin(element.id,"Outlook Calendar")
      if(response.length===0){
        var task = {}
          task._id = nanoid()
          task.idOrigin = element.id
          task.date = element.start.dateTime
          task.name = element.subject
          task.description = element.bodyPreview
          task.priority = 3
          task.origin = "Outlook Calendar"
          task.owner = "me"
          task.state = 0
                
          var aux = await Task.insert(task)
          var register = await Register.get()
          await createTransaction(aux,"Post",register[0].local)
          await Register.incLocal(register[0]._id)
          resolve(aux);  
      }
      else
          resolve(false)  
    }
    else
      resolve(false)   
        
  })
  

}

async function createTransaction(task, type,local){

  var transactions = JSON.parse(JSON.stringify(task)); //new json object here

  transactions.idTask = task._id
  transactions._id = nanoid()
  transactions.type = type;
  transactions.idOrigin = task.origin === 'metodo' ? task._id : task.idOrigin;
  transactions.timestamp = local
 await Transaction.insert(transactions)

}

module.exports = router;
