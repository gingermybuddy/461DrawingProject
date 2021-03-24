#Main server file, 
#Ryan Crow
#Justin Moore
#Jason Thompson

from flask import Flask, jsonify
from flask import request
import json
import sqlite3 # Library used for the databases
import drawSvg as draw # Library used to generate an SVG image 
import sys
import os.path

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

app = Flask(__name__)

PORT = 5000

def getJsonFile():
    jsonFile = request.get_json()
    data = json.loads(jsonFile)

#Creates JSON-formatted circle data
def addCircle(bid,sid,r,x,y,color):
    return jsonify({'shape' : 'circle', 'data' : {'bid' : bid, 'sid' : sid, 'radius' : r, 'x' : x, 'y' : y, 'color' : color}})

#Creates JSON-formatted line data
def addLine(bid,sid,x1,y1,x2,y2,color):
    return jsonify({'shape' : 'line', 'data' : {'bid' : bid, 'sid' : sid, 'start' : {'x' : x1, 'y' : y1}, 'end' : {'x' : x2, 'y' : y2}, 'color' : color}})

#Creates JSON-formatted rectangle data
def addRect (bid,sid,x,y,w,h,color):
    return jsonify({'shape' : 'rect', 'data' : {'bid' : bid, 'sid' : sid, 'x' : x, 'y' : y, 'w' : w, 'h' : h, 'color' : color}})

#Checks shape type received and calls matching function
@app.route('/addShape', methods = ['POST', 'GET'])
def shapeType():

    #Connects to the database
    boardId = request.args.get('bid')
    boardDBName = "board" + str(boardId) + ".db"

    connection = sqlite3.connect(boardDBName)
    cur = connection.cursor()

    shape = request.args.get('shape') #Grabs shape parameter and determines which shape it is; returns an error JSON if improper shape name
    if shape == 'circle':
        r = request.args.get('radius')
        x = request.args.get('x')
        y = request.args.get('y')
        color = request.args.get('color')
        boardId = request.args.get('bid')
        shapeId = request.args.get('sid')
        cur.execute("INSERT INTO Circle VALUES(?,?,?,?,?,?);", (boardId, shapeId, r, x, y, color)) # adds the shape into the database 
        connection.commit()
        return addCircle(boardId,shapeId,r,x,y,color)
    elif shape == 'line':
        x1 = request.args.get('x1')
        y1 = request.args.get('y1')
        x2 = request.args.get('x2')
        y2 = request.args.get('y2')
        color = request.args.get('color')
        boardId = request.args.get('bid')
        shapeId = request.args.get('sid')
        cur.execute("INSERT INTO Line VALUES(?,?,?,?,?,?,?)", (boardId, shapeId, x1, x2, y1, y2, color)) # adds the shape into the database 
        connection.commit()
        return addLine(boardId,shapeId,x1,y1,x2,y2,color)
    elif shape == 'rect':
        x = request.args.get('x')
        y = request.args.get('y')
        w = request.args.get('width')
        h = request.args.get('height')
        color = request.args.get('color')
        boardId = request.args.get('bid')
        shapeId = request.args.get('sid')
        cur.execute("INSERT INTO Rect VALUES(?,?,?,?,?,?,?)", (boardId, shapeId, x, y, w, h, color)) # adds the shape into the database 
        connection.commit()
        return addRect(boardId,shapeId,x,y,w,h,color)
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

    boardId = request.args.get('bid')
    boardDBName = "board" + str(boardId) + ".db"

    connection = sqlite3.connect(boardDBName)
    cur = connection.cursor()

    sqlCommand = """STUFF"""

    svgImageSaveFile = -1

    return svgImageSaveFile

@app.route('/fullUpdate', methods = ['POST', 'GET'])
def fullUpdate():
    
    #open up the database name passed in by http request arg    
    shapes = {}
    boardId = request.args.get('bid')
    boardDBName = "board" + str(boardId) + ".db"
    db_path = os.path.join(BASE_DIR, boardDBName)
    connection = sqlite3.connect(db_path)
    cur = connection.cursor()

    #dump shape tables
    cur.execute("SELECT * FROM Circle")
    for row in cur:
        #for each row, grab the values then place in strings
        boardId = "boardId : " + str(row[0])
        shapeId = "shapeId : " + str(row[1])
        r = "r : " + str(row[2])
        x = "x : " + str(row[3])
        y = "y : " + str(row[4])
        color = "color : " + str(row[5])
        #create json string
        temp = "{" + boardId + ", " + shapeId + ", " + r + ", " + x + ", " + y + "," + color + "}"
        #use shape id as key in the shapes dictionary
        shapes[str(row[1])] = temp

    cur.execute("SELECT * FROM Rect")
    for row in cur:
        boardId = "boardId : " + str(row[0])
        shapeId = "shapeId : " + str(row[1])
        x = "x : " + str(row[2])
        y = "y : " + str(row[3])
        w = "w : " + str(row[4])
        h = "h : " + str(row[5])
        color = "color : " + str(row[6])
        temp = "{" + boardId + ", " + shapeId + ", " + x + ", " + y + ", " + w + ", " +  h + "," + color + "}"
        shapes[str(row[1])] = temp
  
    cur.execute("SELECT * FROM Line")
    for row in cur:
        boardId = "boardId : " + str(row[0])
        shapeId = "shapeId : " + str(row[1])
        x1 = "x1 : " + str(row[2])
        y1 = "y1 : " + str(row[3])
        x2 = "x2 : " + str(row[4])
        y2 = "y2 : " + str(row[5])
        color = "color : " + str(row[6])
        temp = "{" + boardId + ", " + shapeId + ", " + x1 + ", " + y1 + ", " + x2 + ", " + y2 + "," + color + "}"
        shapes[str(row[1])] = temp

    cur.close()
    connection.close()
    #return the jsonified dictionary
    return jsonify(shapes)

@app.route('/createBoard', methods = ['POST', 'GET'])
# Create a new database for every new board that is created
def createBoard():
    boardId = request.args.get('bid')
    boardDBName = "board" + str(boardId) + ".db"

    connection = sqlite3.connect(boardDBName)
    #con = sqlite3.connect('test.db')
    cur = connection.cursor()

    # Create a table for every shape that will be present in the board
    sqlCommand = """CREATE TABLE Circle (boardId int, userId int, radius double, x int, y int, color string);"""
    cur.execute(sqlCommand)
    connection.commit()

    sqlCommand = """CREATE TABLE Line (boardId int, userId int, x1 int, x2 int, y1 int, y2 int, color string);"""
    cur.execute(sqlCommand)
    connection.commit()

    sqlCommand = """CREATE TABLE Rect (boardId int, userId int, x int, y int, width int, height int, color string);"""
    cur.execute(sqlCommand)
    connection.commit()

    cur.close()
    connection.close()
    return "Board created."

if __name__ == '__main__':
    if len(sys.argv) >= 2:
        PORT = sys.argv[1]
    app.run(debug=True, port = PORT)
