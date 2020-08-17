import nltk
from nltk.corpus import words
from nltk.corpus import floresta

#Exemplo muuuito basico usampo corpus portugues "floresta" do nltk
string = "eucomomuito"
tokens = []
lowercaseCorpus = [x.lower() for x in floresta.words()]
i = 0
while i < len(string):
    maxWord = ""
    for j in range(i, len(string)):
        tempWord = string[i:j+1]
        if tempWord in lowercaseCorpus and len(tempWord) > len(maxWord):
            maxWord = tempWord
    i = i+len(maxWord)
    tokens.append(maxWord)

print(tokens)