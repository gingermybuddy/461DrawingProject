#Main server file, 
#Ryan Crow
#Justin Moore
#Jason Thompson

from flask import Flask, jsonify
from flask import request
import json

app = Flask(__name__)

def getJsonFile():
    jsonFile = request.get_json()
    data = json.loads(jsonFile)


#Creates JSON-formatted circle data
def addCircle(r,x,y):
    return jsonify({'shape' : 'circle', 'data' : {'radius' : r, 'x' : x, 'y' : y}})

#Creates JSON-formatted line data
def addLine(x1,y1,x2,y2):
    return jsonify({'shape' : 'line', 'data' : {'start' : {'x' : x1, 'y' : y1}, 'end' : {'x' : x2, 'y' : y2}}})

#Creates JSON-formatted rectangle data
def addRect (x,y,w,h):
    return jsonify({'shape' : 'rect', 'data' : {'x' : x, 'y' : y, 'w' : w, 'h' : h}})

#Checks shape type received and calls matching function
@app.route('/', methods = ['POST', 'GET'])
def shapeType():
    shape = request.args.get('shape') #Grabs shape parameter and determines which shape it is; returns an error JSON if improper shape name
    if shape == 'circle':
        r = request.args.get('radius')
        x = request.args.get('x')
        y = request.args.get('y')
        return addCircle(r,x,y)
    elif shape == 'line':
        x1 = request.args.get('x1')
        y1 = request.args.get('y1')
        x2 = request.args.get('x2')
        y2 = request.args.get('y2')
        return addLine(x1,y1,x2,y2)
    elif shape == 'rect':
        x = request.args.get('x')
        y = request.args.get('y')
        w = request.args.get('width')
        h = request.args.get('height')
        return addRect(x,y,w,h)
    else:
        return jsonify({'error' : 'Bad Shape Type'})

# A implementation of saveFile which allows the user to save the board that they are currently using.
# User will send this url to the server and server will reply back with an SVG file
@app.route('/saveFile', methods = ['POST', 'GET'])
def saveFile():
    barf = 5

if __name__ == '__main__':
    app.run(debug=True)
