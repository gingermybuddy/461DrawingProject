#Main server file, 
#Ryan Crow
#Justin Moore
#Jason Thompson

from flask import Flask, jsonify
from flask import request
import json

index = 0
shapes = {}

app = Flask(__name__)

#!! NON-FUNCTIONAL !!

def getJsonFile():
    jsonFile = request.get_json()
    data = json.loads(jsonFile)

#Creates JSON-formatted circle data
def addCircle(r,x,y,index):
    circle = jsonify({'shape' : 'circle', 'data' : {'radius' : r, 'x' : x, 'y' : y}})
    circleJSON = "{'shape' : 'circle', 'data' : {'radius' : " + str(r) + ", 'x' : " + str(x) + "'y' : " + str(y) + "}}"
    shapes[str(index)] = circleJSON
    index += 1
    print(index)
    return circle

#Creates JSON-formatted line data
def addLine(x1,y1,x2,y2,index):
    line = jsonify({'shape' : 'line', 'data' : {'start' : {'x' : x1, 'y' : y1}, 'end' : {'x' : x2, 'y' : y2}}})
    lineJSON = "{'shape' : 'line', 'data' : {'start' : {'x' :" + str(x1) + ", 'y' : " +  str(y1) + "}, 'end' : {'x' : " + str(x2) + ", 'y' : " + str(y2) + "}}}"
    shapes[str(index)] = lineJSON
    index += 1
    print(index)
    return line

#Creates JSON-formatted rectangle data
def addRect(x,y,w,h,index):
    rectangle = jsonify({'shape' : 'rect', 'data' : {'x' : x, 'y' : y, 'w' : w, 'h' : h}})
    rectangleJSON = "{'shape' : 'rect', 'data' : {'x' : " + str(x) + ", 'y' : " + str(y) + ", 'w' : " + str(w) + ", 'h' : " + str(h) + "}}"
    shapes[str(index)] = rectangleJSON
    index += 1
    return rectangle


#Checks shape type received and calls matching function
@app.route('/', methods = ['POST', 'GET'])
def shapeType():
    shape = request.args.get('shape') #Grabs shape parameter and determines which shape it is; returns an error JSON if improper shape name
    if shape == 'circle':
        r = request.args.get('radius')
        x = request.args.get('x')
        y = request.args.get('y')
        return addCircle(r,x,y,index)
    elif shape == 'line':
        x1 = request.args.get('x1')
        y1 = request.args.get('y1')
        x2 = request.args.get('x2')
        y2 = request.args.get('y2')
        return addLine(x1,y1,x2,y2,index)
    elif shape == 'rect':
        x = request.args.get('x')
        y = request.args.get('y')
        w = request.args.get('width')
        h = request.args.get('height')
        return addRect(x,y,w,h,index)
    elif shape == 'fullUpdate':
        return jsonify(shapes)
    else:
        return jsonify({'error' : 'Bad Shape Type'})

if __name__ == '__main__':
    app.run(debug=True)
