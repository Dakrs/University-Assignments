#! /usr/bin/env python3

import nltk
from nltk.corpus import floresta
from collections import Counter
from getopt import getopt
import re
import sys

opts, resto = getopt(sys.argv[1:],'l')
dop = dict(opts)

sentence_delimiter = r'[\w,;;\'\"\-\s]+[\.?!]+'

def dataFromFile(filename):
    with open(filename,'r') as fileToRead:
        data = fileToRead.read()
    
    text = re.sub(r'[ \n]+','',data)
    return text


def memo(f):
    cache = {}
    def fmemo(*args):
        if args not in cache:
            cache[args] = f(*args)
        return cache[args]
    fmemo.cache = cache
    return fmemo

def pdist(counter):
    N = sum(counter.values())
    return lambda x: counter[x]/N

def biGramms():
    dic = Counter()

    words = floresta.words()
    tam = len(words)
    for i in range(0,tam-1):
        bigram = f'%s %s' % (words[i].lower(),words[i+1].lower())
        dic.update({bigram: 1})
    return dic

BIGRAMM_COUNTER = biGramms()
WORDS_COUNTER = Counter(w.lower() for w in floresta.words())
WORD_PROB = pdist(WORDS_COUNTER) #função para calcular a ocurrência de uma palavra
BIGRAMM_PROB = pdist(BIGRAMM_COUNTER) #função para calcular a ocurrência de um bigrama

def cPwordDouble(word, prev):
    bigram = prev + ' ' + word
    if BIGRAMM_PROB(bigram) > 0 and WORD_PROB(prev) > 0:
        return BIGRAMM_PROB(bigram) / WORD_PROB(prev)
    else:
        return WORD_PROB(word) / 2

def cPwordSingle(word):
    return WORD_PROB(word) / 2

def Bigramm_prob(words):
    probs_list = list(map(lambda x: cPwordSingle(x[1]) if (x[0] == 0) else cPwordDouble(x[1],words[x[0] - 1]),enumerate(words)))
    return cal_probability(probs_list)

def ProbSequence(words):
    probs_list = list(map(lambda x: WORD_PROB(x),words))
    return cal_probability(probs_list)

def cal_probability(nums):
    result = 1
    for x in nums:
        result *= x
    return result

def breakText(text):
    res = []
    max = min(len(text), 20)+1
    for index in range(1, max):
        res.append((text[:index], text[index:]))
    return res


@memo
def possibilities(text):
    if not text: 
        return []
    else:
        candidates = []
        for (first, rest) in breakText(text):
            laux = [first] + possibilities(rest)
            candidates.append(laux)
        return max(candidates,key=Bigramm_prob)

def upper_repl(match):
    return match.group(1).upper()

def normalize(text):
    sentences = re.findall(sentence_delimiter,text)
    sentences_with_capital_letter = [re.sub(r'^ ?(\w)',upper_repl,x) for x in sentences]
    final_sentences = [re.sub(r' ([,\.;:!?])',r'\1',x) for x in sentences_with_capital_letter]
    if '-l' in dop:
        return '\n'.join(final_sentences)
    return ' '.join(final_sentences)

def calculate_text(text):
    list_sentences = re.findall(sentence_delimiter,text)
    list_best_possibilities = [' '.join(possibilities(x)) for x in list_sentences]
    text_in_words = ' '.join(list_best_possibilities)

    return text_in_words

def main():
    if len(resto) == 0:
        raise Exception('Nenhum ficheiro passad no input')

    for file in resto:
        text = dataFromFile(file)
        text_in_words = calculate_text(text)
        final_text = normalize(text_in_words)
        print('Filename: ' + file)
        print(final_text)


if __name__ == "__main__":
    main()




