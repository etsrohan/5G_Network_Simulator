#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/msg.h>
#define RRCConnectSetup 2

void receiveMessage(int);
void sendDLMessage();

void *PHY_UL();
void *PHY_DL();
void *PPCP();
void *RLC();
void *RRC();
void *SDAP();

int sendConnected=0;

struct message
{
  long msgType;
  unsigned char content[50];
};

typedef struct _bit1 {
   // unsigned short  length;  /* number of significant bits */
  unsigned char value;
} _bit1;

typedef struct InitialUE_Identity {
  unsigned short choice;
#       define      ng_5G_S_TMSI_Part1_chosen 1
#       define      randomValue_chosen 2
  // union {
  //  _bit1           ng_5G_S_TMSI_Part1;  /* to choose, set choice to
  // * ng_5G_S_TMSI_Part1_chosen */
  //    _bit1           randomValue;  /* to choose, set choice to
  //  * randomValue_chosen */
  //  } u;
} InitialUE_Identity;

typedef enum EstablishmentCause {
  EstablishmentCause_emergency = 0,
  EstablishmentCause_highPriorityAccess = 1,
  EstablishmentCause_mt_Access = 2,
  EstablishmentCause_mo_Signalling = 3,
  EstablishmentCause_mo_Data = 4,
  EstablishmentCause_mo_VoiceCall = 5,
  EstablishmentCause_mo_VideoCall = 6,
  EstablishmentCause_mo_SMS = 7,
  EstablishmentCause_mps_PriorityAccess = 8,
  EstablishmentCause_mcs_PriorityAccess = 9,
  EstablishmentCause_spare6 = 10,
  EstablishmentCause_spare5 = 11,
  EstablishmentCause_spare4 = 12,
  EstablishmentCause_spare3 = 13,
  EstablishmentCause_spare2 = 14,
  EstablishmentCause_spare1 = 15
} EstablishmentCause;

typedef struct RRCSetupRequest_IEs {
  InitialUE_Identity ue_Identity;
  EstablishmentCause establishmentCause;
  //_bit1           spare;
  unsigned char spare;
} RRCSetupRequest_IEs;

/* TAG-RRCSETUPCOMPLETE-STOP */

/* TAG-RRCSETUPREQUEST-START */
typedef struct RRCSetupRequest {
    RRCSetupRequest_IEs rrcSetupRequest;
} RRCSetupRequest;

struct identityRequest
{
  unsigned char source;
  unsigned char dest;
  unsigned char protocolType;
  unsigned char messageType;
  unsigned char length;
  
  
  struct _bit1 bit1;
  struct InitialUE_Identity ue_Identity;
  unsigned char establishmentCause;
  struct RRCSetupRequest_IEs rrcSetupRequest;
  
}identity_Request;

void SendIndentityResponse(struct identityRequest response, int phy_qid);

/* TAG-RESELECTIONTHRESHOLDQ-STOP */



void *PPCP()
{
  printf("Spawning eNodeB - PPCP Thread\n");
}

void *RLC()
{
  printf("Spawning eNodeB - RLC Thread\n");
}

int charToInt(char c)
{
  int num = 0;
  num = c - '0';
  return num;
}

