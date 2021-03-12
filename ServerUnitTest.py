import unittest 
import server

class TestSum(unittest.TestCase):

    def test_circle(self):
        cir = server.addCircle(1, 2, 3)
        self.assertEqual(cir, {'shape' : 'circle', 'data' : {'radius' : 1, 'x' : 2, 'y' : 3}}, "Should be {'shape' : 'circle', 'data' : {'radius' : 1, 'x' : 2, 'y' : 3}}")

    def test_add_line(self):
        line = server.addLine(1, 1, 1, 4)
        self.assertEqual(line, {'shape' : 'line', 'data' : {'start' : {'x' : 1, 'y' : 1}, 'end' : {'x' : 1, 'y' : 4}}}, "{'shape' : 'line', 'data' : {'start' : {'x' : 1, 'y' : 1}, 'end' : {'x' : 1, 'y' : 4}}}")

if __name__ == '__main__':
    unittest.main()

