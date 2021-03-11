from flask import Flask, jsonify
from flask import request
import json

app = Flask(__name__)

def getJsonFile():
    jsonFile = request.get_json()
    data = json.loads(jsonFile)

def addCircle(r,x,y): #Creates JSON-formatted circle data
	return jsonify({'shape' : 'circle', 'data' : {'radius' : r, 'x' : x, 'y' : y}})

def addLine(x1,y1,x2,y2): #Creates JSON-formatted line data
	return jsonify({'shape' : 'line', 'data' : {'start' : {'x' : x1, 'y' : y1}, 'end' : {'x' : x2, 'y' : y2}}})

@app.route('/', methods = ['POST', 'GET'])
def shapeType(): #Checks shape type received and calls matching function
	shape = request.args.get('shape')
	if shape == 'circle':
		r = request.args.get('radius')
		x = request.args.get('x')
		y = request.args.get('y')
		addCircle(r,x,y)
	elif shape == 'line':
		x1 = request.args.get('x1')
		y1 = request.args.get('y1')
		x2 = request.args.get('x2')
		y2 = request.args.get('y2')
		addLine(x1,y1,x2,y2)
	else:
		return jsonify({'error' : 'Bad Shape Type'})

if __name__ == '__main__':
    app.run(debug=True)
