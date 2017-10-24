/******************************************************************************
 *                         COPYRIGHT 2006 IWEDIA TECHNOLOGIES                 *
 ******************************************************************************
 *
 * MODULE NAME: TDAL_TS  -  TS Management driver
 *
 * FILE NAME:   TDAL_TS.H
 *   $URL: svn+ssh://ext_iwedia-ext@svn.rt-rk.com/TV3.0/sources/chal/core/tdal/trunk/inc/tdal_ts.h $
 *   $Rev: 1129 $
 *   $Date: 2012-02-03 18:01:45 +0100 (ven., 03 f√©vr. 2012) $
 *
 * PUBLIC
 *
 * DESCRIPTION - This driver is divided in 3 parts :
 *                - TDAL_TS_ROUTE  : Connection between TS source and demux
 *                - TDAL_TS_SINK   :
 *                - TDAL_TS_SOURCE :
 *
 *****************************************************************************/

#ifndef _TDAL_TS_H_
#define _TDAL_TS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *   Includes
 ******************************************************************************/

/******************************************************************************
 *   Defines
 ******************************************************************************/
 /* tTDAL_TS_Object values */
#define kTDAL_TS_MASK_CLASS         0xf000
#define kTDAL_TS_MASK_TYPE          0x0ff0
#define kTDAL_TS_MASK_INSTANCE      0x000f

enum {
    eTDAL_TS_CLASS_SOURCE = 0 ,
    eTDAL_TS_CLASS_DESTINATION
};

enum {
    eTDAL_TS_TYPE_TUNER = 0 ,
    eTDAL_TS_TYPE_IEEE1394 ,
    eTDAL_TS_TYPE_MEMORY ,
    eTDAL_TS_TYPE_DEMUX
};

#define kTDAL_TS_NO_OBJECT          0xFFFF


/******************************************************************************
 *   Macros
 ******************************************************************************/
#define mTDAL_TS_GetObjectId( class , type , instance ) \
          ( class << 12 ) + ( type << 4 ) + instance
#define mTDAL_TS_GetObjectClass( object ) \
          ( object & kTDAL_TS_MASK_CLASS ) >> 12
#define mTDAL_TS_GetObjectType( object ) \
          ( object & kTDAL_TS_MASK_TYPE ) >> 4
#define mTDAL_TS_GetObjectInstance( object ) \
          ( object & kTDAL_TS_MASK_INSTANCE )


/******************************************************************************
 *   Typedefs
 ******************************************************************************/


/* ****************** *
 *  Common typedefs   *
 * ****************** */
typedef    uint16_t    tTDAL_TS_Object;



/* ****************** *
 * TS_ROUTE typedefs  *
 * ****************** */
/* error codes */
typedef enum TDAL_TSROUTE_Error_e {
    eTDAL_TSROUTE_NO_ERROR = 0,	    /* no error                  */
    eTDAL_TSROUTE_ERROR_NOT_INIT,	/* driver is not initialised */
    eTDAL_TSROUTE_ERROR_BAD_ARG,	/* wrong argument value      */
    eTDAL_TSROUTE_ERROR_IN_DRIVER,	/* error in the driver       */
    eTDAL_TSROUTE_ERROR_CONFLICT,	/* an object is already used */
    eTDAL_TSROUTE_ERROR_NOT_DONE	/* command not done          */
} tTDAL_TSROUTE_Error;

/* capabilities structure */
typedef struct TDAL_TSROUTE_Capabilities_s {
    uint8_t	            nb_sources;
    tTDAL_TS_Object *   sources_array;
    uint8_t	            nb_destinations;
    tTDAL_TS_Object *   destinations_array;
} tTDAL_TSROUTE_Capabilities;

/* status structure */
typedef struct TDAL_TSROUTE_Status_s {
    tTDAL_TS_Object	    connected_object;
    uint32_t            packet_count;
} tTDAL_TSROUTE_Status;



/* ****************** *
 *  TS_SINK typedefs  *
 * ****************** */
