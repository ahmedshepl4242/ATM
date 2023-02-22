#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include "server.h"

/* Global Variables */
ST_accountsDB_t  dataset_test[255] =
{
	{ 1000.0, RUNNING, "12365479456321562" },
	{ 2000.0, BLOCKED, "1313131313131313" },
	{ 3000.0, RUNNING, "1414141414141414" },
	{ 4000.0, BLOCKED, "1515151515151515" },
	{ 5000.0, RUNNING, "1616161616161616" },
	{ 6000.0, BLOCKED, "1417171717171717" },
	{ 6000.0, RUNNING, "1718171617171417" },
	{ 7000.0, BLOCKED, "1717171413141713" },
	{ 11000.0,RUNNING, "1717171713171717" },
	{ 11000.0,BLOCKED, "1917186565656555" },
	{ 120000.0, RUNNING,"12322479456321562" },
	{ 15000.0, BLOCKED, "1345871362489693" },
	{ 16000.0, RUNNING, "1414142463148794" },
	{ 4000.0, BLOCKED,  "1236547899521565" },
	{ 35000.0, RUNNING, "14744854135749616" },
	{ 6000.0, BLOCKED,  "14173214564711717" },
	{ 80000.0, RUNNING, "1718171617155417" },
	{ 77000.0, BLOCKED, "171717141417813" },
	{ 111100.0,RUNNING, "1717171317176617" },
	{ 11010.0,BLOCKED,  "1917177717171719" }
};
ST_accountsDB_t* accountsDBptr = dataset_test; // to search for a valid account
ST_accountsDB_t* pointerToAccount_in_dataset =0; // to store the address of the valid account

ST_transaction_t transactionsDB[255] = { 0 };
ST_transaction_t* transactionsDBptr = transactionsDB;
ST_transaction_t* transaction_reference = 0; // the pointer used to access transactionsDB array

EN_transState_t transaction_state = 0;
uint32_t transactionsDB_index = 0; // index to be added on transactionDBptr to access transactionsDB array
EN_accountState_t state;


EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_transState_t trans_error_type = 0;
	EN_serverError_t account_validity = 0;
	EN_serverError_t account_state = 0;
	EN_serverError_t amount_availability = 0;
	EN_serverError_t transaction_saved = 0;
	ST_transaction_t* save_transaction_ptr = transData;

	account_validity = isValidAccount(pointer_to_card, accountsDBptr);
	account_state = isBlockedAccount(pointerToAccount_in_dataset);
	amount_availability = isAmountAvailable(terminalptr, pointerToAccount_in_dataset);

	if (account_validity == ACCOUNT_NOT_FOUND)
	{
		trans_error_type = FRAUD_CARD;
		transaction_state = FRAUD_CARD;
	}
	else if (account_state == BLOCKED_ACCOUNT)
	{
		trans_error_type = DECLINED_STOLEN_CARD;
		transaction_state = DECLINED_STOLEN_CARD;
	}
	else if (amount_availability == LOW_BALANCE)
	{
		trans_error_type = DECLINED_INSUFFECIENT_FUND;
		transaction_state = DECLINED_INSUFFECIENT_FUND;
	}
	
	else 
	{
		trans_error_type = APPROVED;
		transaction_state = APPROVED;
		pointerToAccount_in_dataset->balance = pointerToAccount_in_dataset->balance - terminalptr->transAmount;
	}

	transaction_saved = saveTransaction(save_transaction_ptr);
	if (transaction_saved != SERVER_OK)
	{
		trans_error_type = INTERNAL_SERVER_ERROR;
		transaction_state = INTERNAL_SERVER_ERROR;
	}

	return trans_error_type;
}

