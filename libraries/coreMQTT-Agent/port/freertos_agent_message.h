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
 * @file freertos_agent_message.h
 * @brief Functions to interact with queues.
 */
#ifndef FREERTOS_AGENT_MESSAGE_H
#define FREERTOS_AGENT_MESSAGE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* FreeRTOS includes. */
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

/* Include MQTT agent messaging interface. */
#include "core_mqtt_agent_message_interface.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @ingroup mqtt_agent_struct_types
 * @brief Context with which tasks may deliver messages to the agent.
 */
struct MQTTAgentMessageContext
{
    QueueHandle_t queue;
};

/*-----------------------------------------------------------*/

/**
 * @brief Send a message to the specified context.
 * Must be thread safe.
 *
 * @param[in] pMsgCtx An #MQTTAgentMessageContext_t.
 * @param[in] pData Pointer to element to send to queue.
 * @param[in] blockTimeMs Block time to wait for a send.
 *
 * @return `true` if send was successful, else `false`.
 */
bool Agent_MessageSend( MQTTAgentMessageContext_t * pMsgCtx,
                        MQTTAgentCommand_t * const * pCommandToSend,
                        uint32_t blockTimeMs );

/**
 * @brief Receive a message from the specified context.
 * Must be thread safe.
 *
 * @param[in] pMsgCtx An #MQTTAgentMessageContext_t.
 * @param[in] pBuffer Pointer to buffer to write received data.
 * @param[in] blockTimeMs Block time to wait for a receive.
 *
 * @return `true` if receive was successful, else `false`.
 */
bool Agent_MessageReceive( MQTTAgentMessageContext_t * pMsgCtx,
                           MQTTAgentCommand_t ** pReceivedCommand,
                           uint32_t blockTimeMs );

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* FREERTOS_AGENT_MESSAGE_H */
