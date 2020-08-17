const todo = new RegExp('^(\\[TODO\\])','i')


exports.todoRegex = title => {
    return todo.test(title)
}