void *RRC()
{
  int rrcMsgId=0, phy_qid=0;
  int i=0;
  
  struct message rrcMsgReceive;
  unsigned char rcvBuffer[50];
  unsigned char length=0;
  unsigned char src=0;
  unsigned char protocolType=0;
  unsigned char rrcMessageType=0;
  unsigned char msgLength=0;
  unsigned char noOfSignicantBits=0;
  unsigned char initialChoice=0;
  unsigned char establishmentCause=0;
  unsigned char spareField=0;
  unsigned char dest=0;
  
  printf("Spawning eNodeB - RRC Thread\n");
  
  if((rrcMsgId = msgget(0x51, 0666 | IPC_CREAT))<1)
    {
      perror("eNodeB - RRC msgget failed");
    }
  
  if((phy_qid = msgget(0x052, IPC_CREAT | 0666))<0)
    {
      perror("eNodeB - PHY msgget failed");
    }
  
  while(1)
    {
      printf("Inside RRC function, Reading msgqId = %d\n", rrcMsgId);
      
      if(msgrcv(rrcMsgId, &rrcMsgReceive.content, sizeof(rrcMsgReceive.content), 0, 0)<1)
	{
	  perror("eNodeB - RRC msgrcv failed");
	} //used to receive message
      
      
      //var1 =  (unsigned int)(nasMsgReceive.content[0]);
      src = charToInt(rrcMsgReceive.content[0]);
      dest = charToInt(rrcMsgReceive.content[1]);
      protocolType = charToInt(rrcMsgReceive.content[2]);
      rrcMessageType = charToInt(rrcMsgReceive.content[3]);
      msgLength = charToInt(rrcMsgReceive.content[4]);

      noOfSignicantBits = charToInt(rrcMsgReceive.content[5]);
      initialChoice = charToInt(rrcMsgReceive.content[6]);
      establishmentCause = charToInt(rrcMsgReceive.content[7]);
      spareField = charToInt(rrcMsgReceive.content[8]);
      
      printf("----------------------------------------------------------------------------------\n");
      printf("                          Decoding packet from UE-RRC                             \n");
      printf("----------------------------------------------------------------------------------\n");

      if(src==1)
	{
	  printf("eNodeB RRC - Source is UE and value is %d\n", src);
	}
      else
	{
	  printf("eNodeB RRC - Source is INVALID  and value is %d\n", src);
	}

      if(dest==2)
	{
	  printf("eNodeB RRC - Destination is eNodeB  and value is %d\n", dest);
	}
      else
	{
	  printf("eNodeB RRC - Destination is INVALID and value is %d\n", dest);
	}

      if(protocolType==1)
	{
	  printf("eNodeB RRC - Protocol Type is RRC  and value is %d\n", protocolType);
	}
      else
	{
	  printf("eNodeB RRC - Protocol Type is INVALID and value is %d\n", protocolType);
	}
      
      switch(rrcMessageType)
	{
	case 1:
	  printf("eNodeB RRC - Message Type is  RRC Connection Request\n");
	  break;
	case 3:
	  printf("eNodeB RRC - Message Type is  RRC Connection Complete\n");
	  break;
	default:
	  printf("eNodeB RRC - Message Type is  INVALID\n");
	  break;
	}
      
      
      printf("eNodeB RRC - Message Length in RRC = %d\n", msgLength);
      
      printf("----------------------------------------------------------------------------------\n");
      printf("                           Packet received from UE-RRC                            \n");
      printf("----------------------------------------------------------------------------------\n");
      printf("eNodeB RRC - Number of significant bits = %d\n",noOfSignicantBits );
      printf("eNodeB RRC - InitialUE Identity choice from ng_5G_S_TMSI_Part1 and randomValue = %d\n", initialChoice);
      
          
      switch(establishmentCause)
	{
	case 0:
	  printf("eNodeB RRC - Establishment Cause is EMERGENCY\n");
	  break;
	case 1:
	  printf("eNodeB RRC - Establishment Cause is HIGH-PRIORITY-ACCESS\n");
	  break;
	case 2:
	  printf("eNodeB RRC - Establishment Cause is MT-ACCESS\n");
	  break;
	case 3:
	  printf("eNodeB RRC - Establishment Cause is MO-SIGNALING\n");
	  break;
	case 4:
	  printf("eNodeB RRC - Establishment Cause is MO-DATA\n");
	  break;
	case 5:
	  printf("eNodeB RRC - Establishment Cause is MO-VOICE-CALL\n");
	  break;
	case 6:
	  printf("eNodeB RRC - Establishment Cause is MO-VIDEO-CALL\n");
	  break;
	case 7:
	  printf("eNodeB RRC - Establishment Cause is MO-SMS\n");
	  break;
	case 8:
	  printf("eNodeB RRC - Establishment Cause is MPS_PRIORITYACCESS\n");
	  break;
	case 9:
	  printf("eNodeB RRC - Establishment Cause is MCS_PRIORITYACCESS\n");
	  break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	  printf("eNodeB RRC - Establishment Cause is SPARE\n");
	  break;
	default:
	  printf("eNodeB RRC - Establishment Cause is INVALID\n");
	  break;
	  
	  
	}
      
      printf("eNodeB RRC - Spare from RRC Setup Request= %d\n", spareField);
      printf("----------------------------------------------------------------------------------\n");
      identity_Request.source=dest;
      identity_Request.dest=src;
      identity_Request.protocolType=protocolType;
      identity_Request.messageType=RRCConnectSetup;
      identity_Request.length=msgLength;
      identity_Request.bit1.value=noOfSignicantBits;
      identity_Request.ue_Identity.choice=initialChoice;
      identity_Request.establishmentCause=establishmentCause;
      identity_Request.rrcSetupRequest.spare=spareField;
      
      /* Sending packet to eNodeB PHY */
      SendIndentityResponse(identity_Request, phy_qid);
    }
}

