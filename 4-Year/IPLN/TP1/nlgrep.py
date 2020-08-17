#! /usr/bin/env python3

import re
import sys
import fileinput
from getopt import getopt
from termcolor import colored


opts, resto = getopt(sys.argv[1:],"psie:")
dop = dict(opts)

sentence_delimiter = r'[\w,;:\'\"\-\s]+[.?!]+'
paragraph_delimiter = r'\n\n+'

flagP = False
flagS = False
flagI = False

regular_expression = r''

def split_lines(text):
    text_WO =  re.sub(r'\n','',text)
    lines = re.findall(sentence_delimiter,text_WO,flags=re.IGNORECASE)
    return lines

def split_paragraphs(text):
    lines = re.split(paragraph_delimiter,text)
    return lines    

def dataFromFile(filename):
    with open(filename, 'r') as fileToRead:
        data = fileToRead.read()

    return data

def readSTDIN():
    lines = []
    for line in sys.stdin:
        lines.append(line)
    
    return ''.join(lines)

def regEX_multipleWords(list_of_words):
    pattern = ''
    if len(list_of_words) > 0:
        nlist = map(lambda x: '(' + x + ')',list_of_words)
        pattern = '|'.join(nlist)

    return pattern
        
def colorize(matched):
    return colored(matched.group(),'blue')

def match(text_to_process,filename,mulFlag):
    for line in text_to_process:
        matched = re.findall(regular_expression,line)
        list.sort(matched,key=lambda x: -len(x))

        if len(matched) > 0:
            if sys.stdout.isatty():
                src_str  = re.compile(regEX_multipleWords(matched), re.IGNORECASE)
                line  = src_str.sub(colorize, line)

            if mulFlag:
                if sys.stdout.isatty():
                    print(colored(f'File {filename}: ','cyan') + line)
                else:
                    print(f'File {filename}: {line}')
            else:
                print(line)

def process_file(list_of_files):
    multiple_files = len(list_of_files) > 1

    for fileN in list_of_files:
        data = dataFromFile(fileN)
        
        if flagS:
            text_to_process = split_lines(data)
        else:
            text_to_process = split_paragraphs(data)
        
        match(text_to_process,fileN,multiple_files)

def stdinMode():
    data = readSTDIN()
    if flagS:
        text_to_process = split_lines(data)
    else:
        text_to_process = split_paragraphs(data)
    
    match(text_to_process,None,False)

def interactiveMode():
    for line in sys.stdin:
        text_to_process = split_lines(line)
        match(text_to_process,None,False)


def main():
    try:
        global regular_expression
        regular_expression = dop['-e']
        files = resto

        global flagP 
        global flagS
        global flagI

        flagP = '-p' in dop
        flagS = '-s' in dop
        flagI = '-i' in dop

        if (flagP and flagS):
            raise Exception('Error: Both -p and -s flags have been used')

        if (not flagP and not flagS):
            flagS = True

        if len(files) == 0:
            if flagI:
                interactiveMode()
            else:
                stdinMode()

        else: 
            process_file(files)

    except Exception as e:
        print(str(e))



if __name__ == "__main__":
    main()