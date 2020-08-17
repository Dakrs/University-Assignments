var Dblock = require('../models/dblock')

module.exports.get = id => {
    return Dblock
        .find({id:id})
        .exec()
}



module.exports.updateState = (id,state) => {
    return Dblock
          .updateOne({id : id},
                {$set :{state : state}})
}