/* error codes */
typedef enum {
    eTDAL_TSSINK_NO_ERROR = 0,        /* no error. */
    eTDAL_TSSINK_ERROR_BAD_ARG,       /* wrong argument value. */
    eTDAL_TSSINK_ERROR_NO_MEMORY,     /* not enough memory. */
    eTDAL_TSSINK_ERROR_IN_DRIVER,     /* error in the driver */
    eTDAL_TSSINK_ERROR_CONFLICT,      /* error in access to a TS source or a
                                         demux */
    eTDAL_TSSINK_WARNING,             /* an error occured but it could be OK */
    eTDAL_TSSINK_ERROR_BAD_HANDLE,
    eTDAL_TSSINK_ERROR_NOT_LINKED,
    eTDAL_TSSINK_ERROR_CAPABILITIES,  /* not handled in this TDAL TSIN
                                         implementation */
    eTDAL_TSSINK_ERROR_NOT_DONE       /* command not done. */
} tTDAL_TSSINK_Error;


/* injection handle to pass to TDAL_TSSINK functions */
typedef    void *    tTDAL_TSSINK_Handle;


/* callback function called when a buffer is full */
typedef void (* tTDAL_TSSINK_ConsumerCallbackFct )(
    tTDAL_TSSINK_Handle     handle,
    void *                  client_tag,
    uint8_t *               address_1,
    uint32_t                nb_packets_max_1,
    uint8_t *               address_2,
    uint32_t                nb_packets_max_2
);


/* enumeration for different types of TS Sink */
typedef enum TDAL_TSSINK_Type_e {
    eTDAL_TSSINK_TYPE_DEMUX, /* Data are consumed directed by the SWTS (injected in demux) */
    eTDAL_TSSINK_TYPE_EXTERN /* Data are produced by another module */
} tTDAL_TSSINK_Type;


/* structure containing parameters for opening a new injection */
typedef struct TDAL_TSSINK_OpenParam_s {
    tTDAL_TSSINK_Type           type ;
    tTDAL_TS_Object             source ;
    tTDAL_TS_Object             demux ;
    uint32_t                    notification_level ;
} tTDAL_TSSINK_OpenParam;


/* structure containing parameters for opening a new injection */
typedef struct TDAL_TSSINK_StartParam_s {
    bool                        tv_not_radio ; /* TV: true   RADIO: false */
    uint32_t                    initial_packet_number ;
    bool                        realtime ;
} tTDAL_TSSINK_StartParam;


/* structure containing status information about an injection */
typedef struct TDAL_TSSINK_Status_s {
    uint32_t    byterate_Bps;     /* byterate in Bytes/sec */
    bool        end_of_injection; /* TRUE if injection is terminated, FALSE otherwise */
} tTDAL_TSSINK_Status;


/* ****************** *
 * TS_SOURCE typedefs  *
 * ****************** */
/* error codes */
typedef enum {
    eTDAL_TSSOURCE_NO_ERROR = 0,        /* no error. */
    eTDAL_TSSOURCE_ERROR_BAD_ARG,       /* wrong argument value. */
    eTDAL_TSSOURCE_ERROR_NO_MEMORY,     /* not enough memory. */
    eTDAL_TSSOURCE_ERROR_IN_DRIVER,     /* error in the driver */
    eTDAL_TSSOURCE_WARNING,             /* an error occured but the operation
                                           could be done */
    eTDAL_TSSOURCE_ERROR_BAD_HANDLE,    /* the TS Source handle is wrong type */
    eTDAL_TSSOURCE_ERROR_NOT_DONE       /* command not done. */
} tTDAL_TSSOURCE_Error;


/* extraction handle to pass to TDAL_TSSOURCE functions */
typedef void *  tTDAL_TSSOURCE_Handle;


/* enumeration of different type */
typedef enum TDAL_TSSOURCE_Type_e {
    eTDAL_TSSOURCE_TYPE_DEMUX, /* Data are produced by extraction from demux */
    eTDAL_TSSOURCE_TYPE_MEMORY /* Data are produced by another module */
} tTDAL_TSSOURCE_Type;


/* PID type */
typedef    uint16_t  tTDAL_TSSOURCE_Pid;


/* callback function called to notify the external producer that data can be produced */
typedef void (* tTDAL_TSSOURCE_ProducerCallbackFct )(
    tTDAL_TSSOURCE_Handle   handle,
    uint8_t *               address_1,
    uint32_t                nb_packets_max_1,
    uint8_t *               address_2,
    uint32_t                nb_packets_max_2,
    void *                  private_data
);