void SendIndentityResponse(struct identityRequest response, int phy_qid)
{
  if(response.source==2)
    {
      printf("eNodeB RRC - Source is eNodeB and value is %d\n", response.source);
    }
  else
    {
      printf("eNodeB RRC - Source is INVALID  and value is %d\n", response.source);
    }
  
  if(response.dest==1)
    {
      printf("eNodeB RRC - Destination is UE and value is %d\n", response.dest);
    }
  else
    {
      printf("eNodeB RRC - Source is INVALID  and value is %d\n", response.dest);
    }

  if(response.protocolType==1)
    {
      printf("eNodeB RRC - Protocol Type is RRC  and value is %d\n", response.protocolType);
    }
  else
    {
      printf("eNodeB RRC - Protocol Type is INVALID and value is %d\n", response.protocolType);
    }
  
  switch(response.messageType)
    {
    case 2:
      printf("eNodeB RRC - Message Type is  RRC Connection Setup\n");
      break;
      
    default:
      printf("eNodeB RRC - Message Type is  INVALID\n");
      break;
    }
  
  
  printf("eNodeB RRC - Message Length in RRC = %d\n", response.length);

  printf("----------------------------------------------------------------------------------\n");
  printf("                         Sending RRC Connection Setup                             \n");
  printf("----------------------------------------------------------------------------------\n");
  printf("Number of significant bits = %d\n", response.bit1.value);
  printf("InitialUE Identity choice from ng_5G_S_TMSI_Part1 and randomValue = %d\n", response.ue_Identity.choice);
  printf("EstablishmentCause mo Signalling = %d\n", response.establishmentCause);
  printf("Spare from RRC Setup Request= %d\n", response.rrcSetupRequest.spare);
  
  
  
  
  printf("\nMessage Queue Id : %d\n" , phy_qid);

  long msgType = 1;// for message type
  
  
  if(msgsnd(phy_qid, &response, sizeof(response), 0) < 0) // last argu is 0 as wait else IPC_NOWAIT
    {
      perror("eNodeB RRC - msgsnd failed"); 
    }
}

void *SDAP()
{
  printf("Spawning eNodeB - SDAP Thread\n");
}

int main() 
{ 
  pthread_t t1, t2, t3, t4, t5, t6;
  
  /* Spawning PHY UPLink Thread */
  pthread_create(&t1, NULL, PHY_UL, NULL);
  
  /* Spawning PHY DOWNLink Thread */
  pthread_create(&t2, NULL, PHY_DL, NULL);
  
  /* Spawning PPCP Thread */
  pthread_create(&t3, NULL, PPCP, NULL);
  
  /* Spawning RLC Thread */
  pthread_create(&t4, NULL, RLC, NULL);
  
  /* Spawning RRC Thread */
  pthread_create(&t5, NULL, RRC, NULL);
  
  /* Spawning SDAP Thread */
  pthread_create(&t6, NULL, SDAP, NULL);
  
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  pthread_join(t3,NULL);
  pthread_join(t4,NULL);
  pthread_join(t5,NULL);
  pthread_join(t6,NULL);
  
  return 1;
} 

void *PHY_UL()
{
  struct sockaddr_in server_addr,client_addr;
  int sock=0;
  int connected=0;
  int sin_size=0;
  int port=9999;
  
  printf("Spawning eNodeB - PHY UL Thread\n");

  if((sock = socket(AF_INET, SOCK_STREAM, 0))<0)
    {
      perror("eNodeB UL Socket Failed");
    }
    
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = INADDR_ANY;
  
  if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))<0)
    {
      perror("eNodeB UL Bind Failed");
    }

  if(listen(sock, 1)<0)
    {
      perror("eNodeB UL Listen Failed");
    }
  
  sin_size = sizeof(struct sockaddr_in);
  
  if((connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size))<0)
    {
      perror("eNodeB UL Accept Failed");
    }

  sendConnected = connected;
  printf("eNodeB UL Connected Value = %d\n",connected);
  receiveMessage(connected);
}

