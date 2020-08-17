var mongoose= require('mongoose')


var transactionSchema = {
    _id: String,
    idTask:String,
    idOrigin : String,
    date : Date,
    name : String,
    description : String,
    priority : String,
    origin : String,
    owner : String,
    state : Number, 
    index :Number,
    timestamp: {type: Number,required:true},
    type: {type:String, required:true}
};

module.exports = mongoose.model('transaction', transactionSchema)