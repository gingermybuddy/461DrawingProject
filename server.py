from flask import Flask
from flask import request
import json

app = Flask(__name__)

@app.route('/', methods = ['POST', 'GET'])
def getJsonFile():
    jsonFile = request.get_json()
    data = json.loads(jsonFile)

if __name__ == '__main__':
    app.run(debug=True)