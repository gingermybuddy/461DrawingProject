#Main server file, 
#Ryan Crow
#Justin Moore
#Jason Thompson

from flask import Flask, jsonify
from flask import request
import json

shapes = []

app = Flask(__name__)

def fullUpdate():
    return jsonify(shapes)

def getJsonFile():
    jsonFile = request.get_json()
    data = json.loads(jsonFile)

#Creates JSON-formatted circle data
def addCircle(r,x,y):
    circle = jsonify({'shape' : 'circle', 'data' : {'radius' : r, 'x' : x, 'y' : y}})
    shapes += circle
    return circle

#Creates JSON-formatted line data
def addLine(x1,y1,x2,y2):
    line = jsonify({'shape' : 'line', 'data' : {'start' : {'x' : x1, 'y' : y1}, 'end' : {'x' : x2, 'y' : y2}}})
    shapes += line
    return line

#Creates JSON-formatted rectangle data
def addRect (x,y,w,h):
    rectangle = jsonify({'shape' : 'rect', 'data' : {'x' : x, 'y' : y, 'w' : w, 'h' : h}})
    shapes += rectangle
    return rectangle

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

if __name__ == '__main__':
    app.run(debug=True)
