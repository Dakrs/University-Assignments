var Credential = require('../models/credentials')

module.exports.get = type => {
    return Credential
        .find({type:type})
        .exec()
}
module.exports.insert = t => {
    var novo = new Credential(t)
    return novo.save()
}

module.exports.update = (type,token) =>{
    return Credential
        .updateOne({type : type},
            {$set :{token : token}})
        }

      