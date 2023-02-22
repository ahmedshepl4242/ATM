#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#include "terminal.h"

ST_terminalData_t terminal;
ST_terminalData_t* terminalptr = &terminal;



EN_terminalError_t  getTransactionDate(ST_terminalData_t* termData)
{
	

	uint8_t transaction[50];
	uint8_t month[3];
	uint8_t year[3];
	
	uint16_t month_number = 0;
	uint16_t year_number = 0;
	uint16_t jj = 0;
	uint16_t ll = 0;
	uint16_t ii = 0;

	time_t now = time(&now);
	struct tm* dt = localtime(&now);
	strftime(transaction, sizeof(transaction), "%d/%m/%y", dt);
	


	ll = strlen(transaction);
	transaction[ll] = '\0';


	
	ii = 0;

	while (transaction[ii] != '/') {
		month[ii] = transaction[ii];
		ii++;
	}
	month[ii] = '\0';
	month_number = atoi(month);
	
	ii = ll - 2;




	while ((transaction[ii] != '/') && (jj < 2)) {
		year[jj] = transaction[ii];
		ii++;
		jj++;
	}
	year[2] = '\0';
	year_number = atoi(year);

	strcpy(termData->transactionDate, transaction);
	terminalptr->transactionDate[ll] = '\0';

	
}

EN_terminalError_t isCardExpired(ST_Data_of_card_t* Data_of_card, ST_terminalData_t* termData)
{
	EN_terminalError_t error_expired = 0;
	uint16_t ii = 0,jj=0,ll=0;

	uint8_t month_trans[3];
	uint8_t year_trans[3];

	uint16_t month_number_trans = 0;
	uint16_t year_number_trans = 0;
	uint8_t month_expiry[3];
	uint8_t year_expiry[3];

	uint16_t month_number_expiry = 0;
	uint16_t year_number_expiry = 0;

	


	ll = strlen(termData->transactionDate);
	while (termData->transactionDate[ii] != '/') {
		month_trans[ii] = termData->transactionDate[ii];
		ii++;
	}
	month_trans[ii] = '\0';
	month_number_trans = atoi(month_trans);

	 



	ii = ll - 2;
	while ((termData->transactionDate[ii] != '/') && (jj < 2)) {
		year_trans[jj] = termData->transactionDate[ii];
		ii++;
		jj++;
	}
	year_trans[2] = '\0';
	year_number_trans = atoi(year_trans);





	ii = 0,jj=0;
	ll = strlen(Data_of_card->cardExpirationDate);
	while (Data_of_card->cardExpirationDate[ii] != '/') {
		month_expiry[ii] = Data_of_card->cardExpirationDate[ii];
		ii++;
	}
	month_expiry[ii] = '\0';
	month_number_expiry = atoi(month_expiry);  


    ii = ll - 3;
	while ((Data_of_card->cardExpirationDate[ii] != '/') && (jj < 2)) {
		year_expiry[jj] = Data_of_card->cardExpirationDate[ii];
		ii++;
		jj++;
	}
	year_expiry[2] = '\0';
	year_number_expiry = atoi(year_expiry);

	



	if(year_number_trans<year_number_expiry){
	error_expired=TERMINAL_OK;
	}

	else if(year_number_trans>year_number_expiry){
	error_expired=EXPIRED_CARD;
	}

	else{

	
	    if ( month_number_trans<= month_number_expiry)
		{
			 	error_expired=TERMINAL_OK;
		}
		else 
		{
			 error_expired=EXPIRED_CARD;
		}

	}
	
	return error_expired;

}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{

	double money_of_transaction = 0.0;
	uint16_t uu= 0;
	printf("Enter the amount of money : ");
	scanf_s("%lf", &money_of_transaction);

	while (money_of_transaction < 0) {

		printf("please enter postive number of money : ");
		scanf_s("%lf", &money_of_transaction);
		if (money_of_transaction >= 0) {
			break;
		}
	}

	termData->transAmount = money_of_transaction;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	EN_terminalError_t below_max_amout_error = 0; 
	
	
	uint16_t uu = 0;
	 
	 
	if (termData->transAmount <= termData->maxTransAmount) {
		below_max_amout_error = TERMINAL_OK;
		return below_max_amout_error;

			}

	else {
		below_max_amout_error = EXCEED_MAX_AMOUNT;

		return below_max_amout_error;
	}


	
	
	
	
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	EN_terminalError_t  max_amount_error = 0;


	double max_drawn_money = 0.0;
	uint16_t uu = 0;
	printf("Enter the max amont  of drawn  money : ");
	scanf_s("%lf", &max_drawn_money);

	
		
	while (max_drawn_money < 0) {

		printf("please enter postive number of money : ");
		scanf_s("%lf", &max_drawn_money);
		if (max_drawn_money >= 0) {
			break;
		}
	 }

	termData->maxTransAmount = max_drawn_money;

	return max_amount_error;

}

