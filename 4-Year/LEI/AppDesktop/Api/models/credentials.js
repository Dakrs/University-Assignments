var mongoose = require('mongoose');


var tokenSchema = {
    access_token : String,
    refresh_token : String,
    scope : String,
    token_type : String,
    expiry_date : String,
}



var credentialSchema = new mongoose.Schema({
    type: {type :String , required :true},
    token : tokenSchema,
    owner : {type : String, required:true}
  });
    
  module.exports = mongoose.model('credentials', credentialSchema)