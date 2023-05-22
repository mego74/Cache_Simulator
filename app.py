from flask import Flask, render_template, request, redirect, url_for, make_response
import subprocess
import logging
import json
import html



logging.basicConfig(filename='app.log', level=logging.INFO)


app = Flask(__name__)


def display_file():
    summary1 = []
    summary2 = []
    summary3 = []
    summary4 = []
    Line = ''
    zipped_summary = []
    bitsAndTags = []
    index = 0
    with open('output.txt', 'r') as f:
        for line in f:
            if line.startswith('Valid bits and tag'):
                    continue
            elif line[0].isdigit():
                    bitsAndTags.append(line.strip())
            elif line.startswith('Total'):
                 Line += line.split(':')[1].strip()
                 index = index + 1  
            elif line.startswith('Hit ratio'):
                 Line += ' '
                 Line += line.split(':')[1].strip()
                 index = index + 1  
            elif line.startswith('Miss ratio'):
                 Line += ' '
                 Line += line.split(':')[1].strip()  
                 index = index + 1         
            elif line.startswith('AMAT'):
                 Line += ' '
                 Line += line.split(':')[1].strip() 
                 index = index + 1 
            if Line != '' and index == 4: 
                zipped_summary.append(Line)
                index = 0
                Line = ''
    return render_template('output_table_simple.html', zipped_summary = zipped_summary , bits = bitsAndTags)

@app.route('/')
def index():
    # each time the home page is visited or refreshed all these files get deleted.
    files = ['access_sequence.txt']
    for file in files:
        with open(file, 'w') as f:
            f.truncate(0)
    return render_template('index.html')

@app.route('/submit', methods=['POST']) # to submit data to server
def submit():
    # form: firstAddress >> input, , mem_init >> c, mem_text, code_txt, format >> outputForm, mem_option
    cacheSize = request.form['cacheSize']
    lineSize = request.form['lineSize']
    clockCycles = request.form['clockCycles']
    addressing =request.form['addressing']
        
    with open('input.txt', 'w') as f:
        f.write(cacheSize) # input
        f.write(" ")
        f.write(lineSize) # c
        f.write(" ")
        f.write(clockCycles) # outputForm
    with open('access_sequence.txt', 'w') as f:
        f.write(addressing)

    # Run the compiled program and wait for it to finish
    process = subprocess.Popen('./a')
    logging.info('Running C++ program')
    process.wait()
    logging.info("c++ program ran successfully")
    # check for errors:
    return redirect('/output')

@app.route('/output')
def output():
    return display_file()

if __name__ == '__main__':
    app.run(debug=True)
