var Transaction = require('../models/transactions')


module.exports.insert = t => {
    var novo = new Transaction(t)
    return novo.save()
}


module.exports.updateType = (id, type) => {
  return Transaction.updateOne({_id : id},{type: type});
}

module.exports.getTransactionsWithInterval = (local, global) => {
    return Transaction.find({ timestamp: { $gte: global, $lt: local } });

}
