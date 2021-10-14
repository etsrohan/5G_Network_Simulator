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
#include "SystemInformationBlockType2.h"
#include "RRCConnectionSetup.h"
#include "RRCConnectionRequest.h"


#define RRCConnectionRequest 1
#define RRCConnectSetup 2
#define SystemInformationBlockType2 0

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
  unsigned char content[4096];
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

struct _sibType2
{
  
  unsigned char source;
  unsigned char dest;
  unsigned char protocolType;
  unsigned char messageType;
  unsigned char length;

  SystemInformationBlockType2_t sib2;
} sibType2;
struct _rrcConnectionSetup
{
  
  unsigned char source;
  unsigned char dest;
  unsigned char protocolType;
  unsigned char messageType;
  unsigned char length;

  RRCConnectionSetup_t rrcSetup;
} rrcConnectionSetup;

struct _rrcConnectionRequest
{
  
  unsigned char source;
  unsigned char dest;
  unsigned char protocolType;
  unsigned char messageType;
  unsigned char length;

  RRCConnectionRequest_t rrcReq;
} rrcConnectionReq;
void SendSystemInformationBlockType2(unsigned char *buffer, int phy_qid);
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
  
  struct message rrcMsgReceive,rrcMsgSend;
  struct message rrcSendSib2;
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
      
#if 0      
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
#endif
 
      /*Decoding of RRC connection request*/
      
      unsigned char *rrcReqTemp=rrcMsgReceive.content;
      
      rrcConnectionReq.source=*rrcReqTemp++;
         
      rrcConnectionReq.dest=*rrcReqTemp++;
      rrcConnectionReq.protcolType=*rrcReqTemp++;
      rrcConnectionReq.messageType=*rrcReqTemp++;

      rrcConnectionReq.length=*rrcReqTemp++;

      memcpy(rrcConnectionReq.rrcReq.criticalExtensions.present,rrcReqTemp++,sizeof(criticalExtensions_PR));
      
      rrcReqTemp=rrcReqTemp+size(criticalExtensions_PR);
      rrcConnectionReq.rrcReq.criticalExtensions.choice.rrcConnectionRequest_r8.ue_Identity.present=1;
      
      memcpy(rrcConnectionReq.rrcReq.criticalExtensions.choice.rrcConnectionRequest_r8.ue_Identity.present,rrcReqTemp,sizeof(InitialUE_Identity_PR));

      rrcReqTemp=rrcReqTemp+sizeof(InitialUE_Identity_PR);

      memcpy(rrcConnectionReq.rrcReq.criticalExtensions.choice.rrcConnectionRequest_r8.ue_Identity.s_TMSI.mmec,rrcReqTemp,sizeof(BIT_STRING_t));
      
      rrcReqTemp=rrcReqTemp+sizeof(BIT_STRING_t);
    
      memcpy(rrcConnectionReq.rrcReq.criticalExtensions.choice.rrcConnectionRequest_r8.ue_Identity.s_TMSI.m_TMSI,rrcReqTemp,sizeof(BIT_STRING_t));
      
      rrcReqTemp=rrcReqTemp+sizeof(BIT_STRING_t);
    
            
      memcpy(rrcConnectionReq.rrcReq.criticalExtensions.choice.rrcConnectionRequest_r8.establishmentCause,rrcReqTemp,sizeof(EstablishmentCause_t));


      rrcReqTemp=rrcReqTemp+sizeof(ENUMERATED_t);
      
      
    memcpy(&rrcConnectionReq.rrcReq.criticalExtensions.choice.rrcConnectionRequest_r8.spare,rrcReqTemp,sizeof(BIT_STRING_t));


