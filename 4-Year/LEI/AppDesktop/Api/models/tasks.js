var mongoose = require('mongoose');


var taskSchema = new mongoose.Schema({
    _id: String,
    idOrigin : String,
    date : Date,
    name : String,
    description : String,
    priority : String,
    origin : String,
    owner : String,
    state : Number, 
    index :Number
  });
    
  module.exports = mongoose.model('tasks', taskSchema)