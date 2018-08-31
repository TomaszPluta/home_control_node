/*
 * rfm12b_hal.h
 *
 *  Created on: 31.08.2018
 *      Author: tomek
 */

#ifndef RFM12B_HAL_H_
#define RFM12B_HAL_H_


#define RFM_CS_PIN         GPIO_Pin_1
#define RFM_CS_GPIO        GPIOB

#define RFM_IRQ_PIN        GPIO_Pin_1
#define RFM_IRQ_GPIO       GPIOA

//#define RFM_RST_PIN        GPIO_Pin_2
//#define RFM_RST_GPIO       GPIOA

#define RFM_RST_PIN
#define RFM_RST_GPIO

#define RFM_INT_PIN        //GPIO_Pin_3
#define RFM_INT_GPIO       //GPIOA



#define RFM_CS(x)          GPIO_WriteBit(RFM_CS_GPIO, RFM_CS_PIN, x)
//#define RFM_RST(x)         GPIO_WriteBit(RFM_RST_GPIO, RFM_RST_PIN, x)
#define RFM_RST(x)
#define RFM_IRQ_READ()     GPIO_ReadInputDataBit(RFM_IRQ_GPIO, RFM_IRQ_PIN)


#endif /* RFM12B_HAL_H_ */
