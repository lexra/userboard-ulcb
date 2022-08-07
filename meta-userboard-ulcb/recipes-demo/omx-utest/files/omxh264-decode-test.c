#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>

#include <OMX_Component.h>
#include <OMX_Core.h>

#pragma pack(1)

#define VIDEO_DEC_H264_ROLE "video_decoder.avc"

#define OMX_MAX_STRINGNAME_SIZE 128

/*Mag OMX IL Version definitions*/
#define kVersionMajor           1
#define kVersionMinor           2
#define kVersionRevision        0
#define kVersionStep 			0

#define BUFFER_IN_SIZE 4096
#define BUFFER_OUT_SIZE 2*8192

#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 360

#define OMX_IL_INPUT_BUFFER_NUMBER 2
#define OMX_IL_INPUT_BUFFER_SIZE 6291456/2

int first_packet = 1;

static const char* VA_RAW_MIME_TYPE = "video/x-raw-va";


OMX_STATETYPE gVdecState = OMX_StateMax;

static FILE *gTestFile = NULL;

typedef struct {
	OMX_BUFFERHEADERTYPE *bufHeader;
	OMX_U32 bufIndex;
	OMX_BOOL isBusy;
}Buffer_t;

static Buffer_t inBuffer[2], outBuffer[7];

static OMX_BOOL gIsStopped = OMX_FALSE;
static OMX_BOOL gIsFileEnded = OMX_FALSE;


static OMX_ERRORTYPE OMXDecoder_emptyBufferDone
    (OMX_HANDLETYPE component, OMX_PTR appData, OMX_BUFFERHEADERTYPE *buffer);
static OMX_ERRORTYPE OMXDecoder_eventHandler
    (OMX_HANDLETYPE component, OMX_PTR appData, OMX_EVENTTYPE event,
        OMX_U32 data1, OMX_U32 data2, OMX_PTR eventData);
static OMX_ERRORTYPE OMXDecoder_fillBufferDone
    (OMX_HANDLETYPE component, OMX_PTR appData, OMX_BUFFERHEADERTYPE *buffer);


pthread_mutex_t lock;

OMX_U8 data[OMX_IL_INPUT_BUFFER_SIZE];
size_t read_number;

int main()
{
	printf("START\n");

	//gTestFile = fopen("frame.h264","r");
	gTestFile = fopen("/usr/share/tests/test.h264","r");
	if (gTestFile == NULL){
		printf("Failed to open the file: test.h264");
	    return 1;
	}
	
	if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    if (OMX_ErrorNone != OMX_Init()) {
        fprintf(stderr, "OMX init failed\n");
        return 1;
    }
    
    printf("OMX initialized\n");
    
    
    printf("Getting OMX enum\n");

    int index = 0;
    char comp[OMX_MAX_STRINGNAME_SIZE];
    uint32_t numRoles;
    while (OMX_ErrorNoMore != OMX_ComponentNameEnum(
                                comp,
                                OMX_MAX_STRINGNAME_SIZE,
                                index))
    {
        printf("Component \"%s\" with index %d got\n", comp, index);
        index++;
        
        printf("Getting roles of component \"%s\"\n", comp);
        
        if(OMX_ErrorNone != OMX_GetRolesOfComponent(comp, &numRoles, NULL))
        {
            fprintf(stderr, "Can't get count of roles of component\"%s\"\n", comp);
            continue;
        }
        
        printf("Component \"%s\" has %d roles\n", comp, numRoles);
        
        
        if (numRoles > 0)
        {
            OMX_U8 **roles = (OMX_U8**) malloc(numRoles*sizeof(roles));
            for (OMX_U32 i = 0; i < numRoles; ++i) {
                roles[i] = (OMX_U8*) malloc(OMX_MAX_STRINGNAME_SIZE*sizeof(OMX_U8));
            }
            
            if(OMX_ErrorNone != OMX_GetRolesOfComponent(comp, &numRoles, roles))
            {
                fprintf(stderr, "Can't get roles for component \"%s\"\n", comp);
                
                for (OMX_U32 i = 0; i < numRoles; ++i) {
                    free(roles[i]);
                }
                free(roles);
                
                continue;
            }
            
            printf("Component roles:\n");
            
            for (OMX_U32 i = 0; i < numRoles; ++i) {
                printf("Role %d is \"%s\"\n", i, roles[i]);
                free(roles[i]);
            }
            free(roles);
        }
    }
    
    printf("No more components left\n");
    
    char componentName[] = "OMX.RENESAS.VIDEO.DECODER.H264";
    OMX_HANDLETYPE component = NULL;
    void *appData = NULL;
    OMX_CALLBACKTYPE callbacks = {
        OMXDecoder_eventHandler,
        OMXDecoder_emptyBufferDone,
        OMXDecoder_fillBufferDone
    };
    
    if (OMX_ErrorNone != OMX_GetHandle(
            &component,
            componentName,
            appData,
            &callbacks))
    {
        fprintf(stderr, "OMX_GetHandle failed\n");
        return 1;
    }
    
    printf("OMX handle got: %d\n", component);
    
    
    OMX_STATETYPE compState;
	if(OMX_ErrorNone != OMX_GetState(component, &compState)) {
		printf("Error during getting component state\n");
	} else {
		printf("STATE: %d\n", compState);
	}
	
	OMX_PARAM_COMPONENTROLETYPE sComponentRole;
	memset(&sComponentRole, 0, sizeof(OMX_PARAM_COMPONENTROLETYPE));
    sComponentRole.nSize = sizeof(OMX_PARAM_COMPONENTROLETYPE);
    sComponentRole.nVersion.nVersion = OMX_VERSION;
    
	OMX_ERRORTYPE err = OMX_GetParameter(component, OMX_IndexParamStandardComponentRole, &sComponentRole);
	
	printf("Default component role: %s\n", sComponentRole.cRole);
	
//	switch(paramPortVideo.format.video.eCompressionFormat){
//     case OMX_VIDEO_CodingAVC:
//       strcpy((char*)&sComponentRole.cRole[0], VIDEO_DEC_H264_ROLE);
//       break;
//     default :
//       DEBUG(DEB_LEV_ERR,"Error only MPEG4 and AVC(h.264) are supported %08x\n",paramPortVideo.format.video.eCompressionFormat); 
//	}
//     err = OMX_SetParameter(component, OMX_IndexParamStandardComponentRole, &sComponentRole);
//     if(err != OMX_ErrorNone) {
//       printf("\n error in input video format - exiting\n");
//       exit(1);
//     }
        
    OMX_PORT_PARAM_TYPE vDecPortParam;
    memset(&vDecPortParam, 0, sizeof(OMX_PORT_PARAM_TYPE));
    vDecPortParam.nSize = sizeof(OMX_PORT_PARAM_TYPE);
    vDecPortParam.nVersion.nVersion = OMX_VERSION;
    
    if(OMX_ErrorNone != OMX_GetParameter(component, OMX_IndexParamVideoInit, &vDecPortParam)){
	    printf("Error in getting vDec OMX_PORT_PARAM_TYPE parameter\n");
	    return 1;
    }

    printf("get vdec component param(OMX_IndexParamVideoInit): StartPortNumber-%d, Ports-%d\n", vDecPortParam.nStartPortNumber, vDecPortParam.nPorts);
    
    OMX_PARAM_PORTDEFINITIONTYPE sPortDef;
    memset(&sPortDef, 0, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
    sPortDef.nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
    sPortDef.nVersion.nVersion = OMX_VERSION;
    
    
    uint32_t inputPort = 0, outputPort = 0;
    uint32_t inputBuffCount, outputBuffCount, inputBuffSize, outputBuffSize;
    for (uint32_t i = vDecPortParam.nStartPortNumber; i < vDecPortParam.nPorts; i++){
    	sPortDef.nPortIndex = i;
    	
    	OMX_ERRORTYPE err = OMX_GetParameter(component, OMX_IndexParamPortDefinition, &sPortDef);
    	if(OMX_ErrorNone != err){
		    printf("Error in getting OMX_PARAM_PORTDEFINITIONTYPE %d parameter, error: 0x%x\n", i, err);
		    exit(1);
	    }

		printf("sPortDef.eDir: %d\n", sPortDef.eDir);
		printf("sPortDef.nBufferCountActual: %d\n", sPortDef.nBufferCountActual );
		printf("sPortDef.nBufferCountMin: %d\n", sPortDef.nBufferCountMin );
		printf("sPortDef.nBufferSize: %d\n", sPortDef.nBufferSize);
		
	    if (sPortDef.eDir == OMX_DirInput){
	    	printf("OMX_DirInput\n");
	    	//sPortDef.nBufferCountActual = OMX_IL_INPUT_BUFFER_NUMBER;
	    	//sPortDef.nBufferSize = BUFFER_OUT_SIZE;
	    	inputPort = i;
	    	inputBuffCount = sPortDef.nBufferCountActual;
	    	inputBuffSize = sPortDef.nBufferSize;
	    	
	    	sPortDef.nPortIndex = i;
      	    sPortDef.eDomain = OMX_PortDomainVideo;
    	    sPortDef.bBuffersContiguous = OMX_FALSE;
    	    sPortDef.bEnabled = OMX_TRUE;
    	    sPortDef.bPopulated = OMX_FALSE;
    	    sPortDef.nBufferAlignment = 0;
    	   
    	    sPortDef.format.video.nBitrate = 64000;
    	    sPortDef.format.video.xFramerate = 0;
            sPortDef.format.video.nFrameWidth = 360;//VIDEO_WIDTH;
            sPortDef.format.video.nFrameHeight = 640;//VIDEO_HEIGHT;
            sPortDef.format.video.nSliceHeight = 144;//sPortDef.format.video.nFrameHeight;
            sPortDef.format.video.nStride = 176;//sPortDef.format.video.nFrameWidth;
            sPortDef.format.video.eCompressionFormat = OMX_VIDEO_CodingAVC;
            sPortDef.format.video.eColorFormat = OMX_COLOR_FormatUnused;
            sPortDef.format.video.pNativeRender = NULL;
            sPortDef.format.video.pNativeWindow = NULL;
		} else {
		    outputPort = i;
		    outputBuffCount = sPortDef.nBufferCountActual = 7;
	    	outputBuffSize = sPortDef.nBufferSize = 345600;
	    	
	    	sPortDef.nPortIndex = i;
	    	sPortDef.eDomain = OMX_PortDomainVideo;
	    	sPortDef.bBuffersContiguous = OMX_FALSE;
	    	sPortDef.bEnabled = OMX_TRUE;
	    	sPortDef.bPopulated = OMX_FALSE;
	    	sPortDef.nBufferAlignment = 0;
	    	
	    	sPortDef.format.video.nBitrate = 0;
	    	sPortDef.format.video.xFramerate = 0;
            sPortDef.format.video.nFrameWidth = 640;//144;//VIDEO_WIDTH;
            sPortDef.format.video.nFrameHeight = 360;//176;//VIDEO_HEIGHT;
            sPortDef.format.video.nSliceHeight = sPortDef.format.video.nFrameHeight;
            sPortDef.format.video.nStride = 640;//192;
            sPortDef.format.video.eCompressionFormat = OMX_VIDEO_CodingUnused;
            sPortDef.format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
            sPortDef.format.video.bFlagErrorConcealment = OMX_FALSE;
            sPortDef.format.video.pNativeRender = NULL;
            sPortDef.format.video.pNativeWindow = NULL;
           
		}
		
		err = OMX_SetParameter(component, OMX_IndexParamPortDefinition, &sPortDef);
		if(OMX_ErrorNone != err){
			printf("Error in setting OMX_PARAM_PORTDEFINITIONTYPE %d parameter, error: 0x%x\n", i, err);
			//exit(1);
		} else {
		    printf("Success in setting OMX_PARAM_PORTDEFINITIONTYPE %d parameter\n", i);
		}
		
		if(OMX_ErrorNone != OMX_SendCommand  ( component, OMX_CommandPortEnable, i, NULL )) {
			printf("Error enable port\n");
		}
	}
	
	OMX_VIDEO_PARAM_PORTFORMATTYPE format1;
    memset(&format1, 0, sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE));
    format1.nSize = sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE);
    format1.nVersion.nVersion = OMX_VERSION;
    format1.nIndex = 0;
    format1.nPortIndex = inputPort;
    format1.eCompressionFormat = OMX_VIDEO_CodingAVC;
	format1.eColorFormat = OMX_COLOR_FormatUnused;
	format1.xFramerate = 983040;
	
	if(OMX_SetParameter(component, OMX_IndexParamVideoPortFormat, &format1) != OMX_ErrorNone)
	{
	    printf("Can't set inputPort format!\n");
	} else {
	    printf("Set inputPort format success\n");
	}
	
	
	OMX_VIDEO_PARAM_PORTFORMATTYPE format2;
    memset(&format2, 0, sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE));
    format2.nSize = sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE);
    format2.nVersion.nVersion = OMX_VERSION;
    format2.nIndex = 0;
    format2.nPortIndex = outputPort;
    format2.eCompressionFormat = OMX_VIDEO_CodingUnused;
	format2.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
	format2.xFramerate = 0;
	
	if(OMX_SetParameter(component, OMX_IndexParamVideoPortFormat, &format2) != OMX_ErrorNone)
	{
	    printf("Can't set outputPort format!\n");
	} else {
	    printf("Set outputPort format success\n");
	}
   
	
	/*Send State Change Idle command to Video Decoder*/
    printf("STATE : IDLE request\n");
    if(OMX_ErrorNone != OMX_SendCommand(component, OMX_CommandStateSet, OMX_StateIdle, NULL)) {
		printf("Unable to set IDLE state\n");
	} else {
		printf("IDLE command sent\n");
	}


    printf("Allocate input buffers\n");
	for (uint32_t i = 0; i < inputBuffCount; i++){
    	inBuffer[i].bufIndex = i;
    	inBuffer[i].isBusy   = OMX_FALSE;

		printf("OMX_AllocateBuffer, iteration %d\n", i);
		inBuffer[i].bufHeader = NULL;
		
		OMX_ERRORTYPE err = OMX_AllocateBuffer(component, &inBuffer[i].bufHeader, inputPort, &inBuffer[i].bufIndex, inputBuffSize);
		if (OMX_ErrorNone != err) {
			printf("Error on Allocate %dth Buffer, error: 0x%x\n", i, err);
			return 1;
		}
	    
	}
    
    printf("Allocate output buffers\n");
    for (uint32_t i = 0; i < outputBuffCount; i++){
    	outBuffer[i].bufIndex = i;
    	outBuffer[i].isBusy   = OMX_FALSE;

		printf("OMX_AllocateBuffer, iteration %d\n", i);
		outBuffer[i].bufHeader = NULL;
		
		OMX_ERRORTYPE err = OMX_AllocateBuffer(component, &outBuffer[i].bufHeader, outputPort, &outBuffer[i].bufIndex, outputBuffSize);
		if (OMX_ErrorNone != err) {
			printf("Error on Allocate %dth Buffer, error: 0x%x\n", i, err);
			return 1;
		}
	    
	}
	


    while(1) {
		OMX_STATETYPE compState;
		OMX_GetState(component, &compState);
		if(compState == OMX_StateIdle) break;

		printf("Waiting until state of decoder is idle, current: %d\n", compState);
		usleep(200 * 1000);
	}
	printf("STATE : IDLE OK!\n");
	

        
    printf("do play action!\n");
	gIsStopped = OMX_FALSE;
	OMX_SendCommand(component, OMX_CommandStateSet, OMX_StateExecuting, NULL);

    while(gVdecState != OMX_StateExecuting){
        printf("Waiting until state of decoder is executing, current: %d\n", gVdecState);
        usleep(200 * 1000);
    }
               
    
    for (uint32_t i=0; i < outputBuffCount; i++) {
        printf ("FTB: address 0x%x, size = %d\n", outBuffer[i].bufHeader, outputBuffSize);
        err = OMX_FillThisBuffer(component, outBuffer[i].bufHeader);
        if(OMX_ErrorNone != err)
        {
            printf("OMX_FillThisBuffer out error: 0x%lx\n", err);
        } else {
            printf("Success!\n");
        }
    }
    
    fseek ( gTestFile , 0 , SEEK_SET );
    read_number = fread(data, 1, OMX_IL_INPUT_BUFFER_SIZE, gTestFile);
    
    uint32_t buffIndex = 0;
    while(read_number > 0) {
    
       
    
        inBuffer[0].isBusy = OMX_TRUE;
    	printf("Read: %d\n", read_number);
    	memcpy(inBuffer[buffIndex].bufHeader->pBuffer, data, read_number);
    	inBuffer[buffIndex].bufHeader->nFilledLen = read_number;
    	
        if(first_packet)
         {
            inBuffer[buffIndex].bufHeader->nFlags = OMX_BUFFERFLAG_STARTTIME;
            first_packet = 0;
         }
         //else
         //   inBuffer[i].bufHeader->nFlags = OMX_BUFFERFLAG_TIME_UNKNOWN;
         
        printf("send out the buffer: 0x%p\n", inBuffer[buffIndex].bufHeader);
	    err = OMX_EmptyThisBuffer(component, inBuffer[buffIndex].bufHeader);
	    if(OMX_ErrorNone != err) {
	        printf("OMX_EmptyThisBuffer, error: 0x%x\n", err);
	    } else {
	        printf("Success!\n");
	    }
	    
	    usleep(1000 * 1000);
	    
	    if(buffIndex == 0) {
	        buffIndex = 1;
	    } else {
	        buffIndex = 0;
	    }
	    
	    read_number = fread(data, 1, OMX_IL_INPUT_BUFFER_SIZE, gTestFile);
    }
    
    inBuffer[buffIndex].bufHeader->nFilledLen = 0;
    inBuffer[buffIndex].bufHeader->nFlags = OMX_BUFFERFLAG_EOS;
    
    err = OMX_EmptyThisBuffer(component, inBuffer[buffIndex].bufHeader);
    if(OMX_ErrorNone != err) {
        printf("OMX_EmptyThisBuffer, error: 0x%x\n", err);
    } else {
        printf("Success!\n");
    }
	    
	    
    if(OMX_ErrorNone != OMX_SendCommand(component, OMX_CommandFlush, outputPort, NULL)) {
        printf("Unable to flush port %d\n", outputPort);
    } else {
        printf("Flush command sent\n");
    }
    
    /**/

    if (!gIsStopped){
		gIsStopped = OMX_TRUE;
		OMX_SendCommand(component, OMX_CommandStateSet, OMX_StateIdle, NULL);
	}

	while(gVdecState != OMX_StateIdle){

    }

	OMX_SendCommand(component, OMX_CommandStateSet, OMX_StateLoaded, NULL);

    while(gVdecState != OMX_StateLoaded){

    }

    for (uint32_t i = 0; i < inputBuffCount; i++){
	    OMX_FreeBuffer(component, inputPort, inBuffer[i].bufHeader);
    }
    for (uint32_t i = 0; i < outputBuffCount; i++){
	    OMX_FreeBuffer(component, outputPort, outBuffer[i].bufHeader);
    }
    printf("Buffers deallocated\n");

    if (OMX_ErrorNone != OMX_FreeHandle(component)) {
        fprintf(stderr, "OMX_FreeHandle failed\n");
        return 1;
    }
    
    printf("OMX handle released\n");

    if (OMX_ErrorNone != OMX_Deinit()) {
        fprintf(stderr, "OMX deinit failed\n");
        return 1;
    }
    
    printf("OMX deinitialized\n");
    
    pthread_mutex_destroy(&lock);
    
    printf("EXIT\n");

    return 0;
    
}


static OMX_ERRORTYPE
OMXDecoder_emptyBufferDone
    (OMX_HANDLETYPE component, OMX_PTR appData, OMX_BUFFERHEADERTYPE *buffer)
{
    OMX_U8 data[OMX_IL_INPUT_BUFFER_SIZE];
	OMX_U32 index = *(OMX_U32 *)(buffer->pAppPrivate);

	printf("OMXDecoder_emptyBufferDone, get buffer %d returned, buffer header: 0x%x\n", index, buffer);
	inBuffer[index].isBusy = OMX_FALSE;

   printf ("EBD: address %p, size %d\n", buffer, buffer->nFilledLen);
       
    return OMX_ErrorNone;
}

static OMX_ERRORTYPE
OMXDecoder_eventHandler
    (OMX_HANDLETYPE component, OMX_PTR appData, OMX_EVENTTYPE event,
        OMX_U32 data1, OMX_U32 data2, OMX_PTR eventData)
{
    printf("Vdec event: %d\n", event);
    if(event == OMX_EventCmdComplete) {
        printf("Event complete: %d\n", data1);
    	if (data1 == OMX_CommandStateSet) {
        	printf("OMX_CommandStateSet\n");
        	switch ((int)data2) {
	        	case OMX_StateMax:
	        	    printf("OMX_StateMax\n");
	        	    break;
	        	case OMX_StateLoaded:
	        	    printf("OMX_StateLoaded\n");
	        	    gVdecState = OMX_StateLoaded;
	        	    break;
	        	case OMX_StateIdle:
	        	    printf("OMX_StateIdle\n");
	        	    gVdecState = OMX_StateIdle;
	        	    break;
	        	case OMX_StateExecuting:
	        	    printf("OMX_StateExecuting\n");
	        	    gVdecState = OMX_StateExecuting;
	        	    break;
	        	case OMX_StatePause:
	        	    printf("OMX_StatePause\n");
	        	    gVdecState = OMX_StatePause;
	        	    break;
	        	case OMX_StateWaitForResources:
	        	    printf("OMX_StateWaitForResources\n");
	        	    gVdecState = OMX_StateWaitForResources;
	        	    break;
        	}
        	//printf("Unlock mutex\n");
        	//pthread_mutex_unlock(&lock);
        }else if (data1 == OMX_CommandPortEnable){
		    printf("Vdec component enables port %d is done!\n", data2);
	    }else if (data1 == OMX_CommandPortDisable){
      		printf("Vdec component disables port %d is done!\n", data2);
        }else if (data1 == OMX_EventPortSettingsChanged) {
            printf("OMX_EventPortSettingsChanged\n");
    	}else if (data1 == OMX_CommandFlush){
    		printf("Vdec component flushes port %d is done!\n", data2);
    	}
    } else if(event == OMX_EventPortSettingsChanged) {
        printf("OMX_EventPortSettingsChanged\n");
    } else if(event == OMX_EventBufferFlag) {
       printf ("OMX_EventBufferFlag\n");
       if((int)data2 == OMX_BUFFERFLAG_EOS) {
           printf ("got EOS event \n");
       }
    } else if (event == OMX_EventError) {
        printf ("*** unrecoverable error: (0x%lx) \n", data1);
    }else{
    	printf("unsupported event: %d, data1: %u, data2: %u\n", event, data1, data2);
    }

	return OMX_ErrorNone;
}

static OMX_ERRORTYPE
OMXDecoder_fillBufferDone
    (OMX_HANDLETYPE component, OMX_PTR appData, OMX_BUFFERHEADERTYPE *buffer)
{
	OMX_U32 index = *(OMX_U32 *)(buffer->pAppPrivate);

	printf ("FBD: address %p\n", buffer);
	
	printf("OMXDecoder_fillBufferDone, get buffer %d returned, buffer header: 0x%x\n", index, buffer);
	printf("buffer filled len: %d\n", buffer->nFilledLen);
	printf("outBuffer[index].bufHeader->nFilledLen: %d\n", outBuffer[index].bufHeader->nFilledLen);
	
	    printf("Got buffer:\n");
	        	for (int j = 0; j < 256; j++)
                            printf("%x ", outBuffer[index].bufHeader->pBuffer[j]);
                        printf("\n");
	

    outBuffer[index].isBusy = OMX_FALSE;

    return OMX_ErrorNone;
}
