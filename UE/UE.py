import socket
import sys
import time

# Function Definitions
def NEXG_atoi(_string: str = None) -> int:
	# This function takes in a string of numbers and returns its integer value
	# If the string sent is null or empty string '' it returns error code -1
	# If the string contains non-numbers (0-9) then it returns error code -2
	ret = 0

	if (_string == None) | (_string == ''):
		return -1

	for i in range(len(_string)):
		if ord(_string[i]) < 48 or ord(_string[i]) > 57:
			return -2
		ret += (ord(_string[i]) - 48) * (10 ** (len(_string) - i - 1))
	return ret

s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
# host="127.0.0.1"
host = "192.168.1.16"
print(host)
port=9999
s.connect((host,port))
print (s)


def printMainMenu():

	print ( '''---------------------------------------------------------
		 Welcome to NEXP5 Network Simulator
	Developed By - Nex-G Exuberant Solutions Private Limited
		      3GPP Version - Release 14.0
	---------------------------------------------------------
		       Enter UE Test Case Section
	---------------------------------------------------------
	1 - Radio Resource Control (RRC)
	2 - Radio Link Control (RLC)
	3 - Medium Access Control (MAC)
	4 - Packet Data Convergence Protocol (PDCP)
	5 - Non Access Stratum (NAS)
	---------------------------------------------------------
	Enter Test Section - 
	--------------------------------------------------------''')


	choice=int(input("ENTER YOUR CHOICE NAS [5]:"))

	if choice==1:
		RRCTestCaseMenu()
	
	elif choice==2:
		RLCTestCaseMenu()
	
	elif choice==3:
		MACTestCaseMenu()
	
	elif choice==4:
		PDCPTestCaseMenu()
	
	elif choice==5:
		NASTestCaseMenu()
	
	else:
		printMainMenu()		

#End Of PrintMainMenu

def RLCTestCaseMenu():
	printMainMenu()

#End Of RLCTestCaseMainMenu

