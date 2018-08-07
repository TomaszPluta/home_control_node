/*
 * messages.h
 *
 *  Created on: 05.08.2018
 *      Author: tomek
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "stdint.h"

msgDataInt_t CreateMessage(uint8_t * data, uint8_t dataLen, uint8_t type);

#endif /* MESSAGES_H_ */
