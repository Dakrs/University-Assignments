#! /usr/bin/env python3

import jinja2
from users import User
from getopt import getopt
import sys
import fileinput

opts, resto = getopt(sys.argv[1:],"psie:")
dop = dict(opts)

users = []

users.append(User(1,'Banshee','F'))
users.append(User(2,'Addams','F'))
users.append(User(3,'Golias','M'))

columns = ['Id','Nome','Sexo']

env = jinja2.Environment(
    loader=jinja2.FileSystemLoader(searchpath="./extend/"),
    autoescape=jinja2.select_autoescape(['html', 'xml'])
)

template = env.get_template('child.html')


ntext = template.render({'users':users,'columns': columns})


print(ntext)