/* callback function called to notify the external producer that a jump in soure stream is needed */
typedef void (* tTDAL_TSSOURCE_RequestJumpCallbackFct )(
    tTDAL_TSSOURCE_Handle   handle,
    uint32_t     			packet_origin,
	int32_t      			jump_ms,
	uint32_t *   			p_new_packet_position,
    void *                  private_data
);


/* callback function called to notify client application of the TS timestamping */
typedef void (* tTDAL_TSSOURCE_TimestampingCallbackFct )(
    tTDAL_TSSOURCE_Handle   handle,
    void *                  client_tag,
    uint32_t                packet_position,
    uint32_t                packet_time_ms
);


/* structure containing parameters for opening a new extraction */
typedef struct TDAL_TSSOURCE_OpenParam_s {
    tTDAL_TSSOURCE_Type     type ;
    uint32_t                buffer_length ; /* nom ‡ trouver */
    uint32_t                packet_length ;
} tTDAL_TSSOURCE_OpenParam;


/* structure containing parameters for configuring an extraction */
typedef struct TDAL_TSSOURCE_ConfigParam_s {
    union {
      struct {
        tTDAL_TS_Object                         demux ;
      } dmx ;
      struct {
        tTDAL_TSSOURCE_ProducerCallbackFct      producer_callback;
        tTDAL_TSSOURCE_RequestJumpCallbackFct   request_jump_callback;
        void *                                  private_data;
      } mem ;
    } u ;
} tTDAL_TSSOURCE_ConfigParam;


/* ****************** *
 * TS_MERGE typedefs  *
 * ****************** */
/* error codes */
typedef enum {
    eTDAL_TSMERGE_NO_ERROR = 0,        /* no error. */
    eTDAL_TSMERGE_ERROR_BAD_ARG,       /* wrong argument value. */
    eTDAL_TSMERGE_ERROR_NO_MEMORY,     /* not enough memory. */
    eTDAL_TSMERGE_ERROR_IN_DRIVER,     /* error in the driver */
    eTDAL_TSMERGE_ERROR_BAD_HANDLE,    /* the TS Source handle is wrong type */
    eTDAL_TSMERGE_ERROR_NOT_DONE       /* command not done. */
} tTDAL_TSMERGE_Error;

typedef enum {
    eTDAL_TSMERGE_DEVICE_NOT_USED = 0, 
#ifdef PRODUCT_USE_CIMAX_DEVICE
    eTDAL_TSMERGE_CIMAX_MONOSLOT_MONOSTREAM,
    eTDAL_TSMERGE_CIMAX_DOUBLESLOT_MONOSTREAM,
    eTDAL_TSMERGE_CIMAX_MONOSLOT_MULTISTREAM,
    eTDAL_TSMERGE_CIMAX_DOUBLESLOT_MULTISTREAM,
#endif
    eTDAL_TSMERGE_NB_AVAILABLE_MODES
} tTDAL_TSMERGE_Mode;

typedef struct TDAL_TSMERGE_OpenParam_s {
#ifdef PRODUCT_USE_CIMAX_DEVICE
	uint32_t u32TSFilterIndex;
	uint8_t u8InModuleId;
	uint8_t u8OutModuleId;
#else
	uint32_t u32Index;
#endif
} tTDAL_TSMERGE_OpenParam;


/* merge handle to pass to TDAL_TSMERGE functions */
typedef uint32_t tTDAL_TSMERGE_Handle;

/*******************************************************************************
 *   Variables Declarations (IMPORT)
 ******************************************************************************/
/* none */

/*******************************************************************************
 *   Functions Declarations (GLOBAL)
 ******************************************************************************/

/* ******************* *
 * TS Generic functions  *
 * ******************* */
const char 				*TDAL_TS_APIRevisionGet(void);

const char 				*TDAL_TS_PlatformRevisionGet(void);

/* ******************* *
 * TS_ROUTE functions  *
 * ******************* */
tTDAL_TSROUTE_Error     TDAL_TSROUTE_Init( void );

tTDAL_TSROUTE_Error     TDAL_TSROUTE_Term( void );

tTDAL_TSROUTE_Error     TDAL_TSROUTE_GetCapabilities(
                            tTDAL_TSROUTE_Capabilities *    p_capabilities
                        );

tTDAL_TSROUTE_Error     TDAL_TSROUTE_Connect(
                            tTDAL_TS_Object                 source ,
                            tTDAL_TS_Object	                destination	
                        );

