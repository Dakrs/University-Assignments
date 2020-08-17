var mongoose = require('mongoose');


var registerSchema = new mongoose.Schema({
    local: Number,
    global:Number
  });
    
  module.exports = mongoose.model('register', registerSchema)