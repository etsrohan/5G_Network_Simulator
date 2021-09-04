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

def RRCTestCaseMenu():
	print('''---------------------------------------------------------
		       Enter RRC Test Case Category
	---------------------------------------------------------

	1  - System Information Acquisition Procedure
	2  - RRC Paging procedure
	3  - RRC connection establishment
	4  - Initial security activation
	5  - RRC connection reconfiguration
	6  - Counter check
	7  - RRC connection re-establishment
	8  - RRC connection release
	9  - RRC connection release requested by upper layers
	10 - Radio resource configuration
	11 - Radio link failure related actions
	12 - UE actions upon leaving RRC_CONNECTED
	13 - UE actions upon PUCCH/ SRS release request
	14 - Proximity indication
	15 - Inter-RAT mobility
	16 - Measurements Procedure
	---------------------------------------------------------
	Enter RRC Test Category - 
	--------------------------------------------------------
	''')
	choice = int(input("ENTER YOUR CHOICE[1-16]: "))

	if choice == 1:
		SystemInformationAcquisitionProcedureTestCase()
	elif choice == 2:
		RRCPagingProcedureTestCase()
	elif choice == 3:
		RRCConnectionEstablishmentTestCase()
	elif choice == 4:
		InitialSecurityActivationTestCase()
	elif choice == 5:
		RRCConnectionReconfigurationTestCase()
	elif choice == 6:
		CounterCheckTestCase()
	elif choice == 7:
		RRCConnectionReestablishmentTestCase()
	elif choice == 8:
		RRCConnectionReleaseTestCase()
	elif choice == 9:
		RRCConnectionReleaseUpperLayersTestCase()
	elif choice == 10:
		RadioResourceConfigurationTestCase()
	elif choice == 11:
		RadioLinkFailureRelatedActionsTestCase()
	elif choice == 12:
		UEActionsUponLeavingRRCConnectedTestCase()
	elif choice == 13:
		UEActionsUponPUCCHSRSReleaseRequestTestCase()
	elif choice == 14:
		ProximityIndicationTestCase()
	elif choice == 15:
		InterRATMobilityTestCase()
	elif choice == 16:
		MeasurementsProcedureTestCase()
	else:
		printMainMenu()


#End Of RRCTestCaseMainMenu

def SystemInformationAcquisitionProcedureTestCase():
	print("System Information Acquisition Procedure")
	RRCTestCaseMenu()
#------------------------------------------------------------------------------RRC PAGING PROCEDURE-------------------------------------------------------------------------
def RRCPagingProcedureTestCase():
	print("RRC PAGING PROCEDURE INITIATED")
	printMainMenu()
#------------------------------------------------------------------------------RRC PAGING PROCEDURE-------------------------------------------------------------------------
def RRCConnectionEstablishmentTestCase():
	print("RRC connection establishment")
	RRCTestCaseMenu()
def InitialSecurityActivationTestCase():
	print("Initial security activation")
	RRCTestCaseMenu()
def RRCConnectionReconfigurationTestCase():
	print("RRC Connection Reconfiguration")
	RRCTestCaseMenu()
def CounterCheckTestCase():
	print("Counter Check")
	RRCTestCaseMenu()
def RRCConnectionReestablishmentTestCase():
	print("RRC Connection Re-establishment")
	RRCTestCaseMenu()
def RRCConnectionReleaseTestCase():
	print("RRC Connection Release")
	RRCTestCaseMenu()
def RRCConnectionReleaseUpperLayersTestCase():
	print("RRC Connection Release Upper Layers")
	RRCTestCaseMenu()
def RadioResourceConfigurationTestCase():
	print("Radio Resource Configuration")
	RRCTestCaseMenu()
def RadioLinkFailureRelatedActionsTestCase():
	print("Radio Link Failure Related Actions")
	RRCTestCaseMenu()
def UEActionsUponLeavingRRCConnectedTestCase():
	print("UE Actions Upon Leaving RRC Connected")
	RRCTestCaseMenu()
