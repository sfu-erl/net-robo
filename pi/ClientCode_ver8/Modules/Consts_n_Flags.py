class InitializerClass:
	def __init__(self):
		self.__GlobalZeroInitializer = 0
		self.__GlobalOneInitializer = 1
		self.__GlobalTwoInitializer = 2
		self.__GlobalTrueFlag = True
		self.__GlobalFlaseFlag = False
		self.__DrivingThreadFlag = 0

	def Set2Zero(self):
		return self.__GlobalZeroInitializer

	def Set2True(self):
		return self.__GlobalTrueFlag

	def Set2False(self):
		return self.__GlobalFlaseFlag

	def Set2One(self):
		return self.__GlobalOneInitializer

	def Set2Two(self):
		return self.__GlobalTwoInitializer

class GlobalVariables:
	def __init__(self):
		self.__exploMinSpeed = 30
		self.__exploMaxSpeed = 60
		self.__exploSpeed1 = 30
		self.__exploSpeed2 = 45
		self.__exploSpeed3 = 60
		self.__accelerationRate = 5
		self.__decelerationRate = 1	
		self.__turnRate = 1
		self.__shiftRate = 5
		self.__exploSpeedRate = 5
		self.__driveTurnSpeed = 40
		self.__MaxDriveSpeed = 100
		self.__MinDriveSpeed = -100
		self.__ServosIncrementRate = 1
		self.__ServosDecrementRate = -1
		self.__ServosStopSignal = 0
		self.__PAN = 1
		self.__TILT = 2
		self.__ELBOW = 3
		self.__CLAW = 4
		self.__WRIST = 5

	def Get_PanPin(self):
		return self.__PAN

	def Get_TiltPin(self):
		return self.__TILT

	def Get_ElbowPin(self):
		return self.__ELBOW

	def Get_ClawPin(self):
		return self.__CLAW

	def Get_WristPin(self):
		return self.__WRIST
		
	def Get_ServosIncrementRate(self):
		return self.__ServosIncrementRate

	def Get_ServosDecrementRate(self):
		return self.__ServosDecrementRate

	def Get_ServosStopSignal(self):
		return self.__ServosStopSignal
		
	def Get_exploSpeedRate(self):
		return self.__exploSpeedRate

	def Get_exploMinSpeed(self):
		return self.__exploMinSpeed

	def Get_exploMaxSpeed(self):
		return self.__exploMaxSpeed

	def Get_MaxDriveSpeed(self):
		return self.__MaxDriveSpeed

	def Get_MinDriveSpeed(self):
		return self.__MinDriveSpeed
		
	def Get_exploSpeed1(self):
		return self.__exploSpeed1

	# def Get_exploSpeed2(self):
	# 	return self.__exploSpeed2

	# def Get_exploSpeed3(self):
	# 	return self.__exploSpeed3		

	def Get_AccelRate(self):
		return self.__accelerationRate

	def Get_DecelRate(self):
		return self.__decelerationRate

	def Get_TurnRate(self):
		return self.__turnRate

	def Get_ShiftRate(self):
		return self.__shiftRate

	def Get_DriveTurnSpeed(self):
		return self.__driveTurnSpeed
