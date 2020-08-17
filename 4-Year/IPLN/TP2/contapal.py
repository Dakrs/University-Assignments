import os
import requests
import operator
import re
import nltk
from bs4 import BeautifulSoup
from collections import Counter
from nltk.corpus import stopwords
from string import punctuation
from flask import Flask, render_template, request, url_for

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def index():
    errors = []
    results = {}
    if request.method == "POST":
        try:
            url = request.form['url']
            r = requests.get(url)
        except:
            errors.append(
                "URL inválido."
            )
            return render_template('index.html', errors=errors)
        if r:
            content = BeautifulSoup(r.text, 'html.parser')
            while content.script != None:
                content.script.extract()
            while content.style != None:
                content.style.extract()
            while content.meta != None:
                content.meta.extract()
            while content.noscript != None:
                content.noscript.extract()
            text = content.get_text()

            tokens = nltk.word_tokenize(text.lower())
            text = nltk.Text(tokens)
            
            regex = re.compile(r'\w+')
            raw_words = [w for w in text if regex.match(w)] #filtrar lixo que não seja palavras
            # raw_word_count = Counter(raw_words) -> contagem normal
            
            stops = set(nltk.corpus.stopwords.words('portuguese') + nltk.corpus.stopwords.words('english') + list(punctuation))
            no_stop_words = [w for w in raw_words if w.lower() not in stops]
            results = Counter(no_stop_words) # -> contagem sem 'stops'


    return render_template('index.html', errors=errors, results=results)




if __name__ == '__main__':
    app.run(debug=True)