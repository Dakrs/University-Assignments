var express = require('express');
var router = express.Router();
var Task = require('../controllers/tasks')
var Transaction = require('../controllers/transactions')
var Register = require('../controllers/register')
var Credential = require('../controllers/credentials')
var nanoid = require('nanoid')

router.get('/',function(req, res, next) {
    Task.selectAll()
    .then(dados =>res.jsonp(dados))
    .catch(erro => res.status(500).jsonp(erro))
})

router.get('/owner/:owner',function(req, res, next) {
    Task.selectAllByOwner(req.params.owner)
    .then(dados =>res.jsonp(dados))
    .catch(erro => res.status(500).jsonp(erro))
})

router.get('/origin',function(req, res, next) {
    Task.sortByOrigin()
    .then(dados =>res.jsonp(dados))
    .catch(erro => res.status(500).jsonp(erro))
})
router.get('/date',function(req, res, next) {
    Task.sortByTime()
    .then(dados =>res.jsonp(dados))
    .catch(erro => res.status(500).jsonp(erro))
})


router.get('/historic',function(req, res, next) {
    Task.historic()
    .then(dados =>res.jsonp(dados))
    .catch(erro => res.status(500).jsonp(erro))
})

router.get('/register',function(req,res){
    Register.get()
    .then(dados => res.jsonp(dados))
    .catch(erro => res.status(500).jsonp(erro))
})

router.get('/transactions',function(req,res){
    Register.get()
    .then(register => {
            var local = register[0].local
            var global = register[0].global
            Transaction.getTransactionsWithInterval(local,global)
            .then(transactions => res.jsonp(transactions))
            .catch(erro => {
                console.log(erro)
                res.status(500).jsonp(erro)})
        })
    .catch(erro =>{
            console.log(erro)
             res.status(500).jsonp(erro)})

})


router.get('/googleToken',(req,res) =>{
  Credential.get("GOOGLE")
  .then(token => {
    if(token.length> 0){
      res.jsonp(token[0])
    }
    else{
    res.status(500).jsonp(false)
    }
  })
  .catch(erro => res.status(500).jsonp(erro))
})



router.get('/outlookToken',(req,res) =>{
  Credential.get("OUTLOOK")
  .then(token => {
    if(token.length> 0){
      res.jsonp(token[0])
    }
    else{
    res.status(500).jsonp(false)
    }
  })
  .catch(erro => res.status(500).jsonp(erro))
})


router.get('/githubToken',(req,res) =>{
  Credential.get("GITHUB")
  .then(token => {
    if(token.length> 0){
      res.jsonp(token[0])
    }
    else{
    res.status(500).jsonp(false)
    }
  })
  .catch(erro => res.status(500).jsonp(erro))
})




router.get('/:id',function(req, res, next) {
    Task.selectById(req.params.id)
    .then(dados =>res.jsonp(dados))
    .catch(erro => res.status(500).jsonp(erro))
})



router.put('/',function(req, res, next) {
    var todos = req.body.todos[0]
    todos.forEach(element => {
            Task.updateById(element)
    });
    res.jsonp(todos)


})

router.put('/state/:id',function(req, res, next) {
    var state = req.query.state
    Task.updateState(req.params.id,state)
    .then(dados =>{
        Task.selectById(req.params.id)
        .then(dados2 => {
            Register.get()
            .then(register=> {
            Register.incLocal(register[0]._id)
            .then(incLOCAL =>{
                console.log(register)
                var transactions = JSON.parse(JSON.stringify(dados2)); //new json object here
                transactions.idTask = dados2._id;
                transactions.idOrigin = dados2.origin === 'metodo' ? dados2._id : dados2.idOrigin;
                transactions._id = nanoid()
                transactions.type = req.query.state==="1" ? "confirm" : "cancel";
                transactions.timestamp = register[0].local
                console.log(transactions)
                Transaction.insert(transactions)
                .then(resp1 =>  res.jsonp(dados) )
                .catch(err => console.log(err))

            })
            .catch(err => console.log(err))

            })
        })
        .catch(err => console.log(err))

    })
    .catch(erro => res.status(500).jsonp(erro))
})


router.put('/register/:id',function(req,res,nex){
    var local = req.body.local
    var global=req.body.global
    var id = req.params.id
    Register.updateById(id,local,global)
    .then(dados =>res.jsonp(dados))
    .catch(erro =>{
        console.log(erro)
        res.status(500).jsonp(erro)})
})