tTDAL_TSROUTE_Error     TDAL_TSROUTE_Disconnect(
                            tTDAL_TS_Object	                source ,
                            tTDAL_TS_Object	                destination	
                        );

tTDAL_TSROUTE_Error     TDAL_TSROUTE_GetStatus(
                            tTDAL_TS_Object                 object ,
                            tTDAL_TSROUTE_Status *          p_status
                        );



/* ******************* *
 *  TS_SINK functions  *
 * ******************* */
/*   see tdal_tssink.c for detailled comments for these functions   */

            /* Initializes the TDAL_TSSINK driver */
GLOBAL  tTDAL_TSSINK_Error  TDAL_TSSINK_Init( void );

            /* Terminates the TDAL_TSSINK driver */
GLOBAL  tTDAL_TSSINK_Error  TDAL_TSSINK_Term( void );


            /* Initializes an injection */
GLOBAL  tTDAL_TSSINK_Error  TDAL_TSSINK_Open(
                                tTDAL_TSSINK_Handle *       p_handle,
                                tTDAL_TSSINK_OpenParam *    p_oprm
                            );

            /* Closes an injection */
GLOBAL  tTDAL_TSSINK_Error  TDAL_TSSINK_Close(
                                tTDAL_TSSINK_Handle         handle
                            );

            /* Start injecting data */
GLOBAL  tTDAL_TSSINK_Error  TDAL_TSSINK_Start(
                                tTDAL_TSSINK_Handle         handle ,
                                tTDAL_TSSINK_StartParam *   p_sprm
                            );

            /* Start injecting data at same position of 'predecessor' sink */
GLOBAL  tTDAL_TSSINK_Error  TDAL_TSSINK_StartFrom(
                                tTDAL_TSSINK_Handle         handle ,
                                tTDAL_TSSINK_Handle         predecessor ,
                                tTDAL_TSSINK_StartParam *   p_sprm
                            );

            /* Stop the injection of data */
GLOBAL  tTDAL_TSSINK_Error  TDAL_TSSINK_Stop(
                                tTDAL_TSSINK_Handle         handle
                            );

			/* Set time position */
GLOBAL  tTDAL_TSSINK_Error  TDAL_TSSINK_SetTimePosition(
                                tTDAL_TSSINK_Handle         handle,
								bool						relative,
								signed int					timeInMs
                            );

			/* Get packet position */
GLOBAL  tTDAL_TSSINK_Error  TDAL_TSSINK_GetPacketPosition(
                                tTDAL_TSSINK_Handle         handle,
                                uint32_t *					p_packet_position
                            );


			/* Get status information about an injection */
GLOBAL  tTDAL_TSSINK_Error  TDAL_TSSINK_GetStatus(
                                tTDAL_TSSINK_Handle         handle,
                                tTDAL_TSSINK_Status *		p_status
                            );


            /* Set the external consumer call-back */
GLOBAL  tTDAL_TSSINK_Error  TDAL_TSSINK_SetConsumerCallback(
                                tTDAL_TSSINK_Handle                 handle,
                                tTDAL_TSSINK_ConsumerCallbackFct    consumer_callback,
                                void *                              client_tag
                            );

            /* Notify TSSINK of consumption */
GLOBAL  tTDAL_TSSINK_Error  TDAL_TSSINK_NotifyConsumption(
                                tTDAL_TSSINK_Handle         handle,
                                uint32_t                    consumed_packets,
                                uint32_t                    notification_level
                            );

            /* Link this sink to a producer */
GLOBAL  tTDAL_TSSINK_Error  TDAL_TSSINK_SetTSSource(
                                tTDAL_TSSINK_Handle         handle,
                                tTDAL_TSSOURCE_Handle       tssource_handle
                            );



/* ******************* *
 * TS_SOURCE functions *
 * ******************* */
/* see tdal_ts_source.c for detailled comments about these functions */

            /* Initializes the TDAL_TSSOURCE driver */
GLOBAL  tTDAL_TSSOURCE_Error  TDAL_TSSOURCE_Init( void );

            /* Terminates the TDAL_TSSOURCE driver */
GLOBAL  tTDAL_TSSOURCE_Error  TDAL_TSSOURCE_Term( void );

            /* Initializes an extraction */
GLOBAL  tTDAL_TSSOURCE_Error  TDAL_TSSOURCE_Open(
                                tTDAL_TSSOURCE_Handle *         p_handle,
                                tTDAL_TSSOURCE_OpenParam *      p_oprm
                              );

            /* Configures an extraction */
