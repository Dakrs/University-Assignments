var Task = require('../models/tasks')


module.exports.insert = task => {
    var novo = new Task(task)
    return novo.save()
}


module.exports.selectAll = () =>{
    return Task
           .find({state :0})
           .exec() 
}

module.exports.selectAllByOwner = owner => {
    return Task
           .find({owner : owner})
           .exec() 
}




module.exports.selectById = id =>{
    return Task
           .findOne({_id : id})
           .exec()

}

module.exports.findByIdOrigin = (idFont, font) =>{
    return Task
           .find({idOrigin:idFont,  origin : font })
           .exec()
}


module.exports.updateById = task => {
    return Task
           .updateOne({_id :task._id},task,{new:true})
           .exec()        
}

module.exports.updateState = (idTask,state) => {
    return Task
    .updateOne({_id : idTask},
        {$set : {state : state}})
}

module.exports.historic = () => {
    return Task
           .find({state : 1 })
           .exec()    
}

module.exports.sortByOrigin = () =>{
    return Task
           .find()
           .sort({origin : 1})
           .exec()
}

module.exports.sortByTime = () =>{
    return Task
           .find()
           .sort({date : 1})
           .exec()
}


module.exports.removeById = id =>{
    return Task
           .deleteOne({_id:id})
}