def UEActionsUponPUCCHSRSReleaseRequestTestCase():
	print("UE Actions Upon PUCCH/SRS Release Request")
	RRCTestCaseMenu()
def ProximityIndicationTestCase():
	print("Proximity Indication")
	RRCTestCaseMenu()
def InterRATMobilityTestCase():
	print("Inter RAT Mobility")
	RRCTestCaseMenu()
def MeasurementsProcedureTestCase():
	print("Measurements Procedure")
	RRCTestCaseMenu()

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
	print("IDENTIFICATION PROCEDURE INITIATED")

	class identityrequest:
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

	# Source Information
	identityrequest(2)
	
	# Destination Information
	identityrequest(1)

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
	identityrequest(6)

	# Message Length
	identityrequest(3)

	#---------------------------------------------------------
	#Select Protocol Discriminator Mode
	#---------------------------------------------------------
	#1  - Group Call Control                                                0x00
	#2  - Broadcast Call Control                                            0x01  
	#3  - EPS Session Management Messages                                   0x02 
	#4  - Call Control; Call Related SS Messages                            0x03
	#5  - GPRS Trasnparent Transport Protocol (GTTP)                        0x04
	#6  - Mobility Management Messages                                      0x05
	#7  - Radio Sources Management Messages                                 0x06
	#8  - EPS Mobility Managements Messages                                 0x07  
	#9  - GPRS Mobility Managements Messages                                0x08
	#10 - SMS Messages                                                      0x09
	#11 - GPRS Session Management Messages                                  0x0A
	#12 - Non Call Related SS Messages                                      0x0B
	#13 - Location Services Specified In 3GPP TS 44.071 [8a]                0x0C
	#14 - Reserve For Extension Of PD To One Octet Length                   0x0E
	#15 - Used By Tests Procedures Described In 3GPP TS 44.014 [5a], 3GPP TS 34.109 [17a] and 3GPP TS 36.509 [26]    0x0F
	
	protocol_descriminator = identityrequest(7)
	
	print('''\n
	---------------------------------------------------------
	Select Security Header Type
	---------------------------------------------------------
	1  - Plain NAS Message, Not Security Protected                          0x00
		 Security Protected NAS Message:              
	2  - Integrity Protected                                                0x01
	3  - Integrity Protected And Ciphered                                   0x02
	4  - Integrity Protected With New EPS Security Context                  0x03
	5  - Integrity Protected And Ciphered With New EPS Security Context     0x04
	6  - Integrity Protected And Partially Ciphered NAS Message             0x05
		 Non-Standard L3 Message:
	7  - Security Header For The SERVICE REQUEST Message                    0X0C
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
	1  - IMSI                                           0x01    
	2  - IMEI                                           0x02
	3  - IMEISV                                         0x03
	4  - TMSI                                           0x04
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

	# target_time = 0.02

	# Sending the values to UE based on selection
	start = time.time()
	msg = ''.join(ls)

	####################################################################
	# for i in range(len(ls)):
	# 	print("data sent: ", ls[i], i, file = sys.stdout, flush = True)
	# 	msg += str(ls[i])
	####################################################################
	
	num_bytes_sent = s.send(msg.encode())
	print("data sent: ", msg, file = sys.stdout, flush = True)
	print("Number of Bytes Sent: ", num_bytes_sent, file = sys.stdout, flush = True)
	end = time.time()
	print("Data send time: ", end - start, file = sys.stdout, flush = True)

	# Receiving Response Data from the UE

	UE_message_raw = s.recv(16)
	UE_message = UE_message_raw.decode('utf-8')
	print("Message From UE: ", UE_message, file = sys.stdout, flush = True)
	print("Raw Message: ", UE_message_raw, file = sys.stdout, flush = True)

	protocol_descriminator = NEXG_atoi(UE_message[1])

	security_header = NEXG_atoi(UE_message[0])

	identity_response = NEXG_atoi(UE_message[2:4])

	mcc = NEXG_atoi(UE_message[4:7])

	mnc = NEXG_atoi(UE_message[7:9])

	msisdn = NEXG_atoi(UE_message[9:])
	
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
