var Register = require('../models/register')


module.exports.get = () =>{
    return Register
           .find()
           .exec()
}

module.exports.updateById = (id,local,global) => {
    return Register
        .updateOne({_id : id},
        {$set : {local : local, global:global}
        })
}

module.exports.exists = () => {
    return Register.exists()
}

module.exports.insert = register => {
    var novo = new Register(register)
    return novo.save()
}


module.exports.incLocal = (_id) => {
    return Register
        .updateOne({_id : _id},
        {$inc : {local :1}})
}

module.exports.setLocal = (_id,val) => {
  return Register.updateOne({_id : _id},{local: val});
}



module.exports.incGlobal = (_id) => {
    return Register
        .updateOne({_id : _id},
        {$inc : {global :1}})
}