/* Decoding of RRC Connecction Request Ends */
      
      

      /* Filling SIB2 */
      unsigned char *temp=rrcSendSib2.content;
      sibType2.source=0x02;
      memcpy(temp,&sibType2.source,sizeof(sibType2.source));
      temp=temp+sizeof(sibType2.source);
      sibType2.dest=0x01;
      memcpy(temp,&sibType2.dest,sizeof(sibType2.dest));
      temp=temp+sizeof(sibType2.dest);
      sibType2.protocolType=0x01;
      memcpy(temp,&sibType2.protocolType,sizeof(sibType2.protocolType));
      temp=temp+sizeof(sibType2.protocolType);
      sibType2.messageType=SystemInformationBlockType2;
      memcpy(temp,&sibType2.messageType,sizeof(sibType2.messageType));
      temp=temp+sizeof(sibType2.messageType);
      sibType2.length=5+sizeof(SystemInformationBlockType2_t);
      memcpy(temp,&sibType2.length,sizeof(sibType2.length));
      temp=temp+sizeof(sibType2.length);
      sibType2.sib2.ac_BarringInfo=(struct ac_BarringInfo *)malloc(sizeof(struct ac_BarringInfo)); 
      sibType2.sib2.ac_BarringInfo->ac_BarringForEmergency=1;
      memcpy(temp,&sibType2.sib2.ac_BarringInfo->ac_BarringForEmergency,sizeof(sibType2.sib2.ac_BarringInfo->ac_BarringForEmergency));
      temp=temp+sizeof(sibType2.sib2.ac_BarringInfo->ac_BarringForEmergency);
      sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Signalling=(struct AC_BarringConfig *)malloc(sizeof(struct AC_BarringConfig));
      asn_long2INTEGER(&sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Signalling->ac_BarringFactor,ac_BarringFactor_p05);
      memcpy(temp,&sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Signalling->ac_BarringFactor,sizeof(sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Signalling->ac_BarringFactor));
      temp=temp+sizeof(sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Signalling->ac_BarringFactor);
      asn_long2INTEGER(&sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Signalling->ac_BarringTime,ac_BarringTime_s8);
      memcpy(temp,&sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Signalling->ac_BarringTime,sizeof(sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Signalling->ac_BarringTime));
      temp=temp+sizeof(sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Signalling->ac_BarringTime);
      BIT_STRING_t b;
      BIT_STRING_fromBuf(&b,"11011",-1);
      sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Signalling->ac_BarringForSpecialAC=b;
      
      memcpy(temp,&sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Signalling->ac_BarringForSpecialAC,sizeof(sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Signalling->ac_BarringForSpecialAC));
      temp=temp+sizeof(sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Signalling->ac_BarringForSpecialAC);
      
      sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Data=(struct AC_BarringConfig *)malloc(sizeof(struct AC_BarringConfig));
      asn_long2INTEGER(&sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Data->ac_BarringFactor,1);
      memcpy(temp,&sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Data->ac_BarringFactor,sizeof(sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Data->ac_BarringFactor));
      temp=temp+sizeof(sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Data->ac_BarringFactor);
      asn_long2INTEGER(&sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Data->ac_BarringTime,1);
      memcpy(temp,&sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Data->ac_BarringTime,sizeof(sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Data->ac_BarringTime));
      temp=temp+sizeof(sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Data->ac_BarringTime);
      BIT_STRING_fromBuf(&b,"11011",-1);
      sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Data->ac_BarringForSpecialAC=b;
      memcpy(temp,&sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Data->ac_BarringForSpecialAC,sizeof(sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Data->ac_BarringForSpecialAC));
      temp=temp+sizeof(sibType2.sib2.ac_BarringInfo->ac_BarringForMO_Data->ac_BarringForSpecialAC);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.numberOfRA_Preambles,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.numberOfRA_Preambles,sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.numberOfRA_Preambles));
      temp=temp+sizeof(&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.numberOfRA_Preambles);
      sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.preamblesGroupAConfig=(struct preamblesGroupAConfig *)malloc(sizeof(struct preamblesGroupAConfig));
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.preamblesGroupAConfig->sizeOfRA_PreamblesGroupA,1);
      
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.preamblesGroupAConfig->sizeOfRA_PreamblesGroupA,sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.preamblesGroupAConfig->sizeOfRA_PreamblesGroupA));
      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.preamblesGroupAConfig->sizeOfRA_PreamblesGroupA);

      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.preamblesGroupAConfig->messageSizeGroupA,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.preamblesGroupAConfig->messageSizeGroupA,sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.preamblesGroupAConfig->messageSizeGroupA));
      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.preamblesGroupAConfig->messageSizeGroupA);
     asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.preamblesGroupAConfig->messagePowerOffsetGroupB,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.preamblesGroupAConfig->messagePowerOffsetGroupB,sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.preamblesGroupAConfig->messagePowerOffsetGroupB));
      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.preambleInfo.preamblesGroupAConfig->messagePowerOffsetGroupB);
     asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.powerRampingParameters.powerRampingStep,1);
      
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.powerRampingParameters.powerRampingStep,sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.powerRampingParameters.powerRampingStep));
      temp=temp+sizeof(&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.powerRampingParameters.powerRampingStep);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.powerRampingParameters.preambleInitialReceivedTargetPower,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.powerRampingParameters.preambleInitialReceivedTargetPower,sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.powerRampingParameters.preambleInitialReceivedTargetPower));
      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.powerRampingParameters.preambleInitialReceivedTargetPower);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.ra_SupervisionInfo.preambleTransMax,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.ra_SupervisionInfo     .preambleTransMax,sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.ra_SupervisionInfo     .preambleTransMax));
      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.ra_SupervisionInfo     .preambleTransMax);
     asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.ra_SupervisionInfo.ra_ResponseWindowSize,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.ra_SupervisionInfo     .ra_ResponseWindowSize,sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.ra_SupervisionInfo.ra_ResponseWindowSize));
      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.ra_SupervisionInfo     .ra_ResponseWindowSize);
     asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.ra_SupervisionInfo.mac_ContentionResolutionTimer,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.ra_SupervisionInfo     .mac_ContentionResolutionTimer,sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.ra_SupervisionInfo.mac_ContentionResolutionTimer));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.ra_SupervisionInfo     .preambleTransMax);
      sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.maxHARQ_Msg3Tx=1;
      
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.maxHARQ_Msg3Tx,sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.maxHARQ_Msg3Tx));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.maxHARQ_Msg3Tx);
      sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.rach_CE_LevelInfoList_r13=(struct RACH_CE_LevelInfoList_r13 *)malloc(sizeof(struct RACH_CE_LevelInfoList_r13));
      RACH_CE_LevelInfo_r13_t listEle1;
      listEle1.preambleMappingInfo_r13.firstPreamble_r13=1;
      
      memcpy(temp,&listEle1.preambleMappingInfo_r13.firstPreamble_r13,sizeof(listEle1.preambleMappingInfo_r13.firstPreamble_r13));

      temp=temp+sizeof(listEle1.preambleMappingInfo_r13.firstPreamble_r13);
      listEle1.preambleMappingInfo_r13.lastPreamble_r13=1;
      memcpy(temp,&listEle1.preambleMappingInfo_r13.lastPreamble_r13,sizeof(listEle1.preambleMappingInfo_r13.lastPreamble_r13));

      temp=temp+sizeof(listEle1.preambleMappingInfo_r13.lastPreamble_r13);
      asn_long2INTEGER(&listEle1.ra_ResponseWindowSize_r13,1);
      memcpy(temp,&listEle1.ra_ResponseWindowSize_r13,sizeof(listEle1.ra_ResponseWindowSize_r13));

      temp=temp+sizeof(listEle1.ra_ResponseWindowSize_r13);
      asn_long2INTEGER(&listEle1.mac_ContentionResolutionTimer_r13,1);
      memcpy(temp,&listEle1.mac_ContentionResolutionTimer_r13,sizeof(listEle1.mac_ContentionResolutionTimer_r13));

      temp=temp+sizeof(listEle1.mac_ContentionResolutionTimer_r13);
      asn_long2INTEGER(&listEle1.rar_HoppingConfig_r13,1);
      memcpy(temp,&listEle1.rar_HoppingConfig_r13,sizeof(listEle1.rar_HoppingConfig_r13));

      temp=temp+sizeof(listEle1.rar_HoppingConfig_r13);
      listEle1.edt_Parameters_r15=(struct edt_Parameters_r15 *)malloc(sizeof(struct edt_Parameters_r15));
      listEle1.edt_Parameters_r15->edt_LastPreamble_r15=1;
      memcpy(temp,&listEle1.edt_Parameters_r15->edt_LastPreamble_r15,sizeof(listEle1.edt_Parameters_r15->edt_LastPreamble_r15));

      temp=temp+sizeof(listEle1.edt_Parameters_r15->edt_LastPreamble_r15);
      listEle1.edt_Parameters_r15->edt_SmallTBS_Enabled_r15=1;
      
      memcpy(temp,&listEle1.edt_Parameters_r15->edt_SmallTBS_Enabled_r15,sizeof(listEle1.edt_Parameters_r15->edt_SmallTBS_Enabled_r15));

      temp=temp+sizeof(listEle1.edt_Parameters_r15->edt_SmallTBS_Enabled_r15);
      asn_long2INTEGER(&listEle1.edt_Parameters_r15->edt_TBS_r15,1);
      memcpy(temp,&listEle1.edt_Parameters_r15->edt_TBS_r15,sizeof(listEle1.edt_Parameters_r15->edt_TBS_r15));

      temp=temp+sizeof(listEle1.edt_Parameters_r15->edt_TBS_r15);
      asn_long2INTEGER(listEle1.edt_Parameters_r15->mac_ContentionResolutionTimer_r15,1);
      memcpy(temp,&listEle1.edt_Parameters_r15->mac_ContentionResolutionTimer_r15,sizeof(listEle1.edt_Parameters_r15->mac_ContentionResolutionTimer_r15));

      temp=temp+sizeof(listEle1.edt_Parameters_r15->mac_ContentionResolutionTimer_r15);
      ASN_SEQUENCE_ADD(&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.rach_CE_LevelInfoList_r13->list,&listEle1);
      asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.edt_SmallTBS_Subset_r15,1);   
      memcpy(temp,sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.edt_SmallTBS_Subset_r15,sizeof(*(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.edt_SmallTBS_Subset_r15)));

      temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon.edt_SmallTBS_Subset_r15));
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.bcch_Config.modificationPeriodCoeff,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.bcch_Config.modificationPeriodCoeff,sizeof(sibType2.sib2.radioResourceConfigCommon.bcch_Config.modificationPeriodCoeff));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.bcch_Config.modificationPeriodCoeff);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.pcch_Config.defaultPagingCycle,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pcch_Config.defaultPagingCycle,sizeof(sibType2.sib2.radioResourceConfigCommon.pcch_Config.defaultPagingCycle));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pcch_Config.defaultPagingCycle);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.pcch_Config.nB,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pcch_Config.nB,sizeof(sibType2.sib2.radioResourceConfigCommon.pcch_Config.nB));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pcch_Config.nB);
      sibType2.sib2.radioResourceConfigCommon.prach_Config.rootSequenceIndex=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.prach_Config.rootSequenceIndex,sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config.rootSequenceIndex));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config.rootSequenceIndex);
      sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.prach_ConfigIndex=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.prach_ConfigIndex,sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.prach_ConfigIndex));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.prach_ConfigIndex);
      sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.highSpeedFlag=1;
      
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.highSpeedFlag,sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.highSpeedFlag));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.highSpeedFlag);
      sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.zeroCorrelationZoneConfig=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.zeroCorrelationZoneConfig,sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.zeroCorrelationZoneConfig));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.zeroCorrelationZoneConfig);
      sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.prach_FreqOffset=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.prach_FreqOffset,sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.prach_FreqOffset));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config.prach_ConfigInfo.prach_FreqOffset);
      sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon.referenceSignalPower=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon.referenceSignalPower,sizeof(sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon.referenceSignalPower));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon.referenceSignalPower);
      sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon.p_b=1;
      
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon.p_b,sizeof(sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon.p_b));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon.p_b);
      sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic.n_SB=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic.n_SB,sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic.n_SB));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic.n_SB);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic.hoppingMode,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic     .hoppingMode,sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic);
      sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic.pusch_HoppingOffset=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic.pusch_HoppingOffset,sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic.pusch_HoppingOffset));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic.pusch_HoppingOffset);
      sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic.enable64QAM=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic.enable64QAM,sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic.enable64QAM));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.pusch_ConfigBasic.enable64QAM);
      sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.groupHoppingEnabled=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.groupHoppingEnabled,sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.groupHoppingEnabled));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.groupHoppingEnabled);
      sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.groupAssignmentPUSCH=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.groupAssignmentPUSCH,sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.groupAssignmentPUSCH));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.groupAssignmentPUSCH);
      sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.sequenceHoppingEnabled=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.sequenceHoppingEnabled,sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.sequenceHoppingEnabled));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.sequenceHoppingEnabled);
      sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.cyclicShift=1;
      
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.cyclicShift,sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.cyclicShift));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.cyclicShift);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.deltaPUCCH_Shift,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.deltaPUCCH_Shift,sizeof(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.deltaPUCCH_Shift));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.deltaPUCCH_Shift);
      sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.nRB_CQI=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.nRB_CQI,sizeof(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.nRB_CQI));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.nRB_CQI);
      sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.nCS_AN=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.nCS_AN,sizeof(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.nCS_AN));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.nCS_AN);
      sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.n1PUCCH_AN=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.n1PUCCH_AN,sizeof(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.n1PUCCH_AN));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon.n1PUCCH_AN);
      sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.present=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.present,sizeof(sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.present));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.present);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.choice.setup.srs_BandwidthConfig,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.choice.setup.srs_BandwidthConfig,sizeof(sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.choice.setup.srs_BandwidthConfig));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.choice.setup.srs_BandwidthConfig);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.choice.setup.srs_SubframeConfig,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.choice.setup.srs_SubframeConfig,sizeof(sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon.p_b));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.choice.setup.srs_SubframeConfig);
      sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.choice.setup.ackNackSRS_SimultaneousTransmission=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.choice.setup.ackNackSRS_SimultaneousTransmission,sizeof(sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.choice.setup.ackNackSRS_SimultaneousTransmission));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.choice.setup.ackNackSRS_SimultaneousTransmission);
      asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.choice.setup.srs_MaxUpPts,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.choice.setup.srs_MaxUpPts,sizeof(*(sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.choice.setup.srs_MaxUpPts)));

      temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.soundingRS_UL_ConfigCommon.choice.setup.srs_MaxUpPts));
      sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.p0_NominalPUSCH=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.p0_NominalPUSCH,sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.p0_NominalPUSCH));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.p0_NominalPUSCH);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.alpha,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.alpha,sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.alpha));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.alpha);
      sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.p0_NominalPUCCH=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.p0_NominalPUCCH,sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.p0_NominalPUCCH));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.p0_NominalPUCCH);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format1,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format1,sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format1));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format1);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format1b,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format1b,sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format1b));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format1b);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format2,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format2,sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format2));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format2);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format2a,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format2a,sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format2a));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format2a);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format2b,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format2b,sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format2b));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format2b);

      sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaPreambleMsg3=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaPreambleMsg3,sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaPreambleMsg3));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon.deltaPreambleMsg3);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.ul_CyclicPrefixLength,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.ul_CyclicPrefixLength,sizeof(sibType2.sib2.radioResourceConfigCommon.ul_CyclicPrefixLength));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.ul_CyclicPrefixLength);
      sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1020=(struct UplinkPowerControlCommon_v1020 *)malloc(sizeof(struct UplinkPowerControlCommon_v1020));
    
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1020->deltaF_PUCCH_Format3_r10,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1020->deltaF_PUCCH_Format3_r10,sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1020->deltaF_PUCCH_Format3_r10));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1020->deltaF_PUCCH_Format3_r10);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1020->deltaF_PUCCH_Format1bCS_r10,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1020->deltaF_PUCCH_Format1bCS_r10,sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1020->deltaF_PUCCH_Format1bCS_r10));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1020->deltaF_PUCCH_Format1bCS_r10);
      sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon_v1250=(struct RACH_ConfigCommon_v1250 *)malloc(sizeof(struct RACH_ConfigCommon_v1250));
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon_v1250->txFailParams_r12.connEstFailCount_r12,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon_v1250->txFailParams_r12.connEstFailCount_r12,sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon_v1250->txFailParams_r12.connEstFailCount_r12));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon_v1250->txFailParams_r12.connEstFailCount_r12);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon_v1250->txFailParams_r12.connEstFailOffsetValidity_r12,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon_v1250->txFailParams_r12.connEstFailOffsetValidity_r12,sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon_v1250->txFailParams_r12.connEstFailOffsetValidity_r12));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon_v1250->txFailParams_r12.connEstFailOffsetValidity_r12);

    sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon_v1250->txFailParams_r12.connEstFailOffset_r12=(long *)malloc(sizeof(long)); 

     *sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon_v1250->txFailParams_r12.connEstFailOffset_r12=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon_v1250->txFailParams_r12.connEstFailOffset_r12,sizeof(*(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon_v1250->txFailParams_r12.connEstFailOffset_r12)));

      temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.rach_ConfigCommon_v1250->txFailParams_r12.connEstFailOffset_r12));

      sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon_v1270=(struct PUSCH_ConfigCommon_v1270 *)malloc(sizeof(struct PUSCH_ConfigCommon_v1270));

      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon_v1270->enable64QAM_v1270,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon_v1270->enable64QAM_v1270,sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon_v1270->enable64QAM_v1270));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon_v1270->enable64QAM_v1270);
      sibType2.sib2.radioResourceConfigCommon.bcch_Config_v1310=(struct BCCH_Config_v1310 *)malloc(sizeof(struct BCCH_Config_v1310));
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.bcch_Config_v1310->modificationPeriodCoeff_v1310,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.bcch_Config_v1310->modificationPeriodCoeff_v1310,sizeof(sibType2.sib2.radioResourceConfigCommon.bcch_Config_v1310->modificationPeriodCoeff_v1310));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.bcch_Config_v1310->modificationPeriodCoeff_v1310);

      sibType2.sib2.radioResourceConfigCommon.pcch_Config_v1310=(struct PCCH_Config_v1310 *)malloc(sizeof(struct PCCH_Config_v1310));
      
      sibType2.sib2.radioResourceConfigCommon.pcch_Config_v1310->paging_narrowBands_r13=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pcch_Config_v1310->paging_narrowBands_r13,sizeof(sibType2.sib2.radioResourceConfigCommon.pcch_Config_v1310->paging_narrowBands_r13));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pcch_Config_v1310->paging_narrowBands_r13);
      
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.pcch_Config_v1310->mpdcch_NumRepetition_Paging_r13,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pcch_Config_v1310->mpdcch_NumRepetition_Paging_r13,sizeof(sibType2.sib2.radioResourceConfigCommon.pcch_Config_v1310->mpdcch_NumRepetition_Paging_r13));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pcch_Config_v1310->mpdcch_NumRepetition_Paging_r13);
      asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.pcch_Config_v1310->nB_v1310,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pcch_Config_v1310->nB_v1310,sizeof(*(sibType2.sib2.radioResourceConfigCommon.pcch_Config_v1310->nB_v1310)));

      temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.pcch_Config_v1310->nB_v1310));

      
      sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13=(struct FreqHoppingParameters_r13 *)malloc(sizeof(struct FreqHoppingParameters_r13));

      asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy,1);
      memcpy(temp,sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy,sizeof(*(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy)));

      temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy));
      sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy2=(struct dummy2 *)malloc(sizeof(struct dummy2));
      sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy2->present=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy2->present,sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy2->present));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy2->present);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy2->choice.interval_FDD_r13,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy2->choice.interval_FDD_r13,sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy2->choice.interval_FDD_r13));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy2->choice.interval_FDD_r13);
      
      sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy3=(struct dummy3 *)malloc(sizeof(struct dummy3));
      sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy3->present=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy3->present,sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy3->present));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy3->present);
      asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy3->choice.interval_FDD_r13,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy3->choice.interval_FDD_r13,sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy3->choice.interval_FDD_r13));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy3->choice.interval_FDD_r13);
      sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeA_r13=(struct interval_ULHoppingConfigCommonModeA_r13 *)malloc(sizeof(struct interval_ULHoppingConfigCommonModeA_r13));

      
     sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeA_r13->present=1;
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeA_r13->present,sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeA_r13->present));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeA_r13->present);
    
     asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeA_r13->choice.interval_FDD_r13,1);
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeA_r13->choice.interval_FDD_r13,sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeA_r13->choice.interval_FDD_r13));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeA_r13->choice.interval_FDD_r13);

     sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeB_r13=(struct interval_ULHoppingConfigCommonModeB_r13 *)malloc(sizeof(struct interval_ULHoppingConfigCommonModeB_r13));

      
     sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeB_r13->present=1;
     
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeB_r13->present,sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeB_r13->present));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeB_r13->present);
     asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeB_r13->choice.interval_FDD_r13,1);
    
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeB_r13->choice.interval_FDD_r13,sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeB_r13->choice.interval_FDD_r13));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->interval_ULHoppingConfigCommonModeB_r13->choice.interval_FDD_r13);
      sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy4=(long *)malloc(sizeof(long));
     *sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy4=1;
      memcpy(temp,sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy4,sizeof(*(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy4)));

      temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.freqHoppingParameters_r13->dummy4));
     sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon_v1310=(struct PDSCH_ConfigCommon_v1310 *)malloc(sizeof(struct PDSCH_ConfigCommon_v1310));
     asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon_v1310->pdsch_maxNumRepetitionCEmodeA_r13,1);
      memcpy(temp,sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon_v1310->pdsch_maxNumRepetitionCEmodeA_r13,sizeof(*(sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon_v1310->pdsch_maxNumRepetitionCEmodeA_r13)));

      temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon_v1310->pdsch_maxNumRepetitionCEmodeA_r13));
     asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon_v1310->pdsch_maxNumRepetitionCEmodeB_r13,1);
      memcpy(temp,sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon_v1310->pdsch_maxNumRepetitionCEmodeB_r13,sizeof(*(sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon_v1310->pdsch_maxNumRepetitionCEmodeB_r13)));

      temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.pdsch_ConfigCommon_v1310->pdsch_maxNumRepetitionCEmodeB_r13));

     sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon_v1310=(struct PUSCH_ConfigCommon_v1310 *)malloc(sizeof(struct PUSCH_ConfigCommon_v1310));
     asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon_v1310->pusch_maxNumRepetitionCEmodeA_r13,1);
      memcpy(temp,sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon_v1310->pusch_maxNumRepetitionCEmodeA_r13,sizeof(*(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon_v1310->pusch_maxNumRepetitionCEmodeA_r13)));

      temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon_v1310->pusch_maxNumRepetitionCEmodeA_r13));
     asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon_v1310->pusch_maxNumRepetitionCEmodeB_r13,1);

      memcpy(temp,sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon_v1310->pusch_maxNumRepetitionCEmodeA_r13,sizeof(*(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon_v1310->pusch_maxNumRepetitionCEmodeA_r13)));

      temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.pusch_ConfigCommon_v1310->pusch_maxNumRepetitionCEmodeA_r13));
     sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310=(struct PRACH_ConfigSIB_v1310 *)malloc(sizeof(struct PRACH_ConfigSIB_v1310));

     RSRP_Range_t listEle=1;
     memcpy(temp,&listEle,sizeof(listEle));

      temp=temp+sizeof(listEle);
     ASN_SEQUENCE_ADD(&sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->rsrp_ThresholdsPrachInfoList_r13.list,&listEle);
    sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->mpdcch_startSF_CSS_RA_r13=(struct mpdcch_startSF_CSS_RA_r13 *)malloc(sizeof(struct mpdcch_startSF_CSS_RA_r13));    
    
   sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->mpdcch_startSF_CSS_RA_r13->present=1;   
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->mpdcch_startSF_CSS_RA_r13->present,sizeof(sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->mpdcch_startSF_CSS_RA_r13->present));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->mpdcch_startSF_CSS_RA_r13->present);
   asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->mpdcch_startSF_CSS_RA_r13->choice.fdd_r13,1);   
      memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->mpdcch_startSF_CSS_RA_r13->choice.fdd_r13,sizeof(sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->mpdcch_startSF_CSS_RA_r13->choice.fdd_r13));

      temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->mpdcch_startSF_CSS_RA_r13->choice.fdd_r13);
    sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->prach_HoppingOffset_r13=(long *)malloc(sizeof(long));
   *sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->prach_HoppingOffset_r13=1; 
   
      memcpy(temp,sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->prach_HoppingOffset_r13,sizeof(*(sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->prach_HoppingOffset_r13)));

      temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->prach_HoppingOffset_r13));

   struct PRACH_ParametersCE_r13 listEle2;
   listEle2.prach_ConfigIndex_r13=1;
   memcpy(temp,&listEle2.prach_ConfigIndex_r13,sizeof(listEle2.prach_ConfigIndex_r13));

   temp=temp+sizeof(listEle2.prach_ConfigIndex_r13);
   listEle2.prach_FreqOffset_r13=1;
   memcpy(temp,&listEle2.prach_FreqOffset_r13,sizeof(listEle2.prach_FreqOffset_r13));

   temp=temp+sizeof(listEle2.prach_FreqOffset_r13);
  
   asn_long2INTEGER(listEle2.prach_StartingSubframe_r13,1);
   memcpy(temp,listEle2.prach_StartingSubframe_r13,sizeof(*(listEle2.prach_StartingSubframe_r13)));

   temp=temp+sizeof(*(listEle2.prach_StartingSubframe_r13));
   asn_long2INTEGER(listEle2.maxNumPreambleAttemptCE_r13,1);
   memcpy(temp,listEle2.maxNumPreambleAttemptCE_r13,sizeof(*(listEle2.maxNumPreambleAttemptCE_r13)));

   temp=temp+sizeof(*(listEle2.maxNumPreambleAttemptCE_r13));

   asn_long2INTEGER(&listEle2.numRepetitionPerPreambleAttempt_r13,1);
   memcpy(temp,&listEle2.numRepetitionPerPreambleAttempt_r13,sizeof(listEle2.numRepetitionPerPreambleAttempt_r13));

   temp=temp+sizeof(listEle2.numRepetitionPerPreambleAttempt_r13);

   long var=1;
   
   memcpy(temp,&var,sizeof(var));

   temp=temp+sizeof(var);

   ASN_SEQUENCE_ADD(&listEle2.mpdcch_NarrowbandsToMonitor_r13.list,&var);
   asn_long2INTEGER(&listEle2.mpdcch_NumRepetition_RA_r13,1);
   memcpy(temp,&listEle2.mpdcch_NumRepetition_RA_r13,sizeof(listEle2.mpdcch_NumRepetition_RA_r13));

   temp=temp+sizeof(listEle2.mpdcch_NumRepetition_RA_r13);
   asn_long2INTEGER(&listEle2.prach_HoppingConfig_r13,1);
   memcpy(temp,&listEle2.prach_HoppingConfig_r13,sizeof(listEle2.prach_HoppingConfig_r13));

   temp=temp+sizeof(listEle2.prach_HoppingConfig_r13);

   ASN_SEQUENCE_ADD(&sibType2.sib2.radioResourceConfigCommon.prach_ConfigCommon_v1310->prach_ParametersListCE_r13.list,&listEle2);
  
   sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310=(struct PUCCH_ConfigCommon_v1310 *)malloc(sizeof(struct PUCCH_ConfigCommon_v1310));
   sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->n1PUCCH_AN_InfoList_r13=(struct N1PUCCH_AN_InfoList_r13 *)malloc(sizeof(struct N1PUCCH_AN_InfoList_r13));
   
  long var1=1;
   memcpy(temp,&var1,sizeof(var1));

   temp=temp+sizeof(var1);
  ASN_SEQUENCE_ADD(&sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->n1PUCCH_AN_InfoList_r13->list,&var1);

  asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level0_r13,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level0_r13,sizeof(*(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level0_r13)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level0_r13));
  asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level1_r13,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level1_r13,sizeof(*(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level1_r13)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level1_r13));
  asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level2_r13,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level2_r13,sizeof(*(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level2_r13)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level2_r13));
  asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level3_r13,1);

    
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level3_r13,sizeof(*(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level3_r13)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1310->pucch_NumRepetitionCE_Msg4_Level3_r13));
    sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_r14=(struct HighSpeedConfig_r14 *)malloc(sizeof(struct HighSpeedConfig_r14));

    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_r14->highSpeedEnhancedMeasFlag_r14,1);

   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_r14->highSpeedEnhancedMeasFlag_r14,sizeof(*(sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_r14->highSpeedEnhancedMeasFlag_r14)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_r14->highSpeedEnhancedMeasFlag_r14));
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_r14->highSpeedEnhancedDemodulationFlag_r14,1);
    
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_r14->highSpeedEnhancedDemodulationFlag_r14,sizeof(*(sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_r14->highSpeedEnhancedDemodulationFlag_r14)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_r14->highSpeedEnhancedDemodulationFlag_r14));
    sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430=(struct PRACH_Config_v1430 *)malloc(sizeof(struct PRACH_Config_v1430));
    sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->rootSequenceIndexHighSpeed_r14=1;
    
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->rootSequenceIndexHighSpeed_r14,sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->rootSequenceIndexHighSpeed_r14));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->rootSequenceIndexHighSpeed_r14);
    sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->zeroCorrelationZoneConfigHighSpeed_r14=1;
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->zeroCorrelationZoneConfigHighSpeed_r14,sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->zeroCorrelationZoneConfigHighSpeed_r14));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->zeroCorrelationZoneConfigHighSpeed_r14);
    sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->prach_ConfigIndexHighSpeed_r14=1;
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->prach_ConfigIndexHighSpeed_r14,sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->prach_ConfigIndexHighSpeed_r14));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->prach_ConfigIndexHighSpeed_r14);
    sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->prach_FreqOffsetHighSpeed_r14=1;
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->prach_FreqOffsetHighSpeed_r14,sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->prach_FreqOffsetHighSpeed_r14));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.prach_Config_v1430->prach_FreqOffsetHighSpeed_r14);

    sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1430=(struct PUCCH_ConfigCommon_v1430 *)malloc(sizeof(struct PUCCH_ConfigCommon_v1430));

    asn_long2INTEGER (sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1430->pucch_NumRepetitionCE_Msg4_Level3_r14,1);
    
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1430->pucch_NumRepetitionCE_Msg4_Level3_r14,sizeof(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1430->pucch_NumRepetitionCE_Msg4_Level3_r14));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.pucch_ConfigCommon_v1430->pucch_NumRepetitionCE_Msg4_Level3_r14);
    sibType2.sib2.radioResourceConfigCommon.prach_Config_v1530=(struct PRACH_ConfigSIB_v1530 *)malloc(sizeof(struct PRACH_ConfigSIB_v1530));

    struct EDT_PRACH_ParametersCE_r15 listEle3;
    listEle3.edt_PRACH_ParametersCE_r15=(struct edt_PRACH_ParametersCE_r15 *)malloc(sizeof(struct edt_PRACH_ParametersCE_r15));
    listEle3.edt_PRACH_ParametersCE_r15->prach_ConfigIndex_r15=1;
   memcpy(temp,&listEle3.edt_PRACH_ParametersCE_r15->prach_ConfigIndex_r15,sizeof(listEle3.edt_PRACH_ParametersCE_r15->prach_ConfigIndex_r15));

   temp=temp+sizeof(listEle3.edt_PRACH_ParametersCE_r15->prach_ConfigIndex_r15);
    listEle3.edt_PRACH_ParametersCE_r15->prach_FreqOffset_r15=1;
   memcpy(temp,&listEle3.edt_PRACH_ParametersCE_r15->prach_FreqOffset_r15,sizeof(listEle3.edt_PRACH_ParametersCE_r15->prach_FreqOffset_r15));

   temp=temp+sizeof(listEle3.edt_PRACH_ParametersCE_r15->prach_FreqOffset_r15);
    asn_long2INTEGER(listEle3.edt_PRACH_ParametersCE_r15->prach_StartingSubframe_r15,1);
   memcpy(temp,listEle3.edt_PRACH_ParametersCE_r15->prach_StartingSubframe_r15,sizeof(*(listEle3.edt_PRACH_ParametersCE_r15->prach_StartingSubframe_r15)));

   temp=temp+sizeof(*(listEle3.edt_PRACH_ParametersCE_r15->prach_StartingSubframe_r15));
    long var3=1;
   memcpy(temp,&var3,sizeof(var3));

   temp=temp+sizeof(var3);
    ASN_SEQUENCE_ADD(&listEle3.edt_PRACH_ParametersCE_r15->mpdcch_NarrowbandsToMonitor_r15.list,&var3);
    
    ASN_SEQUENCE_ADD(&sibType2.sib2.radioResourceConfigCommon.prach_Config_v1530->edt_PRACH_ParametersListCE_r15.list,&listEle3);
    
    sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15=(struct RSS_Config_r15 *)malloc(sizeof(struct RSS_Config_r15));
    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->duration_r15,1);
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->duration_r15,sizeof(sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->duration_r15));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->duration_r15);
    sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->freqLocation_r15=1;
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->freqLocation_r15,sizeof(sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->freqLocation_r15));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->freqLocation_r15);
    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->periodicity_r15,1);
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->periodicity_r15,sizeof(sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->periodicity_r15));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->periodicity_r15);
    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->powerBoost_r15,1);
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->powerBoost_r15,sizeof(sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->powerBoost_r15));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->powerBoost_r15);


    sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->timeOffset_r15=1;
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->timeOffset_r15,sizeof(sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->timeOffset_r15));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.ce_RSS_Config_r15->timeOffset_r15);

    sibType2.sib2.radioResourceConfigCommon.wus_Config_r15=(struct WUS_Config_r15 *)malloc(sizeof(struct WUS_Config_r15));
    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->maxDurationFactor_r15,1);
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->maxDurationFactor_r15,sizeof(sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->maxDurationFactor_r15));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->maxDurationFactor_r15);
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->numPOs_r15,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->numPOs_r15,sizeof(*(sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->numPOs_r15)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->numPOs_r15));
    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->freqLocation_r15,1);
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->freqLocation_r15,sizeof(sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->freqLocation_r15));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->freqLocation_r15);
    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->timeOffsetDRX_r15,1);
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->timeOffsetDRX_r15,sizeof(sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->timeOffsetDRX_r15));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->timeOffsetDRX_r15);
    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->timeOffset_eDRX_Short_r15,1);
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->timeOffset_eDRX_Short_r15,sizeof(sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->timeOffset_eDRX_Short_r15));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->timeOffset_eDRX_Short_r15);
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->timeOffset_eDRX_Long_r15,1);
    
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->timeOffset_eDRX_Long_r15,sizeof(*(sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->timeOffset_eDRX_Long_r15)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.wus_Config_r15->timeOffset_eDRX_Long_r15));
    sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_v1530=(struct HighSpeedConfig_v1530 *)malloc(sizeof(struct HighSpeedConfig_v1530));
    
    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_v1530->highSpeedMeasGapCE_ModeA_r15,1);
    
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_v1530->highSpeedMeasGapCE_ModeA_r15,sizeof(sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_v1530->highSpeedMeasGapCE_ModeA_r15));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_v1530->highSpeedMeasGapCE_ModeA_r15);
    sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540=(struct UplinkPowerControlCommon_v1530 *)malloc(sizeof(struct UplinkPowerControlCommon_v1530));
    sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.present=1;
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.present,sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.present));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.present);
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format1_r15,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format1_r15,sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format1_r15)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format1_r15));
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format1a_r15,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format1a_r15,sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format1a_r15)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format1a_r15));
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format1b_r15,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format1b_r15,sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format1b_r15)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format1b_r15));
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format3_r15,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format3_r15,sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format3_r15)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_Format3_r15));
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_RM_Format4_r15,1);

   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_RM_Format4_r15,sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_RM_Format4_r15)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_RM_Format4_r15));
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_TBCC_Format4_r15,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_TBCC_Format4_r15,sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_TBCC_Format4_r15)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_slotSPUCCH_TBCC_Format4_r15));
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_Format1and1a_r15,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_Format1and1a_r15,sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_Format1and1a_r15)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_Format1and1a_r15));
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_Format1b_r15,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_Format1and1a_r15,sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_Format1and1a_r15)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_Format1and1a_r15));
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_RM_Format4_r15,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_RM_Format4_r15,sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_RM_Format4_r15)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_RM_Format4_r15));
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_TBCC_Format4_r15,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_TBCC_Format4_r15,sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_TBCC_Format4_r15)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1540->deltaFList_SPUCCH_r15.choice.setup.deltaF_subslotSPUCCH_TBCC_Format4_r15));

    sibType2.sib2.radioResourceConfigCommon.wus_Config_v1560=(struct WUS_Config_v1560 *)malloc(sizeof(struct WUS_Config_v1560));
    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.wus_Config_v1560->powerBoost_r15,1);

   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.wus_Config_v1560->powerBoost_r15,sizeof(sibType2.sib2.radioResourceConfigCommon.wus_Config_v1560->powerBoost_r15));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.wus_Config_v1560->powerBoost_r15);
    sibType2.sib2.radioResourceConfigCommon.wus_Config_v1610=(struct WUS_Config_v1610 *)malloc(sizeof(struct WUS_Config_v1610));
    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.wus_Config_v1610->numDRX_CyclesRelaxed_r16,1);

   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.wus_Config_v1610->numDRX_CyclesRelaxed_r16,sizeof(sibType2.sib2.radioResourceConfigCommon.wus_Config_v1610->numDRX_CyclesRelaxed_r16));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.wus_Config_v1610->numDRX_CyclesRelaxed_r16);
    sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_v1610=(struct HighSpeedConfig_v1610 *)malloc(sizeof(struct HighSpeedConfig_v1610));

    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_v1610->highSpeedEnhMeasFlag2_r16,1);
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_v1610->highSpeedEnhMeasFlag2_r16,sizeof(sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_v1610->highSpeedEnhMeasFlag2_r16));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_v1610->highSpeedEnhMeasFlag2_r16);
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_v1610->highSpeedEnhDemodFlag2_r16,1);

   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_v1610->highSpeedEnhDemodFlag2_r16,sizeof(sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_v1610->highSpeedEnhDemodFlag2_r16));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.highSpeedConfig_v1610->highSpeedEnhDemodFlag2_r16);
    sibType2.sib2.radioResourceConfigCommon.crs_ChEstMPDCCH_ConfigCommon_r16=(struct CRS_ChEstMPDCCH_ConfigCommon_r16 *)malloc(sizeof(struct CRS_ChEstMPDCCH_ConfigCommon_r16));
    
    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.crs_ChEstMPDCCH_ConfigCommon_r16->powerRatio_r16,1);

   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.crs_ChEstMPDCCH_ConfigCommon_r16->powerRatio_r16,sizeof(sibType2.sib2.radioResourceConfigCommon.crs_ChEstMPDCCH_ConfigCommon_r16->powerRatio_r16));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.crs_ChEstMPDCCH_ConfigCommon_r16->powerRatio_r16);
    sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16=(struct GWUS_Config_r16 *)malloc(sizeof(struct GWUS_Config_r16));
    
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->groupAlternation_r16,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->groupAlternation_r16,sizeof(*(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->groupAlternation_r16)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->groupAlternation_r16));
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->commonSequence_r16,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->commonSequence_r16,sizeof(*(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->commonSequence_r16)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->commonSequence_r16));
  
    sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16=(struct GWUS_TimeParameters_r16 *)malloc(sizeof(struct GWUS_TimeParameters_r16));

    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->maxDurationFactor_r16,1);
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->maxDurationFactor_r16,sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->maxDurationFactor_r16));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->maxDurationFactor_r16);
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->numPOs_r16,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->numPOs_r16,sizeof(*(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->numPOs_r16)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->numPOs_r16));
    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->timeOffsetDRX_r16,1);
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->timeOffsetDRX_r16,sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->timeOffsetDRX_r16));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->timeOffsetDRX_r16);

    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->timeOffset_eDRX_Short_r16,1);
   memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->timeOffset_eDRX_Short_r16,sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->timeOffset_eDRX_Short_r16));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->timeOffset_eDRX_Short_r16);
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->timeOffset_eDRX_Long_r16,1);
   memcpy(temp,sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->timeOffset_eDRX_Long_r16,sizeof(*(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->timeOffset_eDRX_Long_r16)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->timeOffset_eDRX_Long_r16));
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->numDRX_CyclesRelaxed_r16,1);
    memcpy(temp,sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->numDRX_CyclesRelaxed_r16,sizeof(*(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->numDRX_CyclesRelaxed_r16)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->numDRX_CyclesRelaxed_r16));
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->powerBoost_r16,1);

    memcpy(temp,sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->powerBoost_r16,sizeof(*(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->powerBoost_r16)));

   temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->timeParameters_r16->powerBoost_r16));
    sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfigDRX_r16.resourceMappingPattern_r16.present=1;
    memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfigDRX_r16.resourceMappingPattern_r16.present,sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfigDRX_r16.resourceMappingPattern_r16.present));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfigDRX_r16.resourceMappingPattern_r16.present);
    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfigDRX_r16.resourceMappingPattern_r16.choice.resourceLocationWithWUS,1);

    memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfigDRX_r16.resourceMappingPattern_r16.choice.resourceLocationWithWUS,sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfigDRX_r16.resourceMappingPattern_r16.choice.resourceLocationWithWUS));

   temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfigDRX_r16.resourceMappingPattern_r16.choice.resourceLocationWithWUS);
    sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfigDRX_r16.numGroupsList_r16=(struct GWUS_NumGroupsList_r16 *)malloc(sizeof(struct GWUS_NumGroupsList_r16));

    GWUS_NumGroups_r16_t listEle4;
    asn_long2INTEGER(&listEle4,1);
    memcpy(temp,&listEle4,sizeof(listEle4));

    temp=temp+sizeof(listEle4);
    ASN_SEQUENCE_ADD(&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfigDRX_r16.numGroupsList_r16->list,&listEle4);    

    
    sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfigDRX_r16.groupsForServiceList_r16=(struct GWUS_GroupsForServiceList_r16 *)malloc(sizeof(struct GWUS_GroupsForServiceList_r16));
    
    long var4=1; 
    memcpy(temp,&var4,sizeof(var4));

    temp=temp+sizeof(var4);
    ASN_SEQUENCE_ADD(&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfigDRX_r16.    groupsForServiceList_r16->list,&var4);


    sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Short_r16=(struct GWUS_ResourceConfig_r16 *)malloc(sizeof(struct GWUS_ResourceConfig_r16));
  
    sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Short_r16->resourceMappingPattern_r16.present=1;
    memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Short_r16->resourceMappingPattern_r16.present,sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Short_r16->resourceMappingPattern_r16.present));

    temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Short_r16->resourceMappingPattern_r16.present);
    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Short_r16->resourceMappingPattern_r16.choice.resourceLocationWithWUS,1);
    memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Short_r16->resourceMappingPattern_r16.choice.resourceLocationWithWUS,sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Short_r16->resourceMappingPattern_r16.choice.resourceLocationWithWUS));

    temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Short_r16->resourceMappingPattern_r16.choice.resourceLocationWithWUS);

    sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Short_r16->numGroupsList_r16=(struct GWUS_NumGroupsList_r16 *)malloc(sizeof(struct GWUS_NumGroupsList_r16));

    GWUS_NumGroups_r16_t listEle5;
    asn_long2INTEGER(&listEle5,1);
    memcpy(temp,&listEle5,sizeof(listEle5));

    temp=temp+sizeof(listEle5);
    ASN_SEQUENCE_ADD(&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Short_r16->numGroupsList_r16->list,&listEle5);    

    
    sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Long_r16->groupsForServiceList_r16=(struct GWUS_GroupsForServiceList_r16 *)malloc(sizeof(struct GWUS_GroupsForServiceList_r16));
    
    long var6=1; 
    memcpy(temp,&var6,sizeof(var6));

    temp=temp+sizeof(var6);
    ASN_SEQUENCE_ADD(&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Long_r16->groupsForServiceList_r16->list,&var6);
    
    
    sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Long_r16=(struct GWUS_ResourceConfig_r16 *)malloc(sizeof(struct GWUS_ResourceConfig_r16));
  
    sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Long_r16->resourceMappingPattern_r16.present=1;
    memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Long_r16->resourceMappingPattern_r16.present,sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Long_r16->resourceMappingPattern_r16.present));

    temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Long_r16->resourceMappingPattern_r16.present);
    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Long_r16->resourceMappingPattern_r16.choice.resourceLocationWithWUS,1);

    memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Long_r16->resourceMappingPattern_r16.choice.resourceLocationWithWUS,sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Long_r16->resourceMappingPattern_r16.choice.resourceLocationWithWUS));

    temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Long_r16->resourceMappingPattern_r16.choice.resourceLocationWithWUS);
    sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Long_r16->numGroupsList_r16=(struct GWUS_NumGroupsList_r16 *)malloc(sizeof(struct GWUS_NumGroupsList_r16));

    GWUS_NumGroups_r16_t listEle6;
    asn_long2INTEGER(&listEle6,1);
    memcpy(temp,&listEle6,sizeof(listEle6));

    temp=temp+sizeof(listEle6);
    ASN_SEQUENCE_ADD(&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Long_r16->numGroupsList_r16->list,&listEle6);    

    
    sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Long_r16->groupsForServiceList_r16=(struct GWUS_GroupsForServiceList_r16 *)malloc(sizeof(struct GWUS_GroupsForServiceList_r16));
    
    long var7=1; 
    memcpy(temp,&var7,sizeof(var7));

    temp=temp+sizeof(var7);
    ASN_SEQUENCE_ADD(&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->resourceConfig_eDRX_Long_r16->groupsForServiceList_r16->list,&var7);


    sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->probThreshList_r16=(struct GWUS_ProbThreshList_r16 *)malloc(sizeof(struct GWUS_ProbThreshList_r16));

    GWUS_PagingProbThresh_r16_t listEle7;
    asn_long2INTEGER(&listEle7,1);
    memcpy(temp,&listEle7,sizeof(listEle7));

    temp=temp+sizeof(listEle7);

    ASN_SEQUENCE_ADD(&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->probThreshList_r16->list,&listEle7);
    
    sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->groupNarrowBandList_r16=(struct GWUS_GroupNarrowBandList_r16 *)malloc(sizeof(struct GWUS_GroupNarrowBandList_r16));    

    BOOLEAN_t boolVar=1;

    memcpy(temp,&boolVar,sizeof(boolVar));

    temp=temp+sizeof(boolVar);
    ASN_SEQUENCE_ADD(&sibType2.sib2.radioResourceConfigCommon.gwus_Config_r16->probThreshList_r16->list,&boolVar);

    sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1610=(struct UplinkPowerControlCommon_v1610 *)malloc(sizeof(struct UplinkPowerControlCommon_v1610));

    asn_long2INTEGER(&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1610->alphaSRS_Add_r16,1);

    memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1610->alphaSRS_Add_r16,sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1610->alphaSRS_Add_r16));

    temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1610->alphaSRS_Add_r16);
    sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1610->p0_NominalSRS_Add_r16=1;

    memcpy(temp,&sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1610->p0_NominalSRS_Add_r16,sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1610->p0_NominalSRS_Add_r16));

    temp=temp+sizeof(sibType2.sib2.radioResourceConfigCommon.uplinkPowerControlCommon_v1610->p0_NominalSRS_Add_r16);
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.rss_MeasConfig_r16,1);
    memcpy(temp,sibType2.sib2.radioResourceConfigCommon.rss_MeasConfig_r16,sizeof(*(sibType2.sib2.radioResourceConfigCommon.rss_MeasConfig_r16)));

    temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.rss_MeasConfig_r16));
    asn_long2INTEGER(sibType2.sib2.radioResourceConfigCommon.rss_MeasNonNCL_r16,1);

    memcpy(temp,sibType2.sib2.radioResourceConfigCommon.rss_MeasNonNCL_r16,sizeof(*(sibType2.sib2.radioResourceConfigCommon.rss_MeasNonNCL_r16)));

    temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.rss_MeasNonNCL_r16));
    BIT_STRING_fromBuf(sibType2.sib2.radioResourceConfigCommon.puncturedSubcarriersDL_r16,"11011",-1);
    memcpy(temp,sibType2.sib2.radioResourceConfigCommon.puncturedSubcarriersDL_r16,sizeof(BIT_STRING_t));

    temp=temp+sizeof(BIT_STRING_t);
    sibType2.sib2.radioResourceConfigCommon.highSpeedInterRAT_NR_r16=(BOOLEAN_t *)malloc(sizeof(BOOLEAN_t));
    *sibType2.sib2.radioResourceConfigCommon.highSpeedInterRAT_NR_r16=1;
    memcpy(temp,sibType2.sib2.radioResourceConfigCommon.highSpeedInterRAT_NR_r16,sizeof(*(sibType2.sib2.radioResourceConfigCommon.highSpeedInterRAT_NR_r16)));

    temp=temp+sizeof(*(sibType2.sib2.radioResourceConfigCommon.highSpeedInterRAT_NR_r16));

    asn_long2INTEGER(&sibType2.sib2.ue_TimersAndConstants.t300,1);
    memcpy(temp,&sibType2.sib2.ue_TimersAndConstants.t300,sizeof(sibType2.sib2.ue_TimersAndConstants.t300));

    temp=temp+sizeof(sibType2.sib2.ue_TimersAndConstants.t300);
    asn_long2INTEGER(&sibType2.sib2.ue_TimersAndConstants.t301,1);
    memcpy(temp,&sibType2.sib2.ue_TimersAndConstants.t301,sizeof(sibType2.sib2.ue_TimersAndConstants.t301));

    temp=temp+sizeof(sibType2.sib2.ue_TimersAndConstants.t301);
    asn_long2INTEGER(&sibType2.sib2.ue_TimersAndConstants.t310,1);
    memcpy(temp,&sibType2.sib2.ue_TimersAndConstants.t310,sizeof(sibType2.sib2.ue_TimersAndConstants.t310));

    temp=temp+sizeof(sibType2.sib2.ue_TimersAndConstants.t310);
    asn_long2INTEGER(&sibType2.sib2.ue_TimersAndConstants.n310,1);
    memcpy(temp,&sibType2.sib2.ue_TimersAndConstants.n310,sizeof(sibType2.sib2.ue_TimersAndConstants.n310));

    temp=temp+sizeof(sibType2.sib2.ue_TimersAndConstants.n310);
    asn_long2INTEGER(&sibType2.sib2.ue_TimersAndConstants.t311,1);
    memcpy(temp,&sibType2.sib2.ue_TimersAndConstants.t311,sizeof(sibType2.sib2.ue_TimersAndConstants.t311));

    temp=temp+sizeof(sibType2.sib2.ue_TimersAndConstants.t311);
    asn_long2INTEGER(&sibType2.sib2.ue_TimersAndConstants.n311,1);
    memcpy(temp,&sibType2.sib2.ue_TimersAndConstants.n311,sizeof(sibType2.sib2.ue_TimersAndConstants.n311));

    temp=temp+sizeof(sibType2.sib2.ue_TimersAndConstants.n311);
    asn_long2INTEGER(sibType2.sib2.ue_TimersAndConstants.t300_v1310,1);
    memcpy(temp,sibType2.sib2.ue_TimersAndConstants.t300_v1310,sizeof(*(sibType2.sib2.ue_TimersAndConstants.t300_v1310)));

    temp=temp+sizeof(*(sibType2.sib2.ue_TimersAndConstants.t300_v1310));
    asn_long2INTEGER(sibType2.sib2.ue_TimersAndConstants.t301_v1310,1);
    memcpy(temp,sibType2.sib2.ue_TimersAndConstants.t301_v1310,sizeof(*(sibType2.sib2.ue_TimersAndConstants.t301_v1310)));

    temp=temp+sizeof(*(sibType2.sib2.ue_TimersAndConstants.t301_v1310));
    asn_long2INTEGER(sibType2.sib2.ue_TimersAndConstants.t310_v1330,1);
    memcpy(temp,sibType2.sib2.ue_TimersAndConstants.t310_v1330,sizeof(*(sibType2.sib2.ue_TimersAndConstants.t310_v1330)));

    temp=temp+sizeof(*(sibType2.sib2.ue_TimersAndConstants.t310_v1330));
    asn_long2INTEGER(sibType2.sib2.ue_TimersAndConstants.t300_r15,1);
    
    memcpy(temp,sibType2.sib2.ue_TimersAndConstants.t300_r15,sizeof(*(sibType2.sib2.ue_TimersAndConstants.t300_r15)));

    temp=temp+sizeof(*(sibType2.sib2.ue_TimersAndConstants.t300_r15));
  
    sibType2.sib2.freqInfo.ul_CarrierFreq=(ARFCN_ValueEUTRA_t *)malloc(sizeof(ARFCN_ValueEUTRA_t));
    *sibType2.sib2.freqInfo.ul_CarrierFreq=1;
    memcpy(temp,sibType2.sib2.freqInfo.ul_CarrierFreq,sizeof(*(sibType2.sib2.freqInfo.ul_CarrierFreq)));

    temp=temp+sizeof(*(sibType2.sib2.freqInfo.ul_CarrierFreq));
    asn_long2INTEGER(sibType2.sib2.freqInfo.ul_Bandwidth,1);
    memcpy(temp,sibType2.sib2.freqInfo.ul_Bandwidth,sizeof(*(sibType2.sib2.freqInfo.ul_Bandwidth)));

    temp=temp+sizeof(*(sibType2.sib2.freqInfo.ul_Bandwidth));

    sibType2.sib2.freqInfo.additionalSpectrumEmission=1;
    memcpy(temp,&sibType2.sib2.freqInfo.additionalSpectrumEmission,sizeof(sibType2.sib2.freqInfo.additionalSpectrumEmission));

    temp=temp+sizeof(sibType2.sib2.freqInfo.additionalSpectrumEmission);

    sibType2.sib2.mbsfn_SubframeConfigList=(struct MBSFN_SubframeConfigList *)malloc(sizeof(struct MBSFN_SubframeConfigList));

    struct MBSFN_SubframeConfig listEle8;
    asn_long2INTEGER(&listEle8.radioframeAllocationPeriod,1);
    memcpy(temp,&listEle8.radioframeAllocationPeriod,sizeof(listEle8.radioframeAllocationPeriod));

    temp=temp+sizeof(listEle8.radioframeAllocationPeriod);
    listEle8.radioframeAllocationOffset=1;
    memcpy(temp,&listEle8.radioframeAllocationOffset,sizeof(listEle8.radioframeAllocationOffset));

    temp=temp+sizeof(listEle8.radioframeAllocationOffset);

    listEle8.subframeAllocation.present=1;
    memcpy(temp,&listEle8.subframeAllocation.present,sizeof(listEle8.subframeAllocation.present));

    temp=temp+sizeof(listEle8.subframeAllocation.present);
    BIT_STRING_fromBuf(&listEle8.subframeAllocation.choice.oneFrame,"11011",-1);
    memcpy(temp,&listEle8.subframeAllocation.choice.oneFrame,sizeof(BIT_STRING_t));

    temp=temp+sizeof(BIT_STRING_t);
    ASN_SEQUENCE_ADD(&sibType2.sib2.mbsfn_SubframeConfigList->list,&listEle8); 

    asn_long2INTEGER(&sibType2.sib2.timeAlignmentTimerCommon,1);
    memcpy(temp,&sibType2.sib2.timeAlignmentTimerCommon,sizeof(sibType2.sib2.timeAlignmentTimerCommon));

    temp=temp+sizeof(sibType2.sib2.timeAlignmentTimerCommon);
    sibType2.sib2.lateNonCriticalExtension=(OCTET_STRING_t *)malloc(sizeof(OCTET_STRING_t)); 
    OCTET_STRING_fromBuf(sibType2.sib2.lateNonCriticalExtension,"11011011", -1);

    memcpy(temp,sibType2.sib2.lateNonCriticalExtension,sizeof(sibType2.sib2.lateNonCriticalExtension));

    temp=temp+sizeof(sibType2.sib2.lateNonCriticalExtension);
    sibType2.sib2.ssac_BarringForMMTEL_Voice_r9=(struct AC_BarringConfig *)malloc(sizeof(struct AC_BarringConfig));

    asn_long2INTEGER(&sibType2.sib2.ssac_BarringForMMTEL_Voice_r9->ac_BarringFactor,1);
    memcpy(temp,&sibType2.sib2.ssac_BarringForMMTEL_Voice_r9->ac_BarringFactor,sizeof(sibType2.sib2.ssac_BarringForMMTEL_Voice_r9->ac_BarringFactor));

    temp=temp+sizeof(sibType2.sib2.ssac_BarringForMMTEL_Voice_r9->ac_BarringFactor);
    asn_long2INTEGER(&sibType2.sib2.ssac_BarringForMMTEL_Voice_r9->ac_BarringTime,1);

    memcpy(temp,&sibType2.sib2.ssac_BarringForMMTEL_Voice_r9->ac_BarringTime,sizeof(sibType2.sib2.ssac_BarringForMMTEL_Voice_r9->ac_BarringTime));

    temp=temp+sizeof(sibType2.sib2.ssac_BarringForMMTEL_Voice_r9->ac_BarringTime);
    BIT_STRING_fromBuf(&sibType2.sib2.ssac_BarringForMMTEL_Voice_r9->ac_BarringForSpecialAC,"11011",-1);

    memcpy(temp,&sibType2.sib2.ssac_BarringForMMTEL_Voice_r9->ac_BarringForSpecialAC,sizeof(BIT_STRING_t));

    temp=temp+sizeof(BIT_STRING_t);
    asn_long2INTEGER(&sibType2.sib2.ssac_BarringForMMTEL_Video_r9->ac_BarringFactor,1);
    memcpy(temp,&sibType2.sib2.ssac_BarringForMMTEL_Video_r9->ac_BarringFactor,sizeof(sibType2.sib2.ssac_BarringForMMTEL_Video_r9->ac_BarringFactor));

    temp=temp+sizeof(sibType2.sib2.ssac_BarringForMMTEL_Video_r9->ac_BarringFactor);
    asn_long2INTEGER(&sibType2.sib2.ssac_BarringForMMTEL_Video_r9->ac_BarringTime,1);
    memcpy(temp,&sibType2.sib2.ssac_BarringForMMTEL_Video_r9->ac_BarringTime,sizeof(sibType2.sib2.ssac_BarringForMMTEL_Video_r9->ac_BarringTime));

    temp=temp+sizeof(sibType2.sib2.ssac_BarringForMMTEL_Video_r9->ac_BarringTime);

    BIT_STRING_fromBuf(&sibType2.sib2.ssac_BarringForMMTEL_Video_r9->ac_BarringForSpecialAC,"11011",-1);

    memcpy(temp,&sibType2.sib2.ssac_BarringForMMTEL_Video_r9->ac_BarringForSpecialAC,sizeof(BIT_STRING_t));

    temp=temp+sizeof(BIT_STRING_t);

    asn_long2INTEGER(&sibType2.sib2.ac_BarringForCSFB_r10->ac_BarringFactor,1);
    memcpy(temp,&sibType2.sib2.ac_BarringForCSFB_r10->ac_BarringFactor,sizeof(sibType2.sib2.ac_BarringForCSFB_r10->ac_BarringFactor));

    temp=temp+sizeof(sibType2.sib2.ac_BarringForCSFB_r10->ac_BarringFactor);
    asn_long2INTEGER(&sibType2.sib2.ac_BarringForCSFB_r10->ac_BarringTime,1);

    memcpy(temp,&sibType2.sib2.ac_BarringForCSFB_r10->ac_BarringTime,sizeof(sibType2.sib2.ac_BarringForCSFB_r10->ac_BarringTime));

    temp=temp+sizeof(sibType2.sib2.ac_BarringForCSFB_r10->ac_BarringTime);
    BIT_STRING_fromBuf(&sibType2.sib2.ac_BarringForCSFB_r10->ac_BarringForSpecialAC,"11011",-1);

    memcpy(temp,&sibType2.sib2.ac_BarringForCSFB_r10->ac_BarringForSpecialAC,sizeof(BIT_STRING_t));

    temp=temp+sizeof(BIT_STRING_t);
    asn_long2INTEGER(sibType2.sib2.ac_BarringSkipForMMTELVoice_r12,0);
    memcpy(temp,sibType2.sib2.ac_BarringSkipForMMTELVoice_r12,sizeof(*(sibType2.sib2.ac_BarringSkipForMMTELVoice_r12)));

    temp=temp+sizeof(*(sibType2.sib2.ac_BarringSkipForMMTELVoice_r12));
    asn_long2INTEGER(sibType2.sib2.ac_BarringSkipForMMTELVideo_r12,0);
    memcpy(temp,sibType2.sib2.ac_BarringSkipForMMTELVideo_r12,sizeof(*(sibType2.sib2.ac_BarringSkipForMMTELVideo_r12)));

    temp=temp+sizeof(*(sibType2.sib2.ac_BarringSkipForMMTELVideo_r12));
    asn_long2INTEGER(sibType2.sib2.ac_BarringSkipForSMS_r12,0);
    
    memcpy(temp,sibType2.sib2.ac_BarringSkipForSMS_r12,sizeof(*(sibType2.sib2.ac_BarringSkipForSMS_r12)));

    temp=temp+sizeof(*(sibType2.sib2.ac_BarringSkipForSMS_r12));
    sibType2.sib2.ac_BarringPerPLMN_List_r12=(struct AC_BarringPerPLMN_List_r12 *)malloc(sizeof(struct AC_BarringPerPLMN_List_r12));

    struct AC_BarringPerPLMN_r12 listEle9;
    listEle9.plmn_IdentityIndex_r12=1;
    memcpy(temp,&listEle9.plmn_IdentityIndex_r12,sizeof(listEle9.plmn_IdentityIndex_r12));

    temp=temp+sizeof(listEle9.plmn_IdentityIndex_r12);
    listEle9.ac_BarringInfo_r12=(struct ac_BarringInfo_r12 *)malloc(sizeof(struct ac_BarringInfo_r12));
    listEle9.ac_BarringInfo_r12->ac_BarringForEmergency_r12=1;
    memcpy(temp,&listEle9.ac_BarringInfo_r12->ac_BarringForEmergency_r12,sizeof(listEle9.ac_BarringInfo_r12->ac_BarringForEmergency_r12));

    temp=temp+sizeof(listEle9.ac_BarringInfo_r12->ac_BarringForEmergency_r12);
    listEle9.ac_BarringInfo_r12->ac_BarringForMO_Signalling_r12=(struct AC_BarringConfig *)malloc(sizeof(struct AC_BarringConfig));
    

    asn_long2INTEGER(&listEle9.ac_BarringInfo_r12->ac_BarringForMO_Signalling_r12->ac_BarringFactor,1);
    memcpy(temp,&listEle9.ac_BarringInfo_r12->ac_BarringForMO_Signalling_r12->ac_BarringFactor,sizeof(listEle9.ac_BarringInfo_r12->ac_BarringForMO_Signalling_r12->ac_BarringFactor));

    temp=temp+sizeof(listEle9.ac_BarringInfo_r12->ac_BarringForMO_Signalling_r12->ac_BarringFactor);
    asn_long2INTEGER(&listEle9.ac_BarringInfo_r12->ac_BarringForMO_Signalling_r12->ac_BarringTime,1);

    memcpy(temp,&listEle9.ac_BarringInfo_r12->ac_BarringForMO_Signalling_r12->ac_BarringTime,sizeof(listEle9.ac_BarringInfo_r12->ac_BarringForMO_Signalling_r12->ac_BarringTime));

    temp=temp+sizeof(listEle9.ac_BarringInfo_r12->ac_BarringForMO_Signalling_r12->ac_BarringTime);
    BIT_STRING_fromBuf(&listEle9.ac_BarringInfo_r12->ac_BarringForMO_Signalling_r12->ac_BarringForSpecialAC,"11011",-1);

    memcpy(temp,&listEle9.ac_BarringInfo_r12->ac_BarringForMO_Signalling_r12->ac_BarringForSpecialAC,sizeof(BIT_STRING_t));

    temp=temp+sizeof(BIT_STRING_t);

    listEle9.ac_BarringInfo_r12->ac_BarringForMO_Data_r12=(struct AC_BarringConfig *)malloc(sizeof(struct AC_BarringConfig));
    

    asn_long2INTEGER(&listEle9.ac_BarringInfo_r12->ac_BarringForMO_Data_r12->ac_BarringFactor,1);
    memcpy(temp,&listEle9.ac_BarringInfo_r12->ac_BarringForMO_Data_r12->ac_BarringFactor,sizeof(listEle9.ac_BarringInfo_r12->ac_BarringForMO_Data_r12->ac_BarringFactor));

    temp=temp+sizeof(listEle9.ac_BarringInfo_r12->ac_BarringForMO_Data_r12->ac_BarringFactor);
    asn_long2INTEGER(&listEle9.ac_BarringInfo_r12->ac_BarringForMO_Data_r12->ac_BarringTime,1);

    memcpy(temp,&listEle9.ac_BarringInfo_r12->ac_BarringForMO_Data_r12->ac_BarringTime,sizeof(listEle9.ac_BarringInfo_r12->ac_BarringForMO_Data_r12->ac_BarringTime));

    temp=temp+sizeof(listEle9.ac_BarringInfo_r12->ac_BarringForMO_Data_r12->ac_BarringTime);
    BIT_STRING_fromBuf(&listEle9.ac_BarringInfo_r12->ac_BarringForMO_Data_r12->ac_BarringForSpecialAC,"11011",-1);

    memcpy(temp,&listEle9.ac_BarringInfo_r12->ac_BarringForMO_Data_r12->ac_BarringForSpecialAC,sizeof(BIT_STRING_t));

    temp=temp+sizeof(BIT_STRING_t);

    asn_long2INTEGER(listEle9.ac_BarringSkipForMMTELVoice_r12,0);
    memcpy(temp,listEle9.ac_BarringSkipForMMTELVoice_r12,sizeof(*(listEle9.ac_BarringSkipForMMTELVoice_r12)));

    temp=temp+sizeof(listEle9.ac_BarringSkipForMMTELVoice_r12);
    asn_long2INTEGER(listEle9.ac_BarringSkipForMMTELVideo_r12,0);
    memcpy(temp,listEle9.ac_BarringSkipForMMTELVideo_r12,sizeof(*(listEle9.ac_BarringSkipForMMTELVideo_r12)));

    temp=temp+sizeof(*(listEle9.ac_BarringSkipForMMTELVideo_r12));
    asn_long2INTEGER(listEle9.ac_BarringSkipForSMS_r12,0);

    memcpy(temp,listEle9.ac_BarringSkipForMMTELVoice_r12,sizeof(*(listEle9.ac_BarringSkipForMMTELVoice_r12)));

    temp=temp+sizeof(*(listEle9.ac_BarringSkipForMMTELVoice_r12));

    listEle9.ac_BarringForCSFB_r12=(struct AC_BarringConfig *)malloc(sizeof(struct AC_BarringConfig));
    
    asn_long2INTEGER(&listEle9.ac_BarringForCSFB_r12->ac_BarringFactor,1);
    memcpy(temp,&listEle9.ac_BarringForCSFB_r12->ac_BarringFactor,sizeof(listEle9.ac_BarringForCSFB_r12->ac_BarringFactor));

    temp=temp+sizeof(listEle9.ac_BarringForCSFB_r12->ac_BarringFactor);
    asn_long2INTEGER(&listEle9.ac_BarringForCSFB_r12->ac_BarringTime,1);

    memcpy(temp,&listEle9.ac_BarringForCSFB_r12->ac_BarringTime,sizeof(listEle9.ac_BarringForCSFB_r12->ac_BarringTime));

    temp=temp+sizeof(listEle9.ac_BarringForCSFB_r12->ac_BarringTime);
    BIT_STRING_fromBuf(&listEle9.ac_BarringForCSFB_r12->ac_BarringForSpecialAC,"11011",-1);

    memcpy(temp,&listEle9.ac_BarringForCSFB_r12->ac_BarringForSpecialAC,sizeof(BIT_STRING_t));

    temp=temp+sizeof(BIT_STRING_t);

    listEle9.ssac_BarringForMMTEL_Voice_r12=(struct AC_BarringConfig *)malloc(sizeof(struct AC_BarringConfig));
    
    asn_long2INTEGER(&listEle9.ssac_BarringForMMTEL_Voice_r12->ac_BarringFactor,1);
    memcpy(temp,&listEle9.ssac_BarringForMMTEL_Voice_r12->ac_BarringFactor,sizeof(listEle9.ssac_BarringForMMTEL_Voice_r12->ac_BarringFactor));

    temp=temp+sizeof(listEle9.ssac_BarringForMMTEL_Voice_r12->ac_BarringFactor);
    asn_long2INTEGER(&listEle9.ssac_BarringForMMTEL_Voice_r12->ac_BarringTime,1);
    memcpy(temp,&listEle9.ssac_BarringForMMTEL_Voice_r12->ac_BarringTime,sizeof(listEle9.ssac_BarringForMMTEL_Voice_r12->ac_BarringTime));

    temp=temp+sizeof(listEle9.ssac_BarringForMMTEL_Voice_r12->ac_BarringTime);

    BIT_STRING_fromBuf(&listEle9.ssac_BarringForMMTEL_Voice_r12->ac_BarringForSpecialAC,"11011",-1);
    

    memcpy(temp,&listEle9.ssac_BarringForMMTEL_Voice_r12->ac_BarringForSpecialAC,sizeof(BIT_STRING_t));

    temp=temp+sizeof(BIT_STRING_t);

    listEle9.ssac_BarringForMMTEL_Video_r12=(struct AC_BarringConfig *)malloc(sizeof(struct AC_BarringConfig));
    
    asn_long2INTEGER(&listEle9.ssac_BarringForMMTEL_Video_r12->ac_BarringFactor,1);
    memcpy(temp,&listEle9.ssac_BarringForMMTEL_Video_r12->ac_BarringFactor,sizeof(listEle9.ssac_BarringForMMTEL_Video_r12->ac_BarringFactor));

    temp=temp+sizeof(listEle9.ssac_BarringForMMTEL_Video_r12->ac_BarringFactor);
    asn_long2INTEGER(&listEle9.ssac_BarringForMMTEL_Video_r12->ac_BarringTime,1);
    memcpy(temp,&listEle9.ssac_BarringForMMTEL_Video_r12->ac_BarringTime,sizeof(listEle9.ssac_BarringForMMTEL_Video_r12->ac_BarringTime));

    temp=temp+sizeof(listEle9.ssac_BarringForMMTEL_Video_r12->ac_BarringTime);

    BIT_STRING_fromBuf(&listEle9.ssac_BarringForMMTEL_Video_r12->ac_BarringForSpecialAC,"11011",-1);

    memcpy(temp,&listEle9.ssac_BarringForMMTEL_Video_r12->ac_BarringForSpecialAC,sizeof(BIT_STRING_t));

    temp=temp+sizeof(BIT_STRING_t);

    ASN_SEQUENCE_ADD(&sibType2.sib2.ac_BarringPerPLMN_List_r12->list,&listEle9);

    sibType2.sib2.voiceServiceCauseIndication_r12=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));

    asn_long2INTEGER(sibType2.sib2.voiceServiceCauseIndication_r12,1);
    memcpy(temp,&sibType2.sib2.voiceServiceCauseIndication_r12,sizeof(*(sibType2.sib2.voiceServiceCauseIndication_r12)));

    temp=temp+sizeof(*(sibType2.sib2.voiceServiceCauseIndication_r12));

    sibType2.sib2.acdc_BarringForCommon_r13=(struct ACDC_BarringForCommon_r13 *)malloc(sizeof(struct ACDC_BarringForCommon_r13));

    sibType2.sib2.acdc_BarringForCommon_r13->acdc_HPLMNonly_r13=1;
    
    memcpy(temp,&sibType2.sib2.acdc_BarringForCommon_r13->acdc_HPLMNonly_r13,sizeof(sibType2.sib2.acdc_BarringForCommon_r13->acdc_HPLMNonly_r13));

    temp=temp+sizeof(sibType2.sib2.acdc_BarringForCommon_r13->acdc_HPLMNonly_r13);
    struct BarringPerACDC_Category_r13 listEle10;
    listEle10.acdc_Category_r13=1;
    
    memcpy(temp,&listEle10.acdc_Category_r13,sizeof(listEle10.acdc_Category_r13));

    temp=temp+sizeof(listEle10.acdc_Category_r13);
    listEle10.acdc_BarringConfig_r13=(struct acdc_BarringConfig_r13 *)malloc(sizeof(struct acdc_BarringConfig_r13));

    asn_long2INTEGER(&listEle10.acdc_BarringConfig_r13->ac_BarringFactor_r13,1);
    memcpy(temp,&listEle10.acdc_BarringConfig_r13->ac_BarringFactor_r13,sizeof(listEle10.acdc_BarringConfig_r13->ac_BarringFactor_r13));

    temp=temp+sizeof(listEle10.acdc_BarringConfig_r13->ac_BarringFactor_r13);
    asn_long2INTEGER(&listEle10.acdc_BarringConfig_r13->ac_BarringTime_r13,1);

    memcpy(temp,&listEle10.acdc_BarringConfig_r13->ac_BarringTime_r13,sizeof(listEle10.acdc_BarringConfig_r13->ac_BarringTime_r13));

    temp=temp+sizeof(listEle10.acdc_BarringConfig_r13->ac_BarringTime_r13);

    ASN_SEQUENCE_ADD(&sibType2.sib2.acdc_BarringForCommon_r13->barringPerACDC_CategoryList_r13.list,&listEle10);
   
    struct ACDC_BarringPerPLMN_r13 listEle11;
    listEle11.plmn_IdentityIndex_r13=1;
    memcpy(temp,&listEle11.plmn_IdentityIndex_r13,sizeof(listEle11.plmn_IdentityIndex_r13));

    temp=temp+sizeof(listEle11.plmn_IdentityIndex_r13);
    listEle11.acdc_OnlyForHPLMN_r13=1;
    memcpy(temp,&listEle11.acdc_OnlyForHPLMN_r13,sizeof(listEle11.acdc_OnlyForHPLMN_r13));

    temp=temp+sizeof(listEle11.acdc_OnlyForHPLMN_r13);
    struct BarringPerACDC_Category_r13 listEle11_1;
    listEle11_1.acdc_Category_r13=1;
    memcpy(temp,&listEle11_1.acdc_Category_r13,sizeof(listEle11_1.acdc_Category_r13));

    temp=temp+sizeof(listEle11_1.acdc_Category_r13);
    listEle11_1.acdc_BarringConfig_r13=(struct acdc_BarringConfig_r13 *)malloc(sizeof(struct acdc_BarringConfig_r13));
    asn_long2INTEGER(&listEle11_1.acdc_BarringConfig_r13->ac_BarringFactor_r13,1);
    memcpy(temp,&listEle11_1.acdc_BarringConfig_r13->ac_BarringFactor_r13,sizeof(listEle11_1.acdc_BarringConfig_r13->ac_BarringFactor_r13));

    temp=temp+sizeof(listEle11_1.acdc_BarringConfig_r13->ac_BarringFactor_r13);
    asn_long2INTEGER(&listEle11_1.acdc_BarringConfig_r13->ac_BarringTime_r13,1);
    memcpy(temp,&listEle11_1.acdc_BarringConfig_r13->ac_BarringTime_r13,sizeof(listEle11_1.acdc_BarringConfig_r13->ac_BarringTime_r13));

    temp=temp+sizeof(listEle11_1.acdc_BarringConfig_r13->ac_BarringTime_r13);
    
    ASN_SEQUENCE_ADD(&listEle11.barringPerACDC_CategoryList_r13.list,&listEle11_1);


    sibType2.sib2.acdc_BarringPerPLMN_List_r13=(struct ACDC_BarringPerPLMN_List_r13 *)malloc(sizeof(struct ACDC_BarringPerPLMN_List_r13));
    ASN_SEQUENCE_ADD(&sibType2.sib2.acdc_BarringPerPLMN_List_r13->list,&listEle11);
    
    sibType2.sib2.udt_RestrictingForCommon_r13=(struct UDT_Restricting_r13 *)malloc(sizeof(struct UDT_Restricting_r13));

    asn_long2INTEGER(sibType2.sib2.udt_RestrictingForCommon_r13->udt_Restricting_r13,0);
    memcpy(temp,sibType2.sib2.udt_RestrictingForCommon_r13->udt_Restricting_r13,sizeof(*(sibType2.sib2.udt_RestrictingForCommon_r13->udt_Restricting_r13)));

    temp=temp+sizeof(*(sibType2.sib2.udt_RestrictingForCommon_r13->udt_Restricting_r13));
    asn_long2INTEGER(sibType2.sib2.udt_RestrictingForCommon_r13->udt_RestrictingTime_r13,0);

    memcpy(temp,sibType2.sib2.udt_RestrictingForCommon_r13->udt_RestrictingTime_r13,sizeof(*(sibType2.sib2.udt_RestrictingForCommon_r13->udt_RestrictingTime_r13)));

    temp=temp+sizeof(*(sibType2.sib2.udt_RestrictingForCommon_r13->udt_RestrictingTime_r13));
    sibType2.sib2.udt_RestrictingPerPLMN_List_r13=(struct UDT_RestrictingPerPLMN_List_r13 *)malloc(sizeof(struct UDT_RestrictingPerPLMN_List_r13));

    struct UDT_RestrictingPerPLMN_r13 listEle12;
    listEle12.plmn_IdentityIndex_r13=1;
    memcpy(temp,&listEle12.plmn_IdentityIndex_r13,sizeof(listEle12.plmn_IdentityIndex_r13));

    temp=temp+sizeof(listEle12.plmn_IdentityIndex_r13);
    listEle12.udt_Restricting_r13=(struct UDT_Restricting_r13 *)malloc(sizeof(struct UDT_Restricting_r13));
    asn_long2INTEGER(listEle12.udt_Restricting_r13->udt_Restricting_r13,0);
    memcpy(temp,listEle12.udt_Restricting_r13->udt_Restricting_r13,sizeof(*(listEle12.udt_Restricting_r13->udt_Restricting_r13)));

    temp=temp+sizeof(*(listEle12.udt_Restricting_r13->udt_Restricting_r13));
    asn_long2INTEGER(listEle12.udt_Restricting_r13->udt_RestrictingTime_r13,0);


    memcpy(temp,listEle12.udt_Restricting_r13->udt_RestrictingTime_r13,sizeof(*(listEle12.udt_Restricting_r13->udt_RestrictingTime_r13)));

    temp=temp+sizeof(*(listEle12.udt_Restricting_r13->udt_RestrictingTime_r13));
    
    ASN_SEQUENCE_ADD(&sibType2.sib2.udt_RestrictingPerPLMN_List_r13->list,&listEle12);

    sibType2.sib2.cIoT_EPS_OptimisationInfo_r13=(struct CIOT_EPS_OptimisationInfo_r13 *)malloc(sizeof(struct CIOT_EPS_OptimisationInfo_r13));

    struct CIOT_OptimisationPLMN_r13 listEle13;
    asn_long2INTEGER(listEle13.up_CIoT_EPS_Optimisation_r13,1);
    memcpy(temp,listEle13.up_CIoT_EPS_Optimisation_r13,sizeof(*(listEle13.up_CIoT_EPS_Optimisation_r13)));

    temp=temp+sizeof(*(listEle13.up_CIoT_EPS_Optimisation_r13));
    asn_long2INTEGER(listEle13.cp_CIoT_EPS_Optimisation_r13,1);
    memcpy(temp,listEle13.cp_CIoT_EPS_Optimisation_r13,sizeof(*(listEle13.cp_CIoT_EPS_Optimisation_r13)));

    temp=temp+sizeof(*(listEle13.cp_CIoT_EPS_Optimisation_r13));
    asn_long2INTEGER(listEle13.attachWithoutPDN_Connectivity_r13,1);
    memcpy(temp,listEle13.attachWithoutPDN_Connectivity_r13,sizeof(*(listEle13.attachWithoutPDN_Connectivity_r13)));

    temp=temp+sizeof(*(listEle13.attachWithoutPDN_Connectivity_r13));

    ASN_SEQUENCE_ADD(&sibType2.sib2.cIoT_EPS_OptimisationInfo_r13->list,&listEle13);


    sibType2.sib2.useFullResumeID_r13=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.useFullResumeID_r13,1);

    memcpy(temp,sibType2.sib2.useFullResumeID_r13,sizeof(*(sibType2.sib2.useFullResumeID_r13)));

    temp=temp+sizeof(*(sibType2.sib2.useFullResumeID_r13));
    sibType2.sib2.unicastFreqHoppingInd_r13=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.unicastFreqHoppingInd_r13,1);

    memcpy(temp,sibType2.sib2.unicastFreqHoppingInd_r13,sizeof(*(sibType2.sib2.unicastFreqHoppingInd_r13)));

    temp=temp+sizeof(*(sibType2.sib2.unicastFreqHoppingInd_r13));


    sibType2.sib2.mbsfn_SubframeConfigList_v1430=(struct MBSFN_SubframeConfigList_v1430 *)malloc(sizeof(struct MBSFN_SubframeConfigList_v1430));


    struct MBSFN_SubframeConfig_v1430 listEle14;

    listEle14.subframeAllocation_v1430.present=1;
    memcpy(temp,&listEle14.subframeAllocation_v1430.present,sizeof(listEle14.subframeAllocation_v1430.present));

    temp=temp+sizeof(listEle14.subframeAllocation_v1430.present);
    BIT_STRING_fromBuf(listEle14.subframeAllocation_v1430.choice.oneFrame_v1430,"11011",-1);

    memcpy(temp,&listEle14.subframeAllocation_v1430.choice.oneFrame_v1430,sizeof(BIT_STRING_t));

    temp=temp+sizeof(BIT_STRING_t);

    ASN_SEQUENCE_ADD(&sibType2.sib2.mbsfn_SubframeConfigList_v1430->list,&listEle14);

    sibType2.sib2.videoServiceCauseIndication_r14=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.videoServiceCauseIndication_r14,1);

    sibType2.sib2.plmn_InfoList_r15=(struct PLMN_InfoList_r15 *)malloc(sizeof(struct PLMN_InfoList_r15));    struct PLMN_Info_r15 listEle15;
    asn_long2INTEGER(listEle15.upperLayerIndication_r15,1);

    memcpy(temp,listEle15.upperLayerIndication_r15,sizeof(*(listEle15.upperLayerIndication_r15)));

    temp=temp+sizeof(*(listEle15.upperLayerIndication_r15));
    ASN_SEQUENCE_ADD(&sibType2.sib2.plmn_InfoList_r15->list,&listEle15);


    sibType2.sib2.cp_EDT_r15=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.cp_EDT_r15,1);

    memcpy(temp,sibType2.sib2.cp_EDT_r15,sizeof(*(sibType2.sib2.cp_EDT_r15)));

    temp=temp+sizeof(*(sibType2.sib2.cp_EDT_r15));

    sibType2.sib2.up_EDT_r15=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.up_EDT_r15,1);
    memcpy(temp,sibType2.sib2.up_EDT_r15,sizeof(*(sibType2.sib2.up_EDT_r15)));

    temp=temp+sizeof(*(sibType2.sib2.up_EDT_r15));

    sibType2.sib2.idleModeMeasurements_r15=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.idleModeMeasurements_r15,1);

    memcpy(temp,sibType2.sib2.idleModeMeasurements_r15,sizeof(*(sibType2.sib2.idleModeMeasurements_r15)));

    temp=temp+sizeof(*(sibType2.sib2.idleModeMeasurements_r15));

    sibType2.sib2.reducedCP_LatencyEnabled_r15=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.reducedCP_LatencyEnabled_r15,1);
    memcpy(temp,sibType2.sib2.reducedCP_LatencyEnabled_r15,sizeof(*(sibType2.sib2.reducedCP_LatencyEnabled_r15)));

    temp=temp+sizeof(*(sibType2.sib2.reducedCP_LatencyEnabled_r15));

    sibType2.sib2.mbms_ROM_ServiceIndication_r15=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.mbms_ROM_ServiceIndication_r15,1);

    memcpy(temp,sibType2.sib2.mbms_ROM_ServiceIndication_r15,sizeof(*(sibType2.sib2.mbms_ROM_ServiceIndication_r15)));

    temp=temp+sizeof(*(sibType2.sib2.mbms_ROM_ServiceIndication_r15));

    sibType2.sib2.rlos_Enabled_r16=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.rlos_Enabled_r16,1);

    memcpy(temp,sibType2.sib2.rlos_Enabled_r16,sizeof(*(sibType2.sib2.rlos_Enabled_r16)));

    temp=temp+sizeof(*(sibType2.sib2.rlos_Enabled_r16));

    sibType2.sib2.earlySecurityReactivation_r16=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.earlySecurityReactivation_r16,1);

    memcpy(temp,sibType2.sib2.earlySecurityReactivation_r16,sizeof(*(sibType2.sib2.earlySecurityReactivation_r16)));

    temp=temp+sizeof(*(sibType2.sib2.earlySecurityReactivation_r16));

    sibType2.sib2.cp_EDT_5GC_r16=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.cp_EDT_5GC_r16,1);

    memcpy(temp,sibType2.sib2.cp_EDT_5GC_r16,sizeof(*(sibType2.sib2.cp_EDT_5GC_r16)));

    temp=temp+sizeof(*(sibType2.sib2.cp_EDT_5GC_r16));
    sibType2.sib2.up_EDT_5GC_r16=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.up_EDT_5GC_r16,1);
    memcpy(temp,sibType2.sib2.up_EDT_5GC_r16,sizeof(*(sibType2.sib2.up_EDT_5GC_r16)));

    temp=temp+sizeof(*(sibType2.sib2.up_EDT_5GC_r16));

    sibType2.sib2.cp_PUR_EPC_r16=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.cp_PUR_EPC_r16,1);

    memcpy(temp,sibType2.sib2.cp_PUR_EPC_r16,sizeof(*(sibType2.sib2.cp_PUR_EPC_r16)));

    temp=temp+sizeof(*(sibType2.sib2.cp_PUR_EPC_r16));

    sibType2.sib2.up_PUR_EPC_r16=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.up_PUR_EPC_r16,1);
    memcpy(temp,sibType2.sib2.up_PUR_EPC_r16,sizeof(*(sibType2.sib2.up_PUR_EPC_r16)));

    temp=temp+sizeof(*(sibType2.sib2.up_PUR_EPC_r16));

    sibType2.sib2.cp_PUR_5GC_r16=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.cp_PUR_5GC_r16,1);

    memcpy(temp,sibType2.sib2.cp_PUR_5GC_r16,sizeof(*(sibType2.sib2.cp_PUR_5GC_r16)));

    temp=temp+sizeof(*(sibType2.sib2.cp_PUR_5GC_r16));

    sibType2.sib2.up_PUR_5GC_r16=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.up_PUR_5GC_r16,1);

    memcpy(temp,sibType2.sib2.up_PUR_5GC_r16,sizeof(*(sibType2.sib2.up_PUR_5GC_r16)));

    temp=temp+sizeof(*(sibType2.sib2.up_PUR_5GC_r16));

    sibType2.sib2.mpdcch_CQI_Reporting_r16=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.mpdcch_CQI_Reporting_r16,1);

    memcpy(temp,sibType2.sib2.mpdcch_CQI_Reporting_r16,sizeof(*(sibType2.sib2.mpdcch_CQI_Reporting_r16)));

    temp=temp+sizeof(*(sibType2.sib2.mpdcch_CQI_Reporting_r16));


    sibType2.sib2.rai_ActivationEnh_r16=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.rai_ActivationEnh_r16,1);

    memcpy(temp,sibType2.sib2.rai_ActivationEnh_r16,sizeof(*(sibType2.sib2.rai_ActivationEnh_r16)));

    temp=temp+sizeof(*(sibType2.sib2.rai_ActivationEnh_r16));

    sibType2.sib2.idleModeMeasurementsNR_r16=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(sibType2.sib2.idleModeMeasurementsNR_r16,1);

    memcpy(temp,sibType2.sib2.idleModeMeasurementsNR_r16,sizeof(*(sibType2.sib2.idleModeMeasurementsNR_r16)));

    temp=temp+sizeof(*(sibType2.sib2.idleModeMeasurementsNR_r16));

// end of filling sib2

    SendSystemInformationBlockType2(rrcSendSib2.content,phy_qid);

// filling of rrc connection setup 
    
      unsigned char *temp=rrcConnectionSetupMsg.content;
      
      rrcConnectionSetup.source=0x02;
      memcpy(temp,&rrcConnectionSetup.source,sizeof(rrcConnectionSetup.source));
      temp=temp+sizeof(rrcConnectionSetup.source);
      rrcConnectionSetup.dest=0x01;
      memcpy(temp,&rrcConnectionSetup.dest,sizeof(rrcConnectionSetup.dest));
      temp=temp+sizeof(rrcConnectionSetup.dest);
      rrcConnectionSetup.protocolType=0x01;
      memcpy(temp,&rrcConnectionSetup.protocolType,sizeof(rrcConnectionSetup.protocolType));
      temp=temp+sizeof(rrcConnectionSetup.protocolType);
      rrcConnectionSetup.messageType=SystemInformationBlockType2;
      memcpy(temp,&rrcConnectionSetup.messageType,sizeof(rrcConnectionSetup.messageType));
      temp=temp+sizeof(rrcConnectionSetup.messageType);
      rrcConnectionSetup.length=5+sizeof(RRCConnectionSetup_t);
      memcpy(temp,&rrcConnectionSetup.length,sizeof(rrcConnectionSetup.length));
      temp=temp+sizeof(rrcConnectionSetup.length);

      rrcConnectionSetup.rrc_TransactionIdentifier=1;
      memcpy(temp,&rrcConnectionSetup.rrc_TransactionIdentifier,sizeof(rrcConnectionSetup.rrc_TransactionIdentifier));
      temp=temp+sizeof(rrcConnectionSetup.rrc_TransactionIdentifier);

      rrcConnectionSetup.criticalExtensions.present=1;
      
      memcpy(temp,&rrcConnectionSetup.criticalExtensions.present,sizeof(rrcConnectionSetup.criticalExtensions.present));
      temp=temp+sizeof(rrcConnectionSetup.criticalExtensions.present);

      rrcConnectionSetup.criticalExtensions.choice.c1.present=1;
      
      memcpy(temp,&rrcConnectionSetup.criticalExtensions.choice.c1.present,sizeof(rrcConnectionSetup.criticalExtensions.choice.c1.present));
      temp=temp+sizeof(rrcConnectionSetup.criticalExtensions.choice.c1.present);

      rrcConnectionSetup.criticalExtensions.choice.c1.choice.rrcConnectionSetup_r8.radioResourceConfigDedicated.srb_ToAddModList=(struct SRB_ToAddModList *)malloc(sizeof(struct SRB_ToAddModList));

      struct SRB_ToAddMod rrcSetupListele1;
      rrcSetupListele1.srb_Identity=1;
      rrcSetupListele1.rlc_Config=(struct rlc_Config *)malloc(sizeof(struct rlc_Config));

      rrcSetupListele1.rlc_Config->present=1;
            
      memcpy(temp,&rrcSetupListele1.rlc_Config->present,sizeof(rrcSetupListele1.rlc_Config->present));
      temp=temp+sizeof(rrcSetupListele1.rlc_Config->present);

      rrcSetupListele1.rlc_Config->choice.explicitValue=1;
      
      memcpy(temp,&rrcSetupListele1.rlc_Config->choice.explicitValue,sizeof(rrcSetupListele1.rlc_Config->choice.explicitValue));
      temp=temp+sizeof(rrcSetupListele1.rlc_Config->choice.explicitValue);

      rrcSetupListele1.logicalChannelConfig=(struct logicalChannelConfig *)malloc(sizeof(struct logicalChannelConfig));

      rrcSetupListele1.logicalChannelConfig->present=1;
      
      memcpy(temp,&rrcSetupListele1.logicalChannelConfig->present,sizeof(rrcSetupListele1.logicalChannelConfig->present));
      temp=temp+sizeof(rrcSetupListele1.logicalChannelConfig->present);
      rrcSetupListele1.logicalChannelConfig->choice.explicitValue=1;
      memcpy(temp,&rrcSetupListele1.logicalChannelConfig->choice.explicitValue,sizeoft(rrcSetupListele1.logicalChannelConfig->choice.explicitValue));
      temp=temp+sizeof(rrcSetupListele1.logicalChannelConfig->choice.explicitValue);
      
      free(rrcSetupListele1.logicalChannelConfig);      

      rrcSetupListele1.pdcp_verChange_r15=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));

      asn_long2INTEGER(rrcSetupListele1.pdcp_verChange_r15,1);
    
      free(rrcSetupListele1.pdcp_verChange_r15);
      rrcSetupListele1.rlc_Config_v1530=(struct RLC_Config_v1530 *)malloc(sizeof(struct RLC_Config_v1530));
      rrcSetupListele1.rlc_Config_v1530->present=1;
      memcpy(temp,&rrcSetupListele1.rlc_Config_v1530->present,sizeoft(rrcSetupListele1.rlc_Config_v1530->present));
      temp=temp+sizeof(rrcSetupListele1.rlc_Config_v1530->present);

      asn_long2INTEGER(&rrcSetupListele1.rlc_Config_v1530->choice.setup.rlc_OutOfOrderDelivery_r15,1); 
      
      memcpy(temp,&rrcSetupListele1.rlc_Config_v1530->choice.setup.rlc_OutOfOrderDelivery_r15,sizeoft(rrcSetupListele1.rlc_Config_v1530->choice.setup.rlc_OutOfOrderDelivery_r15));
      temp=temp+sizeof(rrcSetupListele1.rlc_Config_v1530->choice.setup.rlc_OutOfOrderDelivery_r15);

      free(rrcSetupListele1.rlc_Config_v1530);
      rrcSetupListele1.rlc_BearerConfigSecondary_r15=(struct RLC_BearerConfig_r15 *)malloc(sizeof(struct RLC_BearerConfig_r15));

      rrcSetupListele1.rlc_BearerConfigSecondary_r15->present=1;
      
      memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->present,sizeoft(rrcSetupListele1.rlc_BearerConfigSecondary_r15->present));
      temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->present);

      rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15=(struct RLC_Config_r15 *)malloc(sizeof(struct RLC_Config_r15));
      rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.present=1;
      
      memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.present,sizeoft(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.present));
      temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.present);


      asn_long2INTEGER(&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.t_PollRetransmit_r15,1);
    
      
      memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15-     >mode_r15.choice.am_r15.ul_AM_RLC_r15.t_PollRetransmit_r15,sizeoft(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.t_PollRetransmit_r15));
      temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.t_PollRetransmit_r15);

    asn_long2INTEGER(&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.pollPDU_r15,1);

    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.pollPDU_r15,sizeoft(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.pollPDU_r15));

    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.pollPDU_r15);

   
    asn_long2INTEGER(&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.pollByte_r15,1);

    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.pollByte_r15,sizeoft(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.pollByte_r15));

    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.pollByte_r15);
    asn_long2INTEGER(&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.maxRetxThreshold_r15,1);

    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.maxRetxThreshold_r15,sizeoft(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.maxRetxThreshold_r15));

    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.maxRetxThreshold_r15);


    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.extended_RLC_LI_Field_r15=1;

    
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.extended_RLC_LI_Field_r15,sizeoft(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.extended_RLC_LI_Field_r15));

    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.extended_RLC_LI_Field_r15);
    
   asn_long2INTEGER(&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.dl_AM_RLC_r15.t_Reordering_r15,1);

    
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.t_Reordering_r15,sizeoft(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.t_Reordering_r15));

    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.ul_AM_RLC_r15.t_Reordering_r15);

    
   asn_long2INTEGER(&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.dl_AM_RLC_r15.t_StatusProhibit_r15,1);

    
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.dl_AM_RLC_r15.t_StatusProhibit_r15,sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.dl_AM_RLC_r15.t_StatusProhibit_r15));

    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.dl_AM_RLC_r15.t_StatusProhibit_r15);
    
   rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.dl_AM_RLC_r15.extended_RLC_LI_Field_r15=1;

    
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.dl_AM_RLC_r15.extended_RLC_LI_Field_r15,sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.dl_AM_RLC_r15.extended_RLC_LI_Field_r15));

    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->mode_r15.choice.am_r15.dl_AM_RLC_r15.extended_RLC_LI_Field_r15);

    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->reestablishRLC_r15=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));

    asn_long2INTEGER(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->reestablishRLC_r15,1);
       
    memcpy(temp,rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->reestablishRLC_r15,sizeof(*(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->reestablishRLC_r15)));
    temp=temp+sizeof(*(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->reestablishRLC_r15));
    
    free(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->reestablishRLC_r15);

    
    asn_long2INTEGER(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->rlc_OutOfOrderDelivery_r15,1);
       
    memcpy(temp,rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->rlc_OutOfOrderDelivery_r15,sizeof(*(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->rlc_OutOfOrderDelivery_r15)));
    temp=temp+sizeof(*(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->rlc_OutOfOrderDelivery_r15));
    
    free(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15->rlc_OutOfOrderDelivery_r15);

   free(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup->rlc_Config_r15);

   
   rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelIdentityConfig_r15.present=1;
       
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelIdentityConfig_r15.present,sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelIdentityConfig_r15.present));
    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelIdentityConfig_r15.present);

  rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelIdentityConfig_r15.choice.logicalChannelIdentity_r15=1;
  
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelIdentityConfig_r15.choice.logicalChannelIdentity_r15,sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelIdentityConfig_r15.choice.logicalChannelIdentity_r15));
    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelIdentityConfig_r15.choice.logicalChannelIdentity_r15);

  rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15=(struct LogicalChannelConfig *)malloc(sizeof(struct LogicalChannelConfig)); 

   rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters=(struct ul_SpecificParameters *)malloc(sizeof(struct ul_SpecificParameters *));

  rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->priority=1;

    
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->priority,sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->priority));
    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->priority);

    
  asn_long2INTEGER(&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->prioritisedBitRate,1);

    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->prioritisedBitRate,sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->prioritisedBitRate));
    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->prioritisedBitRate);

    
  asn_long2INTEGER(&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->bucketSizeDuration,1);

    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->bucketSizeDuration,sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->bucketSizeDuration));
    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->bucketSizeDuration);

  rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->bucketSizeDuration->logicalChannelGroup=(long *)malloc(sizeof(long));

  *rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->bucketSizeDuration->logicalChannelGroup=1;

    memcpy(temp,rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->bucketSizeDuration->logicalChannelGroup,sizeof(*(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->bucketSizeDuration->logicalChannelGroup)));
    temp=temp+sizeof(*(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->bucketSizeDuration->logicalChannelGroup));

    free(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters->bucketSizeDuration->logicalChannelGroup);
  
    free(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->ul_SpecificParameters);

    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Mask_r9=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));

    asn_long2INTEGER(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Mask_r9,1);
  
    
    memcpy(temp,rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Mask_r9,rsizeofp(*(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Mask_r9)));
    temp=temp+sizeof(*(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Mask_r9));

  free(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Mask_r9);


    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Prohibit_r12=(BOOLEAN_t *)malloc(sizeof(BOOLEAN_t));

    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Prohibit_r12=1;
  
    
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Prohibit_r12,rsizeofp(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Prohibit_r12));
    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Prohibit_r12);

    free(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Prohibit_r12);

    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->laa_UL_Allowed_r14=(BOOLEAN_t *)malloc(sizeof(BOOLEAN_t));

    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->laa_UL_Allowed_r14=1;
  
    
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->laa_UL_Allowed_r14,sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->laa_UL_Allowed_r14));
    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->laa_UL_Allowed_r14);

    free(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->laa_UL_Allowed_r14);


    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Mask_r9=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));

    asn_long2INTEGER(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Mask_r9,1);

    
    memcpy(temp,rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Mask_r9,sizeof(*(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Mask_r9)));
    temp=temp+sizeof(*(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Mask_r9));

    free(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->logicalChannelSR_Mask_r9);

    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->allowedTTI_Lengths_r15=(struct allowedTTI_Lengths_r15 *)malloc(sizeof(struct allowedTTI_Lengths_r15));

    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->allowedTTI_Lengths_r15->present=1;

    
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->allowedTTI_Lengths_r15->present,sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->allowedTTI_Lengths_r15->present));
    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->allowedTTI_Lengths_r15->present);

    
    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->allowedTTI_Lengths_r15->choice.setup.shortTTI_r15=1;

    
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->allowedTTI_Lengths_r15->choice.setup.shortTTI_r15,sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->choice.setup.shortTTI_r15));
    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->allowedTTI_Lengths_r15->choice.setup.shortTTI_r15);
    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->allowedTTI_Lengths_r15->choice.setup.subframeTTI_r15=1;

    
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->allowedTTI_Lengths_r15->choice.setup.subframeTTI_r15,sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->choice.setup.subframeTTI_r15));
    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->allowedTTI_Lengths_r15->choice.setup.subframeTTI_r15);
    
    free(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15->allowedTTI_Lengths_r15);


    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.logicalChannelSR_Restriction_r15=(struct logicalChannelSR_Restriction_r15 *)malloc(sizeof(struct logicalChannelSR_Restriction_r15));

    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.logicalChannelSR_Restriction_r15->present=1;

    
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.logicalChannelSR_Restriction_r15->present,sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.logicalChannelSR_Restriction_r15->present));
    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.logicalChannelSR_Restriction_r15->present);

    asn_long2INTEGER(&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.logicalChannelSR_Restriction_r15->choice.setup,1);
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.logicalChannelSR_Restriction_r15->choice.setup,sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.logicalChannelSR_Restriction_r15->choice.setup));
    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelCon fig_r15.logicalChannelSR_Restriction_r15->choice.setup);

    free(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.logicalChannelSR_Restriction_r15);
    
    
    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.logicalChannelSR_Restriction_r15=(struct logicalChannelSR_Restriction_r15 *)malloc(sizeof(struct logicalChannelSR_Restriction_r15));

    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.logicalChannelSR_Restriction_r15->present=1;

    
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.channelAccessPriority_r15->present,sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.channelAccessPriority_r15->present));
    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.channelAccessPriority_r15->present);

    asn_long2INTEGER(&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.channelAccessPriority_r15->choice.setup,1);
    memcpy(temp,&rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.channelAccessPriority_r15->choice.setup,sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.channelAccessPriority_r15->choice.setup));
    temp=temp+sizeof(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelCon fig_r15.channelAccessPriority_r15->choice.setup);

    free(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.channelAccessPriority_r15);

    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.lch_CellRestriction_r15=(BIT_STRING_t *)malloc(sizeof(BIT_STRING_t));
    BIT_STRING_fromBuf(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.lch_CellRestriction_r15,"11011",-1);
    memcpy(temp,rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.lch_CellRestriction_r15,sizeof(BIT_STRING_t));

    temp=temp+sizeof(BIT_STRING_t);
    free(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.lch_CellRestriction_r15);
    rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.bitRateMultiplier_r16=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));
    asn_long2INTEGER(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.bitRateMultiplier_r16,1);
    memcpy(temp,rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.bitRateMultiplier_r16,sizeof(ENUMERATED_t));

    temp=temp+sizeof(ENUMERATED_t);
    free(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15.bitRateMultiplier_r16);

    free(rrcSetupListele1.rlc_BearerConfigSecondary_r15->choice.setup.logicalChannelConfig_r15);

    free(rrcSetupListele1.rlc_BearerConfigSecondary_r15);

    rrcSetupListele1.srb_Identity_v1530=(long *)malloc(sizeof(long));
  
    *rrcSetupListele1.srb_Identity_v1530=1;
  
    memcpy(temp,rrcSetupListele1.srb_Identity_v1530,sizeof(long));

    temp=temp+sizeof(long);

    free(rrcSetupListele1.srb_Identity_v1530);

    rrcSetupListele1.rlc_Config_v1560=(struct RLC_Config_v1510 *)malloc(sizeof(struct RLC_Config_v1510));

    asn_long2INTEGER(rrcSetupListele1.rlc_Config_v1560->reestablishRLC_r15,1);

    memcpy(temp,&rrcSetupListele1.rlc_Config_v1560->reestablishRLC_r15,sizeof(rrcSetupListele1.rlc_Config_v1560->reestablishRLC_r15));

    temp=temp+sizeof(rrcSetupListele1.rlc_Config_v1560->reestablishRLC_r15);

    free(rrcSetupListele1.rlc_Config_v1560);

    
    ASN_SEQUENCE_ADD(&rrcConnectionSetup.criticalExtensions.choice.c1.choice.rrcConnectionSetup_r8.radioResourceConfigDedicated.srb_ToAddModList->list,&rrcSetupListele1);
    free(rrcConnectionSetup.criticalExtensions.choice.c1.choice.rrcConnectionSetup_r8.radioResourceConfigDedicated.srb_ToAddModList);

    rrcConnectionSetup.criticalExtensions.choice.c1.choice.rrcConnectionSetup_r8.radioResourceC     onfigDedicated.drb_ToAddModList=(struct DRB_ToAddModList *)malloc(sizeof(struct DRB_ToAddModList));

    struct DRB_ToAddMod rrcSetupListele2;
    rrcSetupListele2.eps_BearerIdentity=(long *)malloc(sizeof(long));
    *rrcSetupListele2.eps_BearerIdentity=1;
    
    
    memcpy(temp,rrcSetupListele2.eps_BearerIdentity,sizeof(*(rrcSetupListele2.eps_BearerIdentity)));

    temp=temp+sizeof(*(rrcSetupListele2.eps_BearerIdentity));

    free(rrcSetupListele2.eps_BearerIdentity);

    rrcSetupListele2.drb_Identity=1;
    

    memcpy(temp,&rrcSetupListele2.drb_Identity,sizeof(rrcSetupListele2.drb_Identity));

    temp=temp+sizeof(rrcSetupListele2.drb_Identity);

    rrcSetupListele2.pdcp_Config=(struct PDCP_Config *)malloc(sizeof(struct PDCP_Config));

    rrcSetupListele2.pdcp_Config->discardTimer=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));

    asn_long2INTEGER(rrcSetupListele2.pdcp_Config->discardTimer,1);
    
    memcpy(temp,rrcSetupListele2.pdcp_Config->discardTimer,sizeof(ENUMERATED_t));

    temp=temp+sizeof(ENUMERATED_t);

    free(rrcSetupListele2.pdcp_Config->discardTimer);
    
    rrcSetupListele2.pdcp_Config->rlc_AM=(struct rlc_AM *)malloc(sizeof(struct rlc_AM));

    rrcSetupListele2.pdcp_Config->rlc_AM->statusReportRequired=1;
    
    memcpy(temp,&rrcSetupListele2.pdcp_Config->rlc_AM->statusReportRequired,sizeof(rrcSetupListele2.pdcp_Config->rlc_AM->statusReportRequired));

    temp=temp+sizeof(rrcSetupListele2.pdcp_Config->rlc_AM->statusReportRequired);
    
    free(rrcSetupListele2.pdcp_Config->rlc_AM);

   
    rrcSetupListele2.pdcp_Config->rlc_UM=(struct rlc_UM *)malloc(sizeof(struct rlc_UM));

    asn_long2INTEGER(rrcSetupListele2.pdcp_Config->rlc_UM->pdcp_SN_Size,1);

    
    memcpy(temp,rrcSetupListele2.pdcp_Config->rlc_UM->pdcp_SN_Size,sizeof(ENUMERATED_t));

    temp=temp+sizeof(ENUMERATED_t);

    free(rrcSetupListele2.pdcp_Config->rlc_UM);


    rrcSetupListele2.pdcp_Config.headerCompression.present=1;

    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.headerCompression.present,sizeof(rrcSetupListele2.pdcp_Config.headerCompression.present));

    temp=temp+sizeof(rrcSetupListele2.pdcp_Config.headerCompression.present);

    rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.maxCID=(long *)malloc(sizeof(long));

    *rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.maxCID=1;
    
    
    memcpy(temp,rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.maxCID,sizeof(long));

    temp=temp+sizeof(long);

    free(rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.maxCID);

    rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0001=1;

    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0001,sizeof(BOOLEAN_t));

    temp=temp+sizeof(BOOLEAN_t);


    rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0002=1;

    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0002,sizeof(BOOLEAN_t));

    temp=temp+sizeof(BOOLEAN_t);


    rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0003=1;

    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0003,sizeof(BOOLEAN_t));

    temp=temp+sizeof(BOOLEAN_t);


    rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0004=1;

    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0004,sizeof(BOOLEAN_t));

    temp=temp+sizeof(BOOLEAN_t);
    rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0005=1;

    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0005,sizeof(BOOLEAN_t));

    temp=temp+sizeof(BOOLEAN_t);
    rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0005=1;

    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0005,sizeof(BOOLEAN_t));

    temp=temp+sizeof(BOOLEAN_t);
    rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0006=1;

    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0006,sizeof(BOOLEAN_t));

    temp=temp+sizeof(BOOLEAN_t);

    rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0101=1;

    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0101,sizeof(BOOLEAN_t));

    temp=temp+sizeof(BOOLEAN_t);
    rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0102=1;

    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0102,sizeof(BOOLEAN_t));

    temp=temp+sizeof(BOOLEAN_t);
    rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0103=1;

    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0103,sizeof(BOOLEAN_t));

    temp=temp+sizeof(BOOLEAN_t);
    rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0104=1;

    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.headerCompression.choice.rohc.profiles.profile0x0104,sizeof(BOOLEAN_t));

    temp=temp+sizeof(BOOLEAN_t);

    rrcSetupListele2.pdcp_Config.rn_IntegrityProtection_r10=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));

    asn_long2INTEGER(rrcSetupListele2.pdcp_Config.rn_IntegrityProtection_r10,1);
    
    memcpy(temp,rrcSetupListele2.pdcp_Config.rn_IntegrityProtection_r10,sizeof(ENUMERATED_t));

    temp=temp+sizeof(ENUMERATED_t);

    free(rrcSetupListele2.pdcp_Config.rn_IntegrityProtection_r10);
    rrcSetupListele2.pdcp_Config.pdcp_SN_Size_v1130=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));

    asn_long2INTEGER(rrcSetupListele2.pdcp_Config.pdcp_SN_Size_v1130,1);
    
    memcpy(temp,rrcSetupListele2.pdcp_Config.pdcp_SN_Size_v1130,sizeof(ENUMERATED_t));

    temp=temp+sizeof(ENUMERATED_t);

    free(rrcSetupListele2.pdcp_Config.pdcp_SN_Size_v1130);

    rrcSetupListele2.pdcp_Config.ul_DataSplitDRB_ViaSCG_r12=(BOOLEAN_t *)malloc(sizeof(BOOLEAN_t));

    *rrcSetupListele2.pdcp_Config.ul_DataSplitDRB_ViaSCG_r12=1;
    
    memcpy(temp,rrcSetupListele2.pdcp_Config.ul_DataSplitDRB_ViaSCG_r12,sizeof(BOOLEAN_t));

    temp=temp+sizeof(BOOLEAN_t);

    free(rrcSetupListele2.pdcp_Config.ul_DataSplitDRB_ViaSCG_r12);
    rrcSetupListele2.pdcp_Config.t_Reordering_r12=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));

    asn_long2INTEGER(rrcSetupListele2.pdcp_Config.t_Reordering_r12,1);
    
    memcpy(temp,rrcSetupListele2.pdcp_Config.t_Reordering_r12,sizeof(ENUMERATED_t));

    temp=temp+sizeof(ENUMERATED_t);

    free(rrcSetupListele2.pdcp_Config.t_Reordering_r12);

    rrcSetupListele2.pdcp_Config.ul_DataSplitThreshold_r13=(struct ul_DataSplitThreshold_r13 *)malloc(sizeof(struct ul_DataSplitThreshold_r13));
    
    rrcSetupListele2.pdcp_Config.ul_DataSplitThreshold_r13->present=1;
    
    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.ul_DataSplitThreshold_r13->present,sizeof(rrcSetupListele2.pdcp_Config.ul_DataSplitThreshold_r13->present));

    temp=temp+sizeof(rrcSetupListele2.pdcp_Config.ul_DataSplitThreshold_r13->present);

    asn_long2INTEGER(&rrcSetupListele2.pdcp_Config.ul_DataSplitThreshold_r13->choice.setup,1);
    memcpy(temp,&rrcSetupListele2.pdcp_Config.ul_DataSplitThreshold_r13->choice,sizeof(rrcSetupListele2.pdcp_Config.ul_DataSplitThreshold_r13->choice));

    temp=temp+sizeof(rrcSetupListele2.pdcp_Config.ul_DataSplitThreshold_r13->choice);

    free(rrcSetupListele2.pdcp_Config.ul_DataSplitThreshold_r13);

    rrcSetupListele2.pdcp_Config.pdcp_SN_Size_v1310=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));

    asn_long2INTEGER(rrcSetupListele2.pdcp_Config.pdcp_SN_Size_v1310,1);
    
    memcpy(temp,rrcSetupListele2.pdcp_Config.pdcp_SN_Size_v1310,sizeof(ENUMERATED_t));

    temp=temp+sizeof(ENUMERATED_t);

    free(rrcSetupListele2.pdcp_Config.pdcp_SN_Size_v1310);

    rrcSetupListele2.pdcp_Config.statusFeedback_r13=(struct statusFeedback_r13 *)malloc(sizeof(struct statusFeedback_r13));

    rrcSetupListele2.pdcp_Config.statusFeedback_r13->present=1;

    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.statusFeedback_r13->present,sizeof(rrcSetupListele2.pdcp_Config.statusFeedback_r13->present));

    temp=temp+sizeof(rrcSetupListele2.pdcp_Config.statusFeedback_r13->present);

    rrcSetupListele2.pdcp_Config.statusFeedback_r13->choice.setup.statusPDU_TypeForPolling_r13=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));


    
    memcpy(temp,rrcSetupListele2.pdcp_Config.statusFeedback_r13->choice.setup.statusPDU_TypeForPolling_r13,sizeof(ENUMERATED_t));

    temp=temp+sizeof(ENUMERATED_t);

    free(rrcSetupListele2.pdcp_Config.statusFeedback_r13->choice.setup.statusPDU_TypeForPolling_r13);

    rrcSetupListele2.pdcp_Config.statusFeedback_r13->choice.setup.statusPDU_Periodicity_Type1_r13=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));


    
    memcpy(temp,rrcSetupListele2.pdcp_Config.statusFeedback_r13->choice.setup.statusPDU_Periodicity_Type1_r13,sizeof(ENUMERATED_t));

    temp=temp+sizeof(ENUMERATED_t);

    free(rrcSetupListele2.pdcp_Config.statusFeedback_r13->choice.setup.statusPDU_Periodicity_Type1_r13);

    rrcSetupListele2.pdcp_Config.statusFeedback_r13->choice.setup.=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));


    
    memcpy(temp,rrcSetupListele2.pdcp_Config.statusFeedback_r13->choice.setup.statusPDU_Periodicity_Type2_r13,sizeof(ENUMERATED_t));

    temp=temp+sizeof(ENUMERATED_t);

    free(rrcSetupListele2.pdcp_Config.statusFeedback_r13->choice.setup.statusPDU_Periodicity_Type2_r13);

    rrcSetupListele2.pdcp_Config.statusFeedback_r13->choice.setup.statusPDU_Periodicity_Offset_r13=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));


    
    memcpy(temp,rrcSetupListele2.pdcp_Config.statusFeedback_r13->choice.setup.statusPDU_Periodicity_Offset_r13,sizeof(ENUMERATED_t));

    temp=temp+sizeof(ENUMERATED_t);

    free(rrcSetupListele2.pdcp_Config.statusFeedback_r13->choice.setup.statusPDU_Periodicity_Offset_r13);

    free(rrcSetupListele2.pdcp_Config.statusFeedback_r13);

    rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14=(struct ul_LWA_Config_r14 *)malloc(sizeof(struct ul_LWA_Config_r14));

    rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14->present=1;
    

    memcpy(temp,&rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14->present,sizeof(rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14->present));

    temp=temp+sizeof(rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14->present);

    rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14->choice.setup.ul_LWA_DRB_ViaWLAN_r14=1;
    
    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14->choice.setup.ul_LWA_DRB_ViaWLAN_r14,sizeof(rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14->choice.setup.ul_LWA_DRB_ViaWLAN_r14));

    temp=temp+sizeof(rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14->choice.setup.ul_LWA_DRB_ViaWLAN_r14);
    
    rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14->choice.setup.ul_LWA_DataSplitThreshold_r14=(ENUMERATED_t *)malloc(sizeof(ENUMERATED_t));

    asn_long2INTEGER(rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14->choice.setup.ul_LWA_DataSplitThreshold_r14,1);

    
    memcpy(temp,rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14->choice.setup.ul_LWA_DataSplitThreshold_r14,sizeof(rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14->choice.setup.ul_LWA_DataSplitThreshold_r14));

    temp=temp+sizeof(rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14->choice.setup.ul_LWA_DataSplitThreshold_r14);

    free(rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14->choice.setup.ul_LWA_DataSplitThreshold_r14);
    
    free(rrcSetupListele2.pdcp_Config.ul_LWA_Config_r14);

    rrcSetupListele2.pdcp_Config.uplinkOnlyHeaderCompression_r14=(struct uplinkOnlyHeaderCompression_r14 *)malloc(sizeof(struct uplinkOnlyHeaderCompression_r14));

    rrcSetupListele2.pdcp_Config.uplinkOnlyHeaderCompression_r14->present=1;
    
    memcpy(temp,&rrcSetupListele2.pdcp_Config.uplinkOnlyHeaderCompression_r14->present,sizeof(rrcSetupListele2.pdcp_Config.uplinkOnlyHeaderCompression_r14->present));

    temp=temp+sizeof(rrcSetupListele2.pdcp_Config.uplinkOnlyHeaderCompression_r14->present);
    
  }

}

void SendSystemInformationBlockType2(unsigned char *rrcSendSib2Content,int phy_qid)
{


    if(msgsnd(phy_qid,rrcSendSib2Content,sizeof(rrcSendSib2Content), 0) < 0) // last argu is 0   
    {
        perror("eNodeB RRC - msgsnd failed");
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
  if(msgsnd(phy_qid, &response, sizeof(response), 0) < 0) // last argu is 0 as wait else IPC_NOWAIT
    {
      perror("eNodeB RRC - msgsnd failed"); 
    }
  
  
  
  
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
#if 0
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
#endif    
      write(sendConnected, &rcvMsg, sizeof(rcvMsg));
    
    }
//    write(sendConnected, &rcvMsg, sizeof(rcvMsg));
}	