router.delete('/:id',function(req, res, next) {
    Task.removeById(req.params.id)
    .then(dados =>res.jsonp(dados))
    .catch(erro => res.status(500).jsonp(erro))
})

router.post('/',function(req,res){
    req.body._id = nanoid();
    req.body.owner = "me"
    req.body.state =0
    Task.insert(req.body)
    .then(dados =>{
        Register.get()
            .then(register=> {
            Register.incLocal(register[0]._id)
            .then(incLOCAL =>{
                console.log(register)
                var transactions = JSON.parse(JSON.stringify(dados)); //new json object here
                transactions.idTask = dados._id
                transactions.idOrigin = dados._id
                transactions._id = nanoid()
                transactions.type = "Post";
                transactions.timestamp = register[0].local
                console.log(transactions)
                Transaction.insert(transactions)
                .then(resp1 =>  res.jsonp(dados) )
                .catch(err => console.log(err))

            })
            .catch(err => console.log(err))

            })


    })
    .catch(erro => res.status(500).jsonp(erro))
})

router.put('/updatetransaction',async function(req,res){
  var id = req.body.id;
  var type = req.body.dep.type;

  try{
    var result = await Transaction.updateType(id,type);
    switch (type) {
      case 'confirm':
        await Task.updateState(req.body.dep.idTask,1);
        break;
      case 'cancel':
        await Task.updateState(req.body.dep.idTask,2);
        break;
      default:
    }
    res.jsonp(type);
  }
  catch(err){
    res.status(500).jsonp(err);
  }
})

router.post('/transactionTotask', async function(req,res){
  var transaction = req.body;
  var dados;
  switch(transaction.type){
    case 'Post':
      dados = await transactionPost(transaction);
      break;
    case 'confirm':
      dados = await transactionConfirm(transaction);
      break;
    case 'cancel':
      dados = await transactionCancel(transaction);
      break;
  }

  try{
    res.jsonp(dados);
  }
  catch(err){
    res.status(500).jsonp(err);
  }
})

router.post('/github/token',(req,res) =>{
  var token = {}
  token.token = req.body.token
  token.owner = "me"
  token.type = "GITHUB"

  Credential.insert(token)
  .then(resp => res.jsonp(true))
  .catch(erro => {
    console.log(erro)
    res.status(500).jsonp(false)})

})


router.post('/google/token',(req,res) =>{
  var token = {}
  token.token = req.body.token
  token.owner = "me"
  token.type = "GOOGLE"

  Credential.insert(token)
  .then(resp => res.jsonp(true))
  .catch(erro => {
    console.log(erro)
    res.status(500).jsonp(false)})
})


router.post('/outlook/token',(req,res) =>{
  var token = {}
  token.token = req.body.token
  token.owner = "me"
  token.type = "OUTLOOK"
  console.log(req.body);

  Credential.insert(token)
  .then(resp => res.jsonp(true))
  .catch(erro => {
    console.log(erro)
    res.status(500).jsonp(false)})
})


async function transactionPost(ts){
  var transactionsCOPY = JSON.parse(JSON.stringify(ts)); //new json object here
  delete transactionsCOPY["type"];
  delete transactionsCOPY["timestamp"];
  transactionsCOPY._id = ts.idTask;
  transactionsCOPY.owner = "me";
  delete transactionsCOPY["idTask"];

  await Task.insert(transactionsCOPY);

  var register = await Register.get();
  var timestamp= register[0].local;

  ts.timestamp=timestamp;
  ts.owner= "me";

  await Transaction.insert(ts);
  await Register.incLocal(register[0]._id);

  return ts;
}

async function transactionConfirm(ts){
  await Task.updateState(ts.idTask,1);
  var register = await Register.get();
  var timestamp= register[0].local;

  ts.timestamp = timestamp;

  await Transaction.insert(ts);
  await Register.incLocal(register[0]._id);

  return ts;
}

async function transactionCancel(ts){
  await Task.updateState(ts.idTask,2);
  var register = await Register.get();
  var timestamp= register[0].local;

  ts.timestamp = timestamp;

  await Transaction.insert(ts);
  await Register.incLocal(register[0]._id);

  return ts;
}





module.exports = router;