#---------------------------------------------------------------RRC TEST CASE--------------------------------------------------------------------------
def RRCTestCaseMenu():
	print("===================RRC TEST INITIATED===================")
	class RRCinit:
		def __init__(self, data):
			self.IEs = data
			ls.append(str(self.IEs))
	ls = []

	#---------------------------------------------------------
	# Select Source and Destination Values
	#---------------------------------------------------------
	#1  - UE																0x01
	#2  - EnodeB															0x02
	#3  - MME																0x03

	# Source (UE)
	RRCinit(1)
	# Destination (N/W)
	RRCinit(2)

	#---------------------------------------------------------
	# Select Protocol Type
	#---------------------------------------------------------
	#1  - RRC																0x01
	#2  - RLC																0x02
	#3  - PDPC																0x03
	#4  - MAC																0x04
	#5  - SDAP																0x05
	#6  - NAS																0x06

	# Message/Protocol Type
	RRCinit(1)

	#---------------------------------------------------------
	# Select Message Type
	#---------------------------------------------------------
	#1  - RRC Connection Request											0x01
	#2  - RLC Connection Setup												0x02
	#3  - RRC Connection Setup Complete										0x03
	#4  - S1AP Initial UE Message											0x04

	# RRC Message Type
	RRCinit(1)

	# Message Length
	RRCinit(5)

	# Packet Information
	# 	1. Value (6 for now)
	# 	2. Choice (1 for now)
	# 	3. Establishment Cause
	# 		emergency 				= 0
	# 		high priority access 	= 1
	# 		mt access 				= 2
	# 		mo signalling 			= 3	(this one for now)
	# 		mo data 				= 4
	# 		mo voicecall 			= 5
	# 		mo videocall 			= 6
	# 		mo sms 					= 7
	# 		mps priority access 	= 8
	# 		mcs priority access 	= 9
	# 		spare 6 				= 10
	# 		spare 5 				= 11
	# 		spare 4 				= 12
	# 		spare 3 				= 13
	# 		spare 2 				= 14
	# 		spare 1 				= 15
	# 	4. Spare (0 for now)

	# Value
	RRCinit(6)

	# Choice
	RRCinit(1)

	# Establishment Cause
	RRCinit(3)

	# Spare
	RRCinit(0)

	# Start sending initial message
	print("\nSENDING DATA TO NETWORK------------------")
	print("\n-------------------------------------------------------------")
	print("UE - DATA SENT TO NETWORK...............")
		# Source
	if ls[0] == '1':
		print("UE RRC - Source is UE and Value is ", ls[0], file = sys.stdout, flush = True)
	elif ls[0] == '2':
		print("UE RRC - Source is eNodeB and Value is ", ls[0], file = sys.stdout, flush = True)
	else:
		print("UE RRC - Source is INVALID", file = sys.stdout, flush = True)
	# Destination
	if ls[1] == '1':
		print("UE RRC - Destination is UE and Value is ", ls[1], file = sys.stdout, flush = True)
	elif ls[1] == '2':
		print("UE RRC - Destination is eNodeB and Value is ", ls[1], file = sys.stdout, flush = True)
	else:
		print("UE RRC - Destination is INVALID", file = sys.stdout, flush = True)
	# 
	if ls[2] == '1':
		print("UE RRC - Protocol Type is RRC and Value is ", ls[2], file = sys.stdout, flush = True)
	else:
		print("UE RRC - Protocol Type is INVALID", file = sys.stdout, flush = True)

	if ls[3] == '1':
		print("UE RRC - Message Type is RRC Connection Request and Value is ", ls[3], file = sys.stdout, flush = True)
	elif ls[3] == '2':
		print("UE RRC - Message Type is RLC Connection Setup and Value is ", ls[3], file = sys.stdout, flush = True)
	elif ls[3] == '3':
		print("UE RRC - Message Type is RRC Connection Setup Complete and Value is ", ls[3], file = sys.stdout, flush = True)
	elif ls[3] == '4':
		print("UE RRC - Message Type is S1AP Initial UE Message and Value is ", ls[3], file = sys.stdout, flush = True)
	else:
		print("UE RRC - Message Type is INVALID", file = sys.stdout, flush = True)
	
	print("UE RRC - Message Length in RRC                       = ", ls[4], file = sys.stdout, flush = True)
	print("\nRRC Connection Packet Information:\n")
	print("UE RRC - Value being sent                            = ", ls[5], file = sys.stdout, flush = True)
	print("Initial UE Identity choice                           = ", ls[6], file = sys.stdout, flush = True)
	print("Establishment Cause - mo Signaling                   = ", ls[7], file = sys.stdout, flush = True)
	print("Spare                                                = ", ls[8], file = sys.stdout, flush = True)
	print("-------------------------------------------------------------")
	start = time.time()
	msg = ''.join(ls)
	num_bytes_sent = s.send(msg.encode())
	print("\nData sent: ", msg, file = sys.stdout, flush = True)
	print("Number of Bytes Sent: ", num_bytes_sent, file = sys.stdout, flush = True)
	end = time.time()
	print("Data send time: ", end - start, file = sys.stdout, flush = True)


	# Start receiving message
	ls = []
	num_bytes_sent = s.send("1".encode())
	print("\nRECEIVING DATA FROM NETWORK------------------")
	UE_message_raw = s.recv(9)
	for i in range(len(UE_message_raw)):
		ls.append(str(UE_message_raw[i]))
	print("Message From UE: ", ''.join(ls), file = sys.stdout, flush = True)
	print("Raw Message: ", UE_message_raw, file = sys.stdout, flush = True)

	# Received Message Parameters
	source = ls[0]
	destination = ls[1]
	protocol_type = ls[2]
	message_type = ls[3]
	message_length = ls[4]
	num_sig_bits = ls[5]
	choice = ls[6]
	spare = ls[7]
	establishment_cause = ls[8]

	print("\n-------------------------------------------------------------")
	print("UE - DATA RECEIVED FROM NETWORK...............")
	# Source
	if source == '1':
		print("UE RRC - Source is UE and Value is ", source, file = sys.stdout, flush = True)
	elif source == '2':
		print("UE RRC - Source is eNodeB and Value is ", source, file = sys.stdout, flush = True)
	else:
		print("UE RRC - Source is INVALID", file = sys.stdout, flush = True)
	# Destination
	if destination == '1':
		print("UE RRC - Destination is UE and Value is ", destination, file = sys.stdout, flush = True)
	elif destination == '2':
		print("UE RRC - Destination is eNodeB and Value is ", destination, file = sys.stdout, flush = True)
	else:
		print("UE RRC - Destination is INVALID", file = sys.stdout, flush = True)
	# 
	if protocol_type == '1':
		print("UE RRC - Protocol Type is RRC and Value is ", protocol_type, file = sys.stdout, flush = True)
	else:
		print("UE RRC - Protocol Type is INVALID", file = sys.stdout, flush = True)

	if message_type == '1':
		print("UE RRC - Message Type is RRC Connection Request and Value is ", message_type, file = sys.stdout, flush = True)
	elif message_type == '2':
		print("UE RRC - Message Type is RLC Connection Setup and Value is ", message_type, file = sys.stdout, flush = True)
	elif message_type == '3':
		print("UE RRC - Message Type is RRC Connection Setup Complete and Value is ", message_type, file = sys.stdout, flush = True)
	elif message_type == '4':
		print("UE RRC - Message Type is S1AP Initial UE Message and Value is ", message_type, file = sys.stdout, flush = True)
	else:
		print("UE RRC - Message Type is INVALID", file = sys.stdout, flush = True)

	print("UE RRC - Message Length in RRC                       = ", message_length, file = sys.stdout, flush = True)
	print("\nRRC Connection Setup Message Parameters:\n")
	print("Number of Significant Bits                           = ", num_sig_bits, file = sys.stdout, flush = True)
	print("Initial UE Identity choice from ng_5g_S_TMSI_PART1   = ", choice, file = sys.stdout, flush = True)
	print("Establishment Cause mo Signaling                     = ", establishment_cause, file = sys.stdout, flush = True)
	print("Spare from RRC Setup Request                         = ", spare, file = sys.stdout, flush = True)
	print("-------------------------------------------------------------")

	printMainMenu()

