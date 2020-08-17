var mongoose= require('mongoose')


var dblockSchema = {
    id: {type: String,required:true},
    state:{type:Boolean, required:true}
}

module.exports = mongoose.model('dblock', dblockSchema)