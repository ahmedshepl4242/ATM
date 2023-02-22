#ifndef CARD_H_
#define CARD_H_

#include <stdio.h>
#include<string.h>
#include <stdlib.h>

#include <time.h>

/* declaration   of typedef */
typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;
/*  typedef declaration  of  Information of Card*/
typedef struct ST_Data_of_card_t
{
    uint8_t cardHolderName[25];
    uint8_t primaryAccountNumber[20];
    uint8_t cardExpirationDate[10];
}ST_Data_of_card_t;

/* Error typedef declaration */
typedef enum EN_cardError_t
{
    CARD_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;


/*   prototypes  of functions  */
EN_cardError_t getCardHolderName(ST_Data_of_card_t* Data_of_card);
EN_cardError_t getCardExpiryDate(ST_Data_of_card_t* Data_of_card);
EN_cardError_t getCardPAN(ST_Data_of_card_t* Data_of_card);
/*extern for global variables*/
extern ST_Data_of_card_t* pointer_to_card;

#endif