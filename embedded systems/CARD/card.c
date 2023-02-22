#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include "card.h"

 

ST_Data_of_card_t card;
ST_Data_of_card_t* pointer_to_card = &card;


 

EN_cardError_t getCardHolderName(ST_Data_of_card_t* Data_of_card)
{
    EN_cardError_t name_error_type = 0;
    uint8_t recieved_name[30] = { 0 };
    uint16_t counter = 0;
  
    printf("Please enter your name : ");
    fscanf(stdin, " ");
    fgets(recieved_name,sizeof(recieved_name), stdin);

     


    counter= strlen(recieved_name);


    if (counter > 20 &&counter < 25)
    {
        name_error_type = CARD_OK;
        strcpy(Data_of_card->cardHolderName, recieved_name);
       
    }

  
    else
    {
        name_error_type = WRONG_NAME;
    }

    

    return name_error_type;

}
EN_cardError_t getCardExpiryDate(ST_Data_of_card_t* Data_of_card)
{
    EN_cardError_t expiry_error = 0;
    uint8_t expiry_storage[100] = {0};
    uint16_t counter = 0,i=0;
    uint8_t month[3] = {0};
    uint8_t year[3] = {0};
    uint16_t max_month_number ;
    uint16_t max_year_number ;
    uint16_t ii=0, jj=0, ll;
    printf("Please enter card expiry_storage date in form month/year : "); // Asking the client to enter card expiry date
   
    fgets(expiry_storage, sizeof(expiry_storage), stdin);  // Take string input from the client
    counter = strlen(expiry_storage);
        expiry_storage[counter] = '\0';


        while (expiry_storage[i] != '\0') {

            if (expiry_storage[i] == '/') {
                break;
            }
            if (expiry_storage[i + 1] == '\0') {
                return  WRONG_EXP_DATE;
            }
            i++;
        }
    while (expiry_storage[ii] != '/') {
        month[ii] = expiry_storage[ii];
        ii++;
    }
    month[ii] = '\0';
    max_month_number = atoi(month);

    ii = counter - 3;
    jj = 0;
    while ((expiry_storage[ii] != '/') && (jj < 2)) {
        year[jj] = expiry_storage[ii];
        ii++;
        jj++;
    }
    year[2] = '\0';
    max_year_number = atoi(year);


    
    if (expiry_storage[2] != '/'|| max_month_number > 12|| counter != 6)
    {
        expiry_error = WRONG_EXP_DATE;
    }
   
    else
    {
        expiry_error = CARD_OK;
        strcpy(Data_of_card->cardExpirationDate, expiry_storage);
       
    }

    return expiry_error;
}
EN_cardError_t getCardPAN(ST_Data_of_card_t* Data_of_card)
{
    EN_cardError_t error_type = 0;
    uint8_t storage_pan[40] = { 0 };
    uint16_t counter = 0;

    printf("Please enter your card's storage_pan : ");  
    
    fgets(storage_pan, sizeof(storage_pan), stdin);  

    counter = strlen(storage_pan);
    storage_pan[counter] = '\0';
    if ( counter >= 16 &&  counter < 21)
    {
        error_type = CARD_OK;
        strcpy(Data_of_card->primaryAccountNumber, storage_pan);
     
    }
    else
    {
        error_type = WRONG_PAN;
    }

    return error_type;

}