EN_serverError_t isValidAccount(ST_Data_of_card_t* Data_of_card, ST_accountsDB_t* dataset)
{
	uint16_t i;
	EN_serverError_t server_error = 0;
	uint16_t ii = 0, jj = 0,ll1=0,ll2=0;
	 
	pointerToAccount_in_dataset = 0;
	uint8_t part1_input_pan[11] = { 0 };
	uint8_t part2_input_pan[11]= { 0 };
	uint8_t part1_storege_pan[11]= { 0 };
	uint8_t part2_storege_pan[11]= { 0 };

	uint32_t part1_number_input;
	uint32_t part2_number_input;
	uint32_t part1_number_storege;
	uint32_t part2_number_storege;


	
	while (ii < 10) {
		part1_input_pan[ii] = Data_of_card->primaryAccountNumber[ii];
		ii++;
	}
	part1_input_pan[ii] = '\0';
	part1_number_input = atoi(part1_input_pan);

	while (Data_of_card->primaryAccountNumber[ii]!='\0'&&(jj<10)) {
		part2_input_pan[jj] = Data_of_card->primaryAccountNumber[ii];
		ii++;
		jj++;
	}
	part2_input_pan[jj] = '\0';
	part2_number_input = atoi(part2_input_pan);

	for (i = 0; i < number_of_accounts; i++) 
	{

		jj = 0;
		ii = 0;

		while (ii < 10) {
			part1_storege_pan[ii] = dataset->primaryAccountNumber[ii];
			ii++;
		}
		part1_storege_pan[ii] = '\0';
		part1_number_storege = atoi(part1_storege_pan);

		while (dataset->primaryAccountNumber[ii] != '\0' && (jj < 10)) {
			part2_storege_pan[jj] = dataset->primaryAccountNumber[ii];
			ii++;
			jj++;
		}
		part2_storege_pan[jj] = '\0';
		part2_number_storege = atoi(part2_storege_pan);


		if (part1_number_input != part1_number_storege) {
			server_error = ACCOUNT_NOT_FOUND;
		}

		else if (part2_number_input != part2_number_storege) {
			server_error = ACCOUNT_NOT_FOUND;
		}

		else {
			server_error = SERVER_OK;
			pointerToAccount_in_dataset= dataset++;
			break;
		}

		dataset++;
	}






	return  server_error;

}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* dataset)
{
	EN_serverError_t server_error = 0;
	if (dataset!=0) {
		if (dataset->state != RUNNING)
		{
			server_error = BLOCKED_ACCOUNT;
		}
		else
		{
			server_error = SERVER_OK;
		}


	  }
	return  server_error;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* dataset)
{
	   EN_serverError_t server_error = 0;
	
	   if (dataset!=0) {
		   if (termData->transAmount > dataset->balance)
		   {
			   server_error = LOW_BALANCE;
		   }
		   else
		   {
			   server_error = SERVER_OK;
		   }



	   }
		

	    return  server_error;

}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	EN_serverError_t server_error = SERVER_OK;

	transData = transData + transactionsDB_index;
	transaction_reference = transData;

	strcpy(transData->cardHolderData.cardHolderName, pointer_to_card->cardHolderName);
	strcpy(transData->cardHolderData.cardExpirationDate, pointer_to_card->cardExpirationDate);
	strcpy(transData->cardHolderData.primaryAccountNumber, pointer_to_card->primaryAccountNumber);

	strcpy(transData->terminalData.transactionDate, terminalptr->transactionDate);
	transData->terminalData.transAmount = terminalptr->transAmount;
	transData->terminalData.maxTransAmount = terminalptr->maxTransAmount;

	transData->transState = transaction_state;

	transData->transactionSequenceNumber = 1 + transactionsDB_index;

	listSavedTransactions();

	transactionsDB_index++;

	return server_error;
}

void listSavedTransactions()
{
	printf("----------------------------------------------------------------------\n");
	 
	printf("Transaction Sequence Number: %ld\n", transaction_reference->transactionSequenceNumber);

	printf("Transaction Date: %s\n", transaction_reference->terminalData.transactionDate);

	printf("Transaction Amount: %f\n", transaction_reference->terminalData.transAmount);
	 
	

	if (transaction_reference->transState == APPROVED) {
		printf("The card is  Approved\n");
	}

	else if (transaction_reference->transState == DECLINED_INSUFFECIENT_FUND) {
		printf("the card is DICLINED\n REASON:DECLINED_INSUFFECIENT_FUND\n");
	}
	else if (transaction_reference->transState == DECLINED_STOLEN_CARD) {
		printf("the card is DICLINED\n REASON:DECLINED_STOLEN_CARD\n");
	}
	
	else if (transaction_reference->transState == FRAUD_CARD) {
		printf("the card is DICLINED\n REASON:FRAUD_CARD\n");
	}
	else if (transaction_reference->transState == INTERNAL_SERVER_ERROR) {
		printf("the card is DICLINED\n REASON:INTERNAL_SERVER_ERROR\n");
	}
	if (transaction_reference->transState == APPROVED) 
	{
		printf("new balance  Amount is: %f\n", pointerToAccount_in_dataset->balance);
	}
	printf("Terminal Max Amount: %f\n", transaction_reference->terminalData.maxTransAmount);

	printf("Cardholder Name: %s\n", transaction_reference->cardHolderData.cardHolderName);

	printf("storage_pan: %s\n", transaction_reference->cardHolderData.primaryAccountNumber);
	
	printf("Card Expiration Date: %s\n", transaction_reference->cardHolderData.cardExpirationDate);
	
	printf("-----------------------------------------------------------------------------------\n");
	
}