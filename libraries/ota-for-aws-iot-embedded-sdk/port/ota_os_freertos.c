/*
 * AWS IoT Over-the-air Update v3.3.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file ota_os_freertos.c
 * @brief Example implementation of the OTA OS Functional Interface for
 * FreeRTOS.
 */

/* FreeRTOS includes. */
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/queue.h"

/* OTA OS POSIX Interface Includes.*/
#include "ota_os_freertos.h"

/* OTA Library include. */
#include "ota.h"
#include "ota_private.h"

/* OTA Event queue attributes.*/
#define MAX_MESSAGES    20
#define MAX_MSG_SIZE    sizeof( OtaEventMsg_t )

/* Array containing pointer to the OTA event structures used to send events to the OTA task. */
static OtaEventMsg_t queueData[ MAX_MESSAGES * MAX_MSG_SIZE ];

/* The queue control structure.  .*/
static StaticQueue_t staticQueue;

/* The queue control handle.  .*/
static QueueHandle_t otaEventQueue;

/* OTA App Timer callback.*/
static OtaTimerCallback_t otaTimerCallback;

/* OTA Timer handles.*/
static TimerHandle_t otaTimer[ OtaNumOfTimers ];

/* OTA Timer callbacks.*/
static void requestTimerCallback( TimerHandle_t T );
static void selfTestTimerCallback( TimerHandle_t T );
void ( * timerCallback[ OtaNumOfTimers ] )( TimerHandle_t T ) = { requestTimerCallback, selfTestTimerCallback };

OtaOsStatus_t OtaInitEvent_FreeRTOS( OtaEventContext_t * pEventCtx )
{
    OtaOsStatus_t otaOsStatus = OtaOsSuccess;

    ( void ) pEventCtx;

    otaEventQueue = xQueueCreateStatic( ( UBaseType_t ) MAX_MESSAGES,
                                        ( UBaseType_t ) MAX_MSG_SIZE,
                                        ( uint8_t * ) queueData,
                                        &staticQueue );

    if( otaEventQueue == NULL )
    {
        otaOsStatus = OtaOsEventQueueCreateFailed;

        LogError( ( "Failed to create OTA Event Queue: "
                    "xQueueCreateStatic returned error: "
                    "OtaOsStatus_t=%i ",
                    otaOsStatus ) );
    }
    else
    {
        LogDebug( ( "OTA Event Queue created." ) );
    }

    return otaOsStatus;
}

OtaOsStatus_t OtaSendEvent_FreeRTOS( OtaEventContext_t * pEventCtx,
                                     const void * pEventMsg,
                                     unsigned int timeout )
{
    OtaOsStatus_t otaOsStatus = OtaOsSuccess;
    BaseType_t retVal = pdFALSE;

    ( void ) pEventCtx;
    ( void ) timeout;

    /* Send the event to OTA event queue.*/
    retVal = xQueueSendToBack( otaEventQueue, pEventMsg, ( TickType_t ) 0 );

    if( retVal == pdTRUE )
    {
        LogDebug( ( "OTA Event Sent." ) );
    }
    else
    {
        otaOsStatus = OtaOsEventQueueSendFailed;

        LogError( ( "Failed to send event to OTA Event Queue: "
                    "xQueueSendToBack returned error: "
                    "OtaOsStatus_t=%i ",
                    otaOsStatus ) );
    }

    return otaOsStatus;
}

OtaOsStatus_t OtaReceiveEvent_FreeRTOS( OtaEventContext_t * pEventCtx,
                                        void * pEventMsg,
                                        uint32_t timeout )
{
    OtaOsStatus_t otaOsStatus = OtaOsSuccess;
    BaseType_t retVal = pdFALSE;

    /* Temp buffer.*/
    uint8_t buff[ sizeof( OtaEventMsg_t ) ];

    ( void ) pEventCtx;
    ( void ) timeout;

    retVal = xQueueReceive( otaEventQueue, &buff, portMAX_DELAY );

    if( retVal == pdTRUE )
    {
        /* copy the data from local buffer.*/
        memcpy( pEventMsg, buff, MAX_MSG_SIZE );
        LogDebug( ( "OTA Event received" ) );
    }
    else
    {
        otaOsStatus = OtaOsEventQueueReceiveFailed;

        LogError( ( "Failed to receive event from OTA Event Queue: "
                    "xQueueReceive returned error: "
                    "OtaOsStatus_t=%i ",
                    otaOsStatus ) );
    }

    return otaOsStatus;
}

OtaOsStatus_t OtaDeinitEvent_FreeRTOS( OtaEventContext_t * pEventCtx )
{
    OtaOsStatus_t otaOsStatus = OtaOsSuccess;

    ( void ) pEventCtx;

    /* Remove the event queue.*/
    if( otaEventQueue != NULL )
    {
        vQueueDelete( otaEventQueue );

        LogDebug( ( "OTA Event Queue Deleted." ) );
    }

    return otaOsStatus;
}

static void selfTestTimerCallback( TimerHandle_t T )
{
    ( void ) T;

    LogDebug( ( "Self-test expired within %ums\r\n",
                otaconfigSELF_TEST_RESPONSE_WAIT_MS ) );

    if( otaTimerCallback != NULL )
    {
        otaTimerCallback( OtaSelfTestTimer );
    }
    else
    {
        LogWarn( ( "Self-test timer event unhandled.\r\n" ) );
    }
}

static void requestTimerCallback( TimerHandle_t T )
{
    ( void ) T;

    LogDebug( ( "Request timer expired in %ums \r\n",
                otaconfigFILE_REQUEST_WAIT_MS ) );

    if( otaTimerCallback != NULL )
    {
        otaTimerCallback( OtaRequestTimer );
    }
    else
    {
        LogWarn( ( "Request timer event unhandled.\r\n" ) );
    }
}

OtaOsStatus_t OtaStartTimer_FreeRTOS( OtaTimerId_t otaTimerId,
                                      const char * const pTimerName,
                                      const uint32_t timeout,
                                      OtaTimerCallback_t callback )
{
    OtaOsStatus_t otaOsStatus = OtaOsSuccess;
    BaseType_t retVal = pdFALSE;

    configASSERT( callback != NULL );
    configASSERT( pTimerName != NULL );

    /* Set OTA lib callback. */
    otaTimerCallback = callback;

    /* If timer is not created.*/
    if( otaTimer[ otaTimerId ] == NULL )
    {
        /* Create the timer. */
        otaTimer[ otaTimerId ] = xTimerCreate( pTimerName,
                                               pdMS_TO_TICKS( timeout ),
                                               pdFALSE,
                                               NULL,
                                               timerCallback[ otaTimerId ] );

        if( otaTimer[ otaTimerId ] == NULL )
        {
            otaOsStatus = OtaOsTimerCreateFailed;

            LogError( ( "Failed to create OTA timer: "
                        "timerCreate returned NULL "
                        "OtaOsStatus_t=%i ",
                        otaOsStatus ) );
        }
        else
        {
            LogDebug( ( "OTA Timer created." ) );

            /* Start the timer. */
            retVal = xTimerStart( otaTimer[ otaTimerId ], portMAX_DELAY );

            if( retVal == pdTRUE )
            {
                LogDebug( ( "OTA Timer started." ) );
            }
            else
            {
                otaOsStatus = OtaOsTimerStartFailed;

                LogError( ( "Failed to start OTA timer: "
                            "timerStart returned error." ) );
            }
        }
    }
    else
    {
        /* Reset the timer. */
        retVal = xTimerReset( otaTimer[ otaTimerId ], portMAX_DELAY );

        if( retVal == pdTRUE )
        {
            LogDebug( ( "OTA Timer restarted." ) );
        }
        else
        {
            otaOsStatus = OtaOsTimerRestartFailed;

            LogError( ( "Failed to set OTA timer timeout: "
                        "timer_settime returned error: "
                        "OtaOsStatus_t=%i ",
                        otaOsStatus ) );
        }
    }

    return otaOsStatus;
}

OtaOsStatus_t OtaStopTimer_FreeRTOS( OtaTimerId_t otaTimerId )
{
    OtaOsStatus_t otaOsStatus = OtaOsSuccess;
    BaseType_t retVal = pdFALSE;

    if( otaTimer[ otaTimerId ] != NULL )
    {
        /* Stop the timer. */
        retVal = xTimerStop( otaTimer[ otaTimerId ], portMAX_DELAY );

        if( retVal == pdTRUE )
        {
            LogDebug( ( "OTA Timer Stopped for Timerid=%i.", otaTimerId ) );
        }
        else
        {
            LogError( ( "Failed to stop OTA timer: "
                        "timer_settime returned error: "
                        "OtaOsStatus_t=%i ",
                        otaOsStatus ) );

            otaOsStatus = OtaOsTimerStopFailed;
        }
    }
    else
    {
        LogWarn( ( "OTA Timer handle NULL for Timerid=%i, can't stop.", otaTimerId ) );
    }

    return otaOsStatus;
}

OtaOsStatus_t OtaDeleteTimer_FreeRTOS( OtaTimerId_t otaTimerId )
{
    OtaOsStatus_t otaOsStatus = OtaOsSuccess;
    BaseType_t retVal = pdFALSE;

    if( otaTimer[ otaTimerId ] != NULL )
    {
        /* Delete the timer. */
        retVal = xTimerDelete( otaTimer[ otaTimerId ], portMAX_DELAY );

        if( retVal == pdTRUE )
        {
            otaTimer[ otaTimerId ] = NULL;
            LogDebug( ( "OTA Timer deleted." ) );
        }
        else
        {
            otaOsStatus = OtaOsTimerDeleteFailed;

            LogError( ( "Failed to delete OTA timer: "
                        "timer_delete returned error: "
                        "OtaOsStatus_t=%i ",
                        otaOsStatus ) );
        }
    }
    else
    {
        otaOsStatus = OtaOsTimerDeleteFailed;

        LogWarn( ( "OTA Timer handle NULL for Timerid=%i, can't delete.", otaTimerId ) );
    }

    return otaOsStatus;
}

void * Malloc_FreeRTOS( size_t size )
{
    return pvPortMalloc( size );
}

void Free_FreeRTOS( void * ptr )
{
    vPortFree( ptr );
}
