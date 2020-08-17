This is a python implementation of the Unix grep command.

Usage:

        python3 mygrep.py [-p] [-s] [-i] -e pattern [FILES]
        
        Find matches of a pattern in lines/paragraphs of file(s).

        pattern                 the pattern to find
        FILES                   the file(s) to search

        optional arguments:
        -p                 filter pattern by paragraph
        -s                 filter pattern by sentence
        -i                 program reads from standard input 