GLOBAL  tTDAL_TSSOURCE_Error  TDAL_TSSOURCE_Config(
                                tTDAL_TSSOURCE_Handle           handle,
                                tTDAL_TSSOURCE_ConfigParam *    p_cfgprm
                              );

            /* Closes an extraction */
GLOBAL  tTDAL_TSSOURCE_Error  TDAL_TSSOURCE_Close(
                                tTDAL_TSSOURCE_Handle           handle
                              );

            /* Add a PID to an extraction */
GLOBAL  tTDAL_TSSOURCE_Error  TDAL_TSSOURCE_AddPid(
                                tTDAL_TSSOURCE_Handle           handle,
                                tTDAL_TSSOURCE_Pid              pid
                              );

            /* Remove a PID from an extraction */
GLOBAL  tTDAL_TSSOURCE_Error  TDAL_TSSOURCE_RemovePid(
                                tTDAL_TSSOURCE_Handle           handle,
                                tTDAL_TSSOURCE_Pid              pid
                              );

            /* Start TS source */
GLOBAL  tTDAL_TSSOURCE_Error  TDAL_TSSOURCE_Start(
                                tTDAL_TSSOURCE_Handle           handle
                              );

            /* Stop TS source */
GLOBAL  tTDAL_TSSOURCE_Error  TDAL_TSSOURCE_Stop(
                                tTDAL_TSSOURCE_Handle           handle
                              );

            /* Set the Timestamping consumer call-back */
GLOBAL  tTDAL_TSSOURCE_Error  TDAL_TSSOURCE_SetTimestampingCallback(
                                tTDAL_TSSOURCE_Handle                   handle,
                                tTDAL_TSSOURCE_TimestampingCallbackFct  timestamping_callback,
                                void *                                  client_tag /*,
                                uint32_t                                nb_packets  */
                              );

            /* Notify TSOUT driver that buffer is
               processed and could be re-used */
GLOBAL  tTDAL_TSSOURCE_Error  TDAL_TSSOURCE_NotifyProduction(
                                tTDAL_TSSOURCE_Handle           handle,
                                uint32_t                        produced_packets,
                                uint32_t                        notification_level /* nom ‡ trouver */
                              );

            /* Pause TS source */
GLOBAL  tTDAL_TSSOURCE_Error  TDAL_TSSOURCE_Pause(
                                tTDAL_TSSOURCE_Handle           handle,
                                bool                            reinit_pointer
                              );

            /* Resume TS source */
GLOBAL  tTDAL_TSSOURCE_Error  TDAL_TSSOURCE_Resume(
                                tTDAL_TSSOURCE_Handle           handle
                              );


/* ******************* *
 * TS_MERGE functions *
 * ******************* */
/* see tdal_ts_merge.c for detailled comments about these functions */

			/* Initializes the TDAL_TSMERGE driver */
GLOBAL	tTDAL_TSMERGE_Error  TDAL_TSMERGE_Init( void );

			/* Terminates the TDAL_TSMERGE driver */
GLOBAL	tTDAL_TSMERGE_Error  TDAL_TSMERGE_Term( void );

			/* Initializes an extraction */
GLOBAL	tTDAL_TSMERGE_Error  TDAL_TSMERGE_Open(
						tTDAL_TSMERGE_Handle * 		p_handle,
						tTDAL_TSMERGE_OpenParam *		p_oprm
					  );

			/* Closes an extraction */
GLOBAL	tTDAL_TSMERGE_Error  TDAL_TSMERGE_Close(
						tTDAL_TSMERGE_Handle			handle
					  );

GLOBAL tTDAL_TSMERGE_Error TDAL_TSMERGE_Start(
						tTDAL_TSMERGE_Handle handle,
						int serviceID
						);

GLOBAL tTDAL_TSMERGE_Error TDAL_TSMERGE_Stop(
						tTDAL_TSMERGE_Handle handle
						);

GLOBAL tTDAL_TSMERGE_Error TDAL_TSMERGE_SetCurrentMode(
						tTDAL_TSMERGE_Mode newMode
					);

GLOBAL tTDAL_TSMERGE_Mode TDAL_TSMERGE_GetCurrentMode(void);

#ifdef __cplusplus
}
#endif

#endif /* _TDAL_TS_H_ */