#---------------------------------------------------------------RRC TEST CASE--------------------------------------------------------------------------

#End Of RRCTestCaseMainMenu

# MACTestCaseMenu

def MACTestCaseMenu():
	printMainMenu()

#End Of MACTestCaseMainMenu

def PDCPTestCaseMenu():
	printMainMenu()
#End Of PDCPTestCaseMainMenu

def NASTestCaseMenu():
	print('''---------------------------------------------------------
		       Enter NAS Test Case Category
	---------------------------------------------------------

	1 - EMM Common Procedure
	2 - EMM Specific Procedure
	3 - EMM Connection Management Procedure
	4 - EPS session management
	5 - UE requested ESM procedures
	6 - Previous Menu Option
	---------------------------------------------------------
	Enter NAS Test Category - 
	--------------------------------------------------------
	''')


	choice=int(input("ENTER YOUR CHOICE[1-6]:"))

	if choice==1:
		EMMCommonProcedureMenu()
	
	elif choice==2:
		EMMSpecificProcedureMenu()
	
	elif choice==3:
		EMMConnectionManagementProcedureMenu()
	
	elif choice==4:
		EPSSessionManagementProcedureMenu()
	
	elif choice==5:
		UERequestedESMProcedureMenu()
	
	elif choice==6:
		printMainMenu()
	else:
		NASTestCaseMenu()

#End Of NASTestCaseMainMenu

def EMMCommonProcedureMenu():
	print('''---------------------------------------------------------
	Enter NAS - EMM Common Procedure Test Cases
	---------------------------------------------------------
	1 - GUTI Reallocation Procedure
	2 - Authentication procedure
	3 - Security mode control procedure
	4 - Identification Procedure
	5 - GUTI-Procedure
	6 - Previous Menu Option
	---------------------------------------------------------
	Enter Test Case Number -
	--------------------------------------------------------
	''')
	
	choice=int(input("ENTER YOUR CHOICE[1-6]:"))
	
	if(choice==1):
		GUTIReallocationProcedureTestCase()
	
	elif(choice==2):
		AuthenticationProcedureTestCase()
	
	elif(choice==3):
		SecurityModeControlProcedureTestCase()
	
	elif(choice==4):
		IdentificationProcedureTestCase()
	
	elif(choice==5):
		GUTIProcedureTestCase()
	
	elif(choice==6):
		NASTestCaseMenu()

	else:
		print("Invalid Menu Option")
		EMMCommonProcedureMenu()
	
#End of EMMCommonProcedureMenu

def GUTIReallocationProcedureTestCase():
	EMMCommonProcedureMenu()

def AuthenticationProcedureTestCase():
	EMMCommonProcedureMenu()

def SecurityModeControlProcedureTestCase():
	EMMCommonProcedureMenu()
#---------------------------------------------------------------IDENTIFICATION PROCEDURE TEST CASE--------------------------------------------------------------------------
def IdentificationProcedureTestCase():
	print("======================IDENTIFICATION PROCEDURE INITIATED======================", file = sys.stdout, flush = True)
	print("TCPServer Waiting for client ------------ ", file = sys.stdout, flush = True)

	UE_message_raw = s.recv(16)
	UE_message = UE_message_raw.decode('utf-8')
	print("Message From UE: ", UE_message, file = sys.stdout, flush = True)
	print("Raw Message: ", UE_message_raw, file = sys.stdout, flush = True)

	printMainMenu()
