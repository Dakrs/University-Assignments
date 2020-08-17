#! /usr/bin/env python3

from jinja2 import Template
from users import User
from getopt import getopt
import sys
import fileinput

opts, resto = getopt(sys.argv[1:],"psie:")
dop = dict(opts)


texto = ""

for line in fileinput.input(resto):
        texto += line

users = []

users.append(User(1,'Banshee','F'))
users.append(User(2,'Addams','F'))
users.append(User(3,'Golias','M'))

columns = ['Id','Nome','Sexo']

tmp = Template(texto)

ntext = tmp.render({'users':users,'columns': columns})

print(ntext)
