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
 * @file freertos_agent_message.c
 * @brief Implements functions to interact with queues.
 */

/* Standard includes. */
#include <string.h>
#include <stdio.h>

/* Kernel includes. */
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

/* Header include. */
#include "freertos_agent_message.h"
#include "core_mqtt_agent_message_interface.h"

/*-----------------------------------------------------------*/

bool Agent_MessageSend( MQTTAgentMessageContext_t * pMsgCtx,
                        MQTTAgentCommand_t * const * pCommandToSend,
                        uint32_t blockTimeMs )
{
    BaseType_t queueStatus = pdFAIL;

    if( ( pMsgCtx != NULL ) && ( pCommandToSend != NULL ) )
    {
        queueStatus = xQueueSendToBack( pMsgCtx->queue, pCommandToSend, pdMS_TO_TICKS( blockTimeMs ) );
    }

    return ( queueStatus == pdPASS ) ? true : false;
}

/*-----------------------------------------------------------*/

bool Agent_MessageReceive( MQTTAgentMessageContext_t * pMsgCtx,
                           MQTTAgentCommand_t ** pReceivedCommand,
                           uint32_t blockTimeMs )
{
    BaseType_t queueStatus = pdFAIL;

    if( ( pMsgCtx != NULL ) && ( pReceivedCommand != NULL ) )
    {
        queueStatus = xQueueReceive( pMsgCtx->queue, pReceivedCommand, pdMS_TO_TICKS( blockTimeMs ) );
    }

    return ( queueStatus == pdPASS ) ? true : false;
}