void receiveMessage(int connected)
{  
  unsigned char noOfBytes;
  int rrcMsgQId=0;
  int d=0;
  
  struct message sndMsg;
  
  if((rrcMsgQId = msgget(0x51, 0666 | IPC_CREAT))<0)
    {
      perror("eNodeB - RRC msgque failed");
    }
  
  sndMsg.msgType = 1;
  
  while(1)
    {
      printf("Receiving packet from Socket - %d\n", d);
      d++;
      
      if((noOfBytes = recv(connected, sndMsg.content, sizeof(sndMsg.content),0))<0)
	{
	  perror("eNodeB - PHY UL Recv failed");
	}
      
      printf("Message Content is %s and Length = %d, msgId = %d\n",sndMsg.content, noOfBytes, rrcMsgQId);
      
      //HandlePhyPacket(packet.message);
      if(msgsnd(rrcMsgQId, &sndMsg.content, sizeof(sndMsg.content), 0)<0)
	{
	  perror("eNodeB- PHY UL to RRC msgsend failed");
	}//send message
      
      printf("Sent message from eNodeB - PHY to RRC and message is : %s \n", sndMsg.content);
    }
}	

void *PHY_DL()
{
  struct sockaddr_in server_addr,client_addr;
  int sock=0;
  int connected=0;
  int sin_size=0;
  int port=8080;

  printf("Spawning eNodeB - PHY DL Thread\n");

#if 0  
  if((sock = socket(AF_INET, SOCK_STREAM, 0))<0)
    {
      perror("eNodeB - DL Socket failed");
    }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = INADDR_ANY;
  
  if(bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) <0)
    {
      perror("eNodeB DL bind failed");
    }

  if (listen(sock, 1)<0)
    {
      perror("eNodeB DL Listen failed");
    }
  
  sin_size = sizeof(struct sockaddr_in);
  
  if((connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size))<0)
    {
      perror("eNodeB DL Accept");
    }

  printf("eNodeB DL - Connected Value = %d\n",connected);
#endif    
  sendDLMessage();
}

void sendDLMessage()
{  
  unsigned char noOfBytes=0;
  int phyMsqQId=0;
  struct identityRequest rcvMsg;
  
  if((phyMsqQId = msgget(0x52, 0666 | IPC_CREAT))<0)
    {
      perror("eNodeB PHY - msgque failed");
    }

  printf("Reading Queue In eNodeB PHY - %d",phyMsqQId);
  
  while(1)
    {
            
      if(msgrcv(phyMsqQId, &rcvMsg, sizeof(rcvMsg), 0, 0)<1)
	{
	  perror("eNodeB PHY - msgrcv failed");
	}

      printf("eNodeB PHY - Received\n");
      printf("eNodeB PHY - Source in Enodeb = %d\n", rcvMsg.source);
      printf("eNodeB PHY - Destination in Enodeb = %d\n", rcvMsg.dest);
      printf("eNodeB PHY - Protolcol type in Enodeb = %d\n", rcvMsg.protocolType);
      printf("eNodeB PHY - Message type in Enodeb = %d\n", rcvMsg.messageType);
      printf("eNodeB PHY - Message Length in Enodeb = %d\n", rcvMsg.length);
      
      printf("eNodeB PHY - RRC Connection Setup Message Parameters:\n");
      
      printf("eNodeB PHY - Number of significant bits = %d\n", rcvMsg.bit1.value);
      printf("eNodeB PHY - InitialUE Identity choice from ng_5G_S_TMSI_Part1 and randomValue = %d\n", rcvMsg.ue_Identity.choice);
      printf("eNodeB PHY - EstablishmentCause mo Signalling = %d\n", rcvMsg.establishmentCause);
      printf("eNodeB PHY - Spare from RRC Setup Request= %d\n", rcvMsg.rrcSetupRequest.spare);
      write(sendConnected, &rcvMsg, sizeof(rcvMsg));
    }
  //write(sendConnected, &rcvMsg, sizeof(rcvMsg));
}	
