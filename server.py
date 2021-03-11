from flask import Flask
from flask import request
import json

app = Flask(__name__)

@app.route('/', methods = ['POST', 'GET'])
def getJsonFile():
    jsonFile = request.get_json()
    data = json.loads(jsonFile)

def addCircle(r,x,y): #Creates JSON-formatted circle data
    return jsonify({'shape' : 'circle', 'data' : {'radius' : r, 'x' : x, 'y' : y}})

def addLine(x1,y1,x2,y2): #Creates JSON-formatted line data
    return jsonify({'shape' : 'line', 'data' : {'start' : {'x' : x1, 'y' : y1}, 'end' : {'x' : x2, 'y' : y2}}})

def addRect (x,y,w,h): #Creates JSON-formatted rectangle data
    return jsonify({'shape' : 'rect', 'data' : {'x' : x, 'y' : y, 'w' : w, 'h' : h}})

def shapeType(): #Checks shape type received and calls matching function
    shape = request.args.get('shape')
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
