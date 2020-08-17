const router = require('express').Router();
const _ = require('underscore');
const GitHub = require('github-api');
var nanoid = require('nanoid');
const jwt = require('jsonwebtoken');
const keys = require('../config/keys');
var Task = require('../controllers/tasks');
var Register = require('../controllers/register')
const Credential = require('../controllers/credentials');
var Transaction = require('../controllers/transactions')
let promise = Promise.resolve();


const oauth2 = require('simple-oauth2').create({
  client: {
    id: keys.github.clientID,
    secret: keys.github.clientSecret
  },
  auth: {
    authorizeHost: 'https://github.com',
    authorizePath: '/login/oauth/authorize',
    tokenHost: 'https://github.com',
    tokenPath: '/login/oauth/access_token'
  }
});

function getAuthUrl() {
  const authorizationUri = oauth2.authorizationCode.authorizeURL({
    redirect_uri: 'http://localhost:4545/github/login/return',
    scope: keys.github.scope
  });
  return authorizationUri;
}

router.get('/url',async function(req,res){
  var url = await getAuthUrl()
  res.jsonp(url)
});

router.get('/verify', function(req,res){
  Credential.get("GITHUB")
  .then(dados =>{
      if(dados.length>0)
    res.jsonp(true)
      else
      res.jsonp(false)
  })
  .catch(erro => res.jsonp(false))

})

// callback route for github to redirect to
// hand control to passport to use code to grab profile info
router.get('/login/return', async function(req, res, next) {
    // Get auth code
  const code = req.query.code;

  // If code is present, use it
  if (code) {
    try {
      await getTokenFromCode(code);
      // Redirect to home
      res.jsonp("Close the window")
    } catch (err) {
      res.jsonp(false)
    }
  } else {
    // Otherwise complain
    res.jsonp(false)
  }
});

router.get('/issues', async function(req, res, next) {
    task = {}

    const accessToken = await getAccessToken();

    if(accessToken){
      let gh = new GitHub({
        token: accessToken
      });

      let me = await gh.getUser();
      const requestProfile = me.getProfile();
      requestProfile.then(function(val){
        return val.data.login;
      });
      let user = await requestProfile;
      const username = user.data.login;

      try {
        const headers = {
          "Authorization" : "Token " + accessToken
        }
        const url = "https://api.github.com/user/repos";/*?q=author:" + username + " type:issue";*/
        const response = await fetch(url, {
          "method": "GET",
          "headers": headers
        });

        const result =  await response.json();
        var issueRes = [];
        var issuesTotal = new Array(result.length);

        Promise.all(result.map(async (repo,index) => {
          title = repo.name;
          const headers = {
              "Authorization" : "Token " + accessToken
          }
          const url2 = "https://api.github.com/repos/" + username + "/" + title +  "/issues";/*?q=author:" + username + " type:issue";*/

          const issues = await fetch(url2, {
                "method": "GET",
                "headers": headers
          })
          issueRes = await issues.json();

          if(issueRes.length > 0){
            issuesTotal[index] = issueRes;
          }
          else{
            issuesTotal[index] = null;
          }
        })).then(async () => {

                  for(var i = 0; i < issuesTotal.length; i++){
                    var middle = issuesTotal[i];
                    if (middle === null)
                      continue;
                    console.log('Entrei');

                    for(var k = 0; k < middle.length; k++){
                      var resTask = await addTasksV2(middle[k]);
                      console.log(resTask);
                    }
                  }

                  res.jsonp(true);
        })

      } catch (error) {
        console.log(error);
        githubData = { error: error }
      }
    } else {
      res.redirect(getAuthUrl());
    }

});

/*
router.get('/issues', async function(req, res, next) {
    task = {}

    const accessToken = await getAccessToken();

    if(accessToken){
      let gh = new GitHub({
        token: accessToken
      });

      let me = await gh.getUser();
      const requestProfile = me.getProfile();
      requestProfile.then(function(val){
        return val.data.login;
      });
      let user = await requestProfile;
      const username = user.data.login;

      try {
        const headers = {
          "Authorization" : "Token " + accessToken
        }
        const url = "https://api.github.com/user/repos";
        const response = await fetch(url, {
          "method": "GET",
          "headers": headers
        });

        const result =  await response.json();
        var issueRes = [];

        for(var k = 0; k < result.length; k++){
          title = result[k].name;
          const headers = {
              "Authorization" : "Token " + accessToken
          }
          const url2 = "https://api.github.com/repos/" + username + "/" + title +  "/issues";

          const issues = await fetch(url2, {
                "method": "GET",
                "headers": headers
          })
          issueRes = await issues.json();
          if(issueRes.length > 0){
            for(var i = 0; i < issueRes.length; i++){
              var data = await addTasksV2(issueRes[i]);
              console.log(data);
            }
          }
        }
        res.jsonp(false);
      }
      catch (error) {
          console.log(error);
          githubData = { error: error }
        }
      }
      else {
        res.redirect(getAuthUrl());
      }

});*/

// auth logout
router.get('/logoff', function(req, res, next) {
  clearCookies(res);
  // Redirect to home
  res.redirect('/github/login/return');
});

async function getTokenFromCode(auth_code) {
  var creden = {}
  var tokenAux = {}
  let result = await oauth2.authorizationCode.getToken({
    code: auth_code,
    redirect_uri: 'http://localhost:4545/github/login/return',
    scope: keys.github.scope
  });

  const token = oauth2.accessToken.create(result);

  creden.type = "GITHUB"
  creden.owner ="me"
  tokenAux.access_token = token.token.access_token
  tokenAux.scope = token.token.scope
  tokenAux.token_type=token.token.token_type
  creden.token = tokenAux

  await Credential.insert(creden);

  return token.token.access_token;
}

async function getAccessToken() {
  // Do we have an access token cached?
  var token = await Credential.get("GITHUB");

  return token[0].token.access_token;

}


async function addTasksV2(r){
  var response = await Task.findByIdOrigin(r.id,"GITHUB");
  if (response.length===0){
    var task = {}
    task._id = nanoid()
    task.idOrigin = r.id
    task.name = r.title
    task.description = r.body
    task.origin = "GITHUB"
    task.owner = "me"
    task.state = 0

    var aux = await Task.insert(task)
    var register = await Register.get()
    await createTransaction(aux,"Post",register[0].local)
    await Register.incLocal(register[0]._id)

    return aux;
  }
  return null;
}

function addTasks(r){
  return new Promise (async (resolve,reject) => {
    var response = await Task.findByIdOrigin(r.id,"GITHUB")
    if (response.length===0){
      var task = {}
          task._id = nanoid()
          task.idOrigin = r.id
          task.name = r.title
          task.description = r.body
          task.origin = "GITHUB"
          task.owner = "me"
          task.state = 0

          var aux = await Task.insert(task)
          var register = await Register.get()
          await createTransaction(aux,"Post",register[0].local)
          await Register.incLocal(register[0]._id)

          return resolve(aux)
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
