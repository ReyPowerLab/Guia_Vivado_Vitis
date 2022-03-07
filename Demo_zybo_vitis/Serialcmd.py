
from serial import Serial
import time

class zynqTest():
	def __init__(self, port='COM8', baudrate=115200, vectorSize=128 ,tests=1):
		self.serial = Serial(port=port, baudrate=baudrate)
		self.expected = []
		self.vectors = []
		self.tests = tests
		self.vectorSize = vectorSize

	def sendVector(self, vector):
		for element in vector:
			self.serial.write( (str(element)+"\n").encode('ascii'))
			time.sleep(0.01)
	
	def receiveResult(self):
		line = self.serial.readline().decode('ascii')
		while (line == None or line == "\n"):
			time.sleep(0.2)
		y_add, y_mean = line.strip().split(":")[1].split(";")
		return float(y_add), float(y_mean)

	def readGoldenRef(self, gold):
		with open(gold,"r") as goldenRef:
			for tst in range(self.tests):
				count = 0
				auxVec = []
				while (count < self.vectorSize):
					data = goldenRef.readline().strip()
					auxVec.append(float(data))
					count += 1
				self.expected.append((float(goldenRef.readline().strip()),float(goldenRef.readline().strip())))
				self.vectors.append(auxVec)

	def closeSerial(self):
		self.serial.close()
		self.serial = None

	def runTest(self):
		result = 0
		for tst in range(self.tests):
			self.sendVector(self.vectors[tst])
			y_add, y_mean = self.receiveResult()
			add_err = 100 * abs((y_add-self.expected[tst][0])/self.expected[tst][0])
			mean_err = 100 * abs((y_mean-self.expected[tst][1])/self.expected[tst][1])
			print("TRIAL: %d,\t Expected: [%.3f,%.3f]\tGot:[%.3f,%.3f] Err: [%.6f%%,%.6f%%]\n" % (tst,self.expected[tst][0], self.expected[tst][1],y_add,y_mean,add_err, mean_err))
			if (add_err > 1 or mean_err > 1):
				result += 1
				print("TRIAL: %d FAILED!!" % tst)

		return result


if __name__ == "__main__":
	zynqDev = zynqTest('COM8', 115200, 128, 10)
	print("Listening...")
	zynqDev.readGoldenRef("..\\Demo_zybo_hls\\goldenReference.dat")
	res = zynqDev.runTest()
	if res:
		print(50 * "*")
		print("*" + 22 * " " + "FAIL" + 22 * " " + "*")
		print(50*"*")
	else:
		print(50 * "*")
		print("*" + 22 * " " + "PASS" + 22 * " " + "*")
		print(50*"*")
	print(res)

	zynqDev.closeSerial()


