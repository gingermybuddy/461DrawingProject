import unittest
#import server
import subprocess

#check wget output
wget = "wget http://127.0.0.1:5000/addShape?shape=line&bid=test&sid=1&x1=10&y1=10&x2=20&y2=20&color=00000F"
result = subprocess.check_output(wget, shell=True)
print(result)

#check curl output
curl = "curl http://127.0.0.1:5000/addShape?shape=line&bid=test&sid=1&x1=10&y1=10&x2=20&y2=20&color=00000F"
result = subprocess.check_output(curl, shell=True)
print(result)

#check ellipse
curlE = "curl http://127.0.0.1:5000/addShape?shape=ellipse&bid=None&sid=1&x1=10&y1=10&x2=100&y2=100&color=00000F"
result = 



#class TestSum(unittest.TestCase):

 #   def test_circle(self):
  #      cir = server.addCircle(1, 2, 3)
   #     self.assertEqual(cir, {'shape' : 'circle', 'data' : {'radius' : 1, 'x' : 2, 'y' : 3}}, "Should be {'shape' : 'circle', 'data' : {'radius' : 1, 'x' : 2, 'y' : 3}}")

    #def test_add_line(self):
     #   line = server.addLine(1, 1, 1, 4)
      #  self.assertEqual(line, {'shape' : 'line', 'data' : {'start' : {'x' : 1, 'y' : 1}, 'end' : {'x' : 1, 'y' : 4}}}, "{'shape' : 'line', 'data' : {'start' : {'x' : 1, 'y' : 1}, 'end' : {'x' : 1, 'y' : 4}}}")

#if __name__ == '__main__':
 #   unittest.main()