#---------------------------------------------------------------IDENTIFICATION PROCEDURE TEST CASE--------------------------------------------------------------------------
def GUTIProcedureTestCase():
	EMMCommonProcedureMenu()

def EMMSpecificProcedureMenu():
	print('''---------------------------------------------------------
               Enter NAS - EMM Specific Procedure
	---------------------------------------------------------

	1 - Attach Procedure
	2 - Detach Procdure
	3 - Tracking Area Update Procedure
	4 - eCall Inactivity Procedure
	5 - Previous Menu Option
	---------------------------------------------------------
	Enter Test Case Number -
	--------------------------------------------------------
	''')

	choice=int(input("ENTER YOUR CHOICE[1-5]:"))

	if(choice==1):
		attachProcedureTestCase()
	
	elif(choice==2):
		detachProcedureTestCase()

	elif(choice==3):
		TrackingAreaUpdateProcedureTestCase()

	elif(choice==4):
		eCallInactivityProcedureTestCase()

	elif(choice==5):
		NASTestCaseMenu()

	else:
		print("Invalid Menu Option")
		EMMSpecificProcedureMenu()

#End of EMMSpecificProcedureMenu

def detachProcedureTestCase():
	EMMSpecificProcedureMenu()

def TrackingAreaUpdateProcedureTestCase():
	EMMSpecificProcedureMenu()

def eCallInactivityProcedureTestCase():
	EMMSpecificProcedureMenu()

def EMMConnectionManagementProcedureMenu():
	print('''---------------------------------------------------------
	Enter NAS - EMM Connection Management Procedure
	--------------------------------------------------------
	
	1 - Service request procedure
	2 - Paging procedure
	3 - Transport of NAS messages procedure
	4 - Generic transport of NAS messages procedure
	5 - Previous Menu Option
	---------------------------------------------------------

	Enter Test Case Number -
	--------------------------------------------------------''')
	
	choice=int(input("ENTER YOUR CHOICE[1-5]:"))

	if(choice==1):
		ServiceRequestPrecureTestCase()

	elif(choice==2):
		PagingPrecureTestCase()

	elif(choice==3):
		TransportNASMessagePrecureTestCase()
	
	elif(choice==4):
		GenericTransportNASMessagePrecureTestCase()

	elif(choice==5):
		NASTestCaseMenu()

	else:
		print("Invalid Menu Option")
		EMMConnectionManagementProcedureMenu()

#End of EMMConnectionManagementProcedureMenu

def ServiceRequestPrecureTestCase():
	EMMConnectionManagementProcedureMenu()

def PagingPrecureTestCase():
	EMMConnectionManagementProcedureMenu()

def TransportNASMessagePrecureTestCase():
	EMMConnectionManagementProcedureMenu()

def GenericTransportNASMessagePrecureTestCase():
	EMMConnectionManagementProcedureMenu()



def EPSSessionManagementProcedureMenu():
	print('''---------------------------------------------------------
               Enter NAS - EPS session management
	---------------------------------------------------------

	1 - Default EPS bearer context activation procedure
	2 - Dedicated EPS bearer context activation procedure
	3 - EPS bearer context modification procedure
	4 - EPS bearer context deactivation procedure
	5 - Previous Menu Option
	---------------------------------------------------------
	Enter Test Case Number -
	--------------------------------------------------------''')

	choice=int(input("ENTER YOUR CHOICE[1-5]:"))

	if(choice==1):
		DefaultEPSBearerContextActivationProcedureTestCase()

	elif(choice==2):
		DedicatedEPSBearerContextActivationProcedureTestCase()

	elif(choice==3):
		EPSBearerContextModificationProcedureTestCase()

	elif(choice==4):
		EPSBearerContextDeactivationProcedureTestCase()

	elif(choice==5):
		NASTestCaseMenu()

	else:
		print("Invalid Menu Option")
		EPSSessionManagementProcedureMenu()

#End of EPSSessionManagementProcedureMenu

def DefaultEPSBearerContextActivationProcedureTestCase():
	EPSSessionManagementProcedureMenu()

def DedicatedEPSBearerContextActivationProcedureTestCase():
	EPSSessionManagementProcedureMenu()
def EPSBearerContextModificationProcedureTestCase():
	EPSSessionManagementProcedureMenu()
def EPSBearerContextDeactivationProcedureTestCase():
	EPSSessionManagementProcedureMenu()

def UERequestedESMProcedureMenu():
	print('''---------------------------------------------------------
               Enter NAS - UE requested ESM procedures
	---------------------------------------------------------

	1 - UE requested PDN connectivity procedure
	2 - UE requested PDN disconnect procedure
	3 - UE requested bearer resource allocation procedure
	4 - UE requested bearer resource modification procedure
	5 - Previous Menu Option
	---------------------------------------------------------
	Enter Test Case Number -
	--------------------------------------------------------''')
	choice=int(input("ENTER YOUR CHOICE[1-5]:"))

	if(choice==1):
		UEReqPDNConnectivityProcedureTestCase()

	elif(choice==2):
		UEReqPDNDisconnectProcedureTestCase()

	elif(choice==3):
		UEReqBearerResourceAllocProcedureTestCase()

	elif(choice==4):
		UEReqBearerResourceModProcedureTestCase()
	
	elif(choice==5):
		NASTestCaseMenu()

	else:
		print("Invalid Menu Option")
		UERequestedESMProcedureMenu()

#End Of UERequestedESMProcedureMenu

def UEReqPDNConnectivityProcedureTestCase():
	UERequestedESMProcedureMenu()
def UEReqPDNDisconnectProcedureTestCase():
	UERequestedESMProcedureMenu()
def UEReqBearerResourceAllocProcedureTestCase():
	UERequestedESMProcedureMenu()
def UEReqBearerResourceModProcedureTestCase():
	UERequestedESMProcedureMenu()
 
def attachProcedureTestCase():
	print("ATTACH PROCEDURE INITIATED")
	class attachaccept:
		def __init__(self, data):
			self.IEs = data
			ls.append(self.IEs)

	ls = []
	# These values are just examples and not direct correlation
	Message_source = attachaccept(1)
	Message_destination = attachaccept(2)
	Message_length = attachaccept(25)
	Message_size = attachaccept(sys.getsizeof(int) * 25)
	Protocol_discriminator = attachaccept(7)  # EPS mobility management messages(0111)
	Security_header_type = attachaccept(2)  # Integrity protected and ciphered(0010)
	Attach_accept_message_identity = attachaccept(66)  # message type(0100 0010)
	EPS_attach_result = attachaccept(13)  # combine EPS/IMSI attach(0010)
	Spare_half_octet = attachaccept(14)  # spare set to zero 1/2 octet
	T3412_value = attachaccept(15)  # T3412
	TAI_list = attachaccept(16)  # TAI list (lenght 7-97 bit)
	ESM_message_container = attachaccept(17)  # ESM message container contents (octet 4 to n); Max value of 65535 octets
	# This IE can contain any ESM PDU as defined in subclause
	GUTI = attachaccept(18)  # EPS mobile identity(13 octet)
	Location_area_identification = attachaccept(19)  # MCC+MNC+LAC(6 octet) include during combine attach
	MS_identity = attachaccept(20)  # type of identity(3bit)+identity digit
	EMM_cause = attachaccept(21)  # type of attach 2 octet
	tT3402_value = attachaccept(22)  # GPRS timer 2 octet
	T3423_value = attachaccept(23)  # GPRS timer 3 octet
	Equivalent_PLMNs = attachaccept(24)  # PLMN list
	Emergency_number_list = attachaccept(25)  # list 5 to 50 octet
	EPS_network_feature_support = attachaccept(26)  # feature support
	Additional_update_result = attachaccept(27)  # update
	T3412_extended_value = attachaccept(28)  # GPRS timer
	tT3324_value = attachaccept(29)  # GPRS timer
	Extended_DRX_parameters = attachaccept(30)  # DRX
	DCN_ID = attachaccept(31)  # Dedicated core NW id
	SMS_services_status = attachaccept(32)  # SMS services
	Non_3GPP_access_provided = attachaccept(33)  # Non-3GPP access provided emergency numbers policy
	T3448_value = attachaccept(34)  # GPRS Timer

	print("MESSAGE BUILT")

	for i in range(len(ls)):
		print("data sent:", i)
		s.send(str(ls[i]).encode())
		time.sleep(0.03)
	i = 0
	for i in range(5):
	    s_messg = s.recv(10)
	    print("message from UE: ", s_messg.decode('utf-8'))


printMainMenu()
