var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
var mongoose = require('mongoose');
const passport = require('passport');
const keys = require('./config/keys');
const cookieSession = require('cookie-session');

var Register = require('./controllers/register')

require('dotenv').config({path : __dirname+'/.env'});

/****************************
 * MONGO CONNECTION
 ****************************/
const DATABASE_NAME = 'Access';

mongoose.connect('mongodb://127.0.0.1:27017/' + DATABASE_NAME, { useNewUrlParser: true, useUnifiedTopology: true })
  .then(() =>{
    console.log(`Connected to Mongo at [${DATABASE_NAME}] database...`)
    process.send('READY');
  }) 
  .catch((erro) => console.log(`Mongo: Error connecting to [${DATABASE_NAME}]: ${erro}`))

Register.exists()
.then(resp => {
  if (resp){
    console.log("ja existe")
  }
  else {
    Register.insert({local:0,global:0})
    .then(resp => console.log("criou collection nova"))
    .catch(err => console.log(err))
  }
})


var indexRouter = require('./routes/google');
var githubRouter = require('./routes/github');
var outlookROuter = require('./routes/outlook');
var authorizeRouter = require('./routes/authorize');
var apiRouter = require('./routes/api');

var app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use(passport.initialize());
app.use(passport.session());


app.use('/github', githubRouter);
app.use('/google', indexRouter);
app.use('/outlook',outlookROuter)
app.use('/authorize',authorizeRouter)
app.use('/api',apiRouter)

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

module.exports = app;
