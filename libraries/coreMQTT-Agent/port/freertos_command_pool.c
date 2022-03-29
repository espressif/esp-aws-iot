/*
 * ThirdEye
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
/**
 * @file freertos_command_pool.c
 * @brief Implements functions to obtain and release commands.
 */

/* Standard includes. */
#include <string.h>
#include <stdio.h>

/* Kernel includes. */
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/* Header include. */
#include "freertos_command_pool.h"
#include "freertos_agent_message.h"

/*-----------------------------------------------------------*/

#define QUEUE_NOT_INITIALIZED    ( 0U )
#define QUEUE_INITIALIZED        ( 1U )

#define MQTT_COMMAND_CONTEXTS_POOL_SIZE     ( 10 )

/**
 * @brief The pool of command structures used to hold information on commands (such
 * as PUBLISH or SUBSCRIBE) between the command being created by an API call and
 * completion of the command by the execution of the command's callback.
 */
static MQTTAgentCommand_t commandStructurePool[ MQTT_COMMAND_CONTEXTS_POOL_SIZE ];

/**
 * @brief The message context used to guard the pool of MQTTAgentCommand_t structures.
 * For FreeRTOS, this is implemented with a queue. Structures may be
 * obtained by receiving a pointer from the queue, and returned by
 * sending the pointer back into it.
 */
static MQTTAgentMessageContext_t commandStructMessageCtx;

/**
 * @brief Initialization status of the queue.
 */
static volatile uint8_t initStatus = QUEUE_NOT_INITIALIZED;

/*-----------------------------------------------------------*/

void Agent_InitializePool( void )
{
    size_t i;
    MQTTAgentCommand_t * pCommand;
    static uint8_t staticQueueStorageArea[ MQTT_COMMAND_CONTEXTS_POOL_SIZE * sizeof( MQTTAgentCommand_t * ) ];
    static StaticQueue_t staticQueueStructure;
    bool commandAdded = false;

    if( initStatus == QUEUE_NOT_INITIALIZED )
    {
        memset( ( void * ) commandStructurePool, 0x00, sizeof( commandStructurePool ) );
        commandStructMessageCtx.queue = xQueueCreateStatic( MQTT_COMMAND_CONTEXTS_POOL_SIZE,
                                                       sizeof( MQTTAgentCommand_t * ),
                                                       staticQueueStorageArea,
                                                       &staticQueueStructure );
        configASSERT( commandStructMessageCtx.queue );

        /* Populate the queue. */
        for( i = 0; i < MQTT_COMMAND_CONTEXTS_POOL_SIZE; i++ )
        {
            /* Store the address as a variable. */
            pCommand = &commandStructurePool[ i ];
            /* Send the pointer to the queue. */
            commandAdded = Agent_MessageSend( &commandStructMessageCtx, &pCommand, 0U );
            configASSERT( commandAdded );
        }

        initStatus = QUEUE_INITIALIZED;
    }
}

/*-----------------------------------------------------------*/

MQTTAgentCommand_t * Agent_GetCommand( uint32_t blockTimeMs )
{
    MQTTAgentCommand_t * structToUse = NULL;
    bool structRetrieved = false;

    /* Check queue has been created. */
    configASSERT( initStatus == QUEUE_INITIALIZED );

    /* Retrieve a struct from the queue. */
    structRetrieved = Agent_MessageReceive( &commandStructMessageCtx, &( structToUse ), blockTimeMs );

    if( !structRetrieved )
    {
        LogError( ( "No command structure available." ) );
    } else {
        LogDebug( ( "Removed Command Context %d from pool",
                    ( int ) ( structToUse - commandStructurePool ) ) );
    }

    return structToUse;
}

/*-----------------------------------------------------------*/

bool Agent_ReleaseCommand( MQTTAgentCommand_t * pCommandToRelease )
{
    bool structReturned = false;

    configASSERT( initStatus == QUEUE_INITIALIZED );

    /* See if the structure being returned is actually from the pool. */
    if( ( pCommandToRelease >= commandStructurePool ) &&
        ( pCommandToRelease < ( commandStructurePool + MQTT_COMMAND_CONTEXTS_POOL_SIZE ) ) )
    {
        structReturned = Agent_MessageSend( &commandStructMessageCtx, &pCommandToRelease, 0U );

        /* The send should not fail as the queue was created to hold every command
         * in the pool. */
        configASSERT( structReturned );
        LogDebug( ( "Returned Command Context %d to pool",
                    ( int ) ( pCommandToRelease - commandStructurePool ) ) );
    }

    return structReturned;
}
