import socket
from typing import Protocol

s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
host="192.168.1.20"
print (host)
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

def RRCTestCaseMenu():
	printMainMenu()

#End Of RRCTestCaseMainMenu

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
#--------------------------------------------------------------------------------------------------------------------------------------
def IdentificationProcedureTestCase():
	print("IDENTIFICATION PROCEDURE INITIATED")

	class identityrequest:
		def __init__(self, data):
			self.IEs = data
			ls.append(self.IEs)

	ls = []

	print('''\n
	---------------------------------------------------------
	Select Protocol Discriminator Mode
	---------------------------------------------------------
	1  - Group Call Control
	2  - Broadcast Call Control
	3  - EPS Session Management Messages
	4  - Call Control; Call Related SS Messages
	5  - GPRS Trasnparent Transport Protocol (GTTP)
	6  - Mobility Management Messages
	7  - Radio Sources Management Messages
	8  - EPS Mobility Managements Messages
	9  - GPRS Mobility Managements Messages
	10 - SMS Messages
	11 - GPRS Session Management Messages
	12 - Non Call Related SS Messages
	13 - Location Services Specified In 3GPP TS 44.071 [8a]
	14 - Reserve For Extension Of PD To One Octet Length
	15 - Used By Tests Procedures Described In 3GPP TS 44.014 [5a], 3GPP TS 34.109 [17a] and 3GPP TS 36.509 [26]
	---------------------------------------------------------
	''')

	# choice = int(input("ENTER YOUR CHOICE[1-15]: "))
	# if choice == 1:
	# 	protocol_descriminator = identityrequest(0)
	# elif(choice==2):
	# 	protocol_descriminator = identityrequest(1)
	# elif(choice==3):
	# 	protocol_descriminator = identityrequest(2)
	# elif(choice==4):
	# 	protocol_descriminator = identityrequest(3)
	# elif(choice==5):
	# 	protocol_descriminator = identityrequest(4)
	# elif(choice==6):
	# 	protocol_descriminator = identityrequest(5)
	# elif(choice==7):
	# 	protocol_descriminator = identityrequest(6)
	# elif(choice==8):
	protocol_descriminator = identityrequest(7)
	# elif(choice==9):
	# 	protocol_descriminator = identityrequest(8)
	# elif(choice==10):
	# 	protocol_descriminator = identityrequest(9)
	# elif(choice==11):
	# 	protocol_descriminator = identityrequest(10)
	# elif(choice==12):
	# 	protocol_descriminator = identityrequest(11)
	# elif(choice==13):
	# 	protocol_descriminator = identityrequest(12)
	# elif(choice==14):
	# 	protocol_descriminator = identityrequest(14)
	# elif(choice==15):
	# 	protocol_descriminator = identityrequest(15)
	# else:
	# 	print("Invalid Menu Option")
	# 	IdentificationProcedureTestCase()

	print('''\n
	---------------------------------------------------------
	Select Security Header Type
	---------------------------------------------------------
	1  - Plain NAS Message, Not Security Protected
		 Security Protected NAS Message:
	2  - Integrity Protected
	3  - Integrity Protected And Ciphered
	4  - Integrity Protected With New EPS Security Context
	5  - Integrity Protected And Ciphered With New EPS Security Context
	6  - Integrity Protected And Partially Ciphered NAS Message
		 Non-Standard L3 Message:
	7  - Security Header For The SERVICE REQUEST Message
	---------------------------------------------------------
	''')
	choice = int(input("ENTER YOUR CHOICE[1-7]: "))
	if choice == 1:
		security_header = identityrequest(0)
	elif(choice==2):
		security_header = identityrequest(1)
	elif(choice==3):
		security_header = identityrequest(2)
	elif(choice==4):
		security_header = identityrequest(3)
	elif(choice==5):
		security_header = identityrequest(4)
	elif(choice==6):
		security_header = identityrequest(5)
	elif(choice==7):
		security_header = identityrequest(12)
	else:
		print("Invalid Menu Option")
		IdentificationProcedureTestCase()

	# Identity Request (0101 0101) not Identity Response (0101 0110)
	identity_request_msg = identityrequest(85)
	
	print('''\n
	---------------------------------------------------------
	Select Identity Type / IMEI
	---------------------------------------------------------
	1  - IMSI
	2  - IMEI
	3  - IMEISV
	4  - TMSI
	---------------------------------------------------------
	''')
	choice = int(input("ENTER YOUR CHOICE[1-4]: "))
	if choice == 1:
		security_header = identityrequest(1)
	elif(choice==2):
		security_header = identityrequest(2)
	elif(choice==3):
		security_header = identityrequest(3)
	elif(choice==4):
		security_header = identityrequest(4)
	else:
		print("Invalid Menu Option")
		IdentificationProcedureTestCase()

	# Spare Octet value
	spare_octet = identityrequest(0)

	print("MESSAGE BUILT")

	# Sending the values to UE based on selection
	for i in range(len(ls)):
		print("data sent: ", i)
		s.send(str(ls[i]).encode())
	i = 0
	for i in range(5):
	    s_messg = s.recv(10)
	    print("message from UE: ", s_messg.decode('utf-8'))

	printMainMenu()
#--------------------------------------------------------------------------------------------------------------------------------------
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

	for i in range(ls.__len__()):
	    print("data sent:", i)
	    s.send(str(ls[i]).encode())
	i = 0
	for i in range(5):
	    s_messg = s.recv(10)
	    print("message from UE: ", s_messg.decode('utf-8'))


printMainMenu()
