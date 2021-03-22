#Main server file, 
#Ryan Crow
#Justin Moore
#Jason Thompson

from flask import Flask, jsonify
from flask import request
import json
import sqlite3 # Library used for the databases
import drawSvg as draw # Library used to generate an SVG image 

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
@app.route('/addShape', methods = ['POST', 'GET'])
def shapeType():

    #Connects to the database
    boardId = request.args.get('id')
    boardDBName = "board" + str(boardId) + ".db"

    connection = sqlite3.connect(boardDBName)
    cur = connection.cursor()

    shape = request.args.get('shape') #Grabs shape parameter and determines which shape it is; returns an error JSON if improper shape name
    if shape == 'circle':
        r = request.args.get('radius')
        x = request.args.get('x')
        y = request.args.get('y')
        boardId = request.args.get('id')
        userId = request.args.get('uid')
        cur.execute("INSERT INTO Circle VALUES(?,?,?,?,?);", (boardId, userId, r, x, y)) # adds the shape into the database 
        connection.commit()
        return addCircle(r,x,y)
    elif shape == 'line':
        x1 = request.args.get('x1')
        y1 = request.args.get('y1')
        x2 = request.args.get('x2')
        y2 = request.args.get('y2')
        boardId = request.args.get('id')
        userId = request.args.get('uid')
        cur.execute("INSERT INTO Line VALUES(?,?,?,?,?,?)", (boardId, userId, x1, x2, y1, y2)) # adds the shape into the database 
        connection.commit()
        return addLine(x1,y1,x2,y2)
    elif shape == 'rect':
        x = request.args.get('x')
        y = request.args.get('y')
        w = request.args.get('width')
        h = request.args.get('height')
        boardId = request.args.get('id')
        userId = request.args.get('uid')
        cur.execute("INSERT INTO Circle VALUES(?,?,?,?,?,?)", (boardId, userId, x, y, w, h)) # adds the shape into the database 
        connection.commit()
        return addRect(x,y,w,h)
    else:
        return jsonify({'error' : 'Bad Shape Type'})

    cur.close()
    connection.close()

# A implementation of saveFile which allows the user to save the board that they are currently using.
# User will send this url to the server and server will reply back with an SVG file
@app.route('/saveFile', methods = ['POST', 'GET'])
def saveFile():

    # First get all the entries in the database and parse them out by shape
    # Add differend if statements for each shape 

    boardId = request.args.get('id')
    boardDBName = "board" + str(boardId) + ".db"

    connection = sqlite3.connect(boardName)
    cur = connection.cursor()

    sqlCommand = """STUFF"""

    return svgImageSaveFile


@app.route('/createBoard', methods = ['POST', 'GET'])
# Create a new database for every new board that is created
def createBoard():
    boardId = request.args.get('id')
    boardDBName = "board" + str(boardId) + ".db"

    connection = sqlite3.connect(boardDBName)
    #con = sqlite3.connect('test.db')
    cur = connection.cursor()

    # Create a table for every shape that will be present in the board
    sqlCommand = """CREATE TABLE Circle (boardId int, userId int, radius double, x int, y int);"""
    cur.execute(sqlCommand)
    connection.commit()

    sqlCommand = """CREATE TABLE Line (boardId int, userId int, x1 int, x2 int, y1 int, y2 int);"""
    cur.execute(sqlCommand)
    connection.commit()

    sqlCommand = """CREATE TABLE Rect (boardId int, userId int, x int, y int, width int, height int);"""
    cur.execute(sqlCommand)
    connection.commit()

    cur.close()
    connection.close()
    



if __name__ == '__main__':
    app.run(debug=True)
