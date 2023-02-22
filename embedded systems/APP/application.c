#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS


#include "application.h"


void appStart(void)
{

	EN_cardError_t return_value_name = 0;
	EN_cardError_t return_value_expiry = 0;
	EN_cardError_t return_value_pan = 0;

	/*terminal module local variables*/
	EN_terminalError_t transaction_date = 0;
	EN_terminalError_t card_expiry = 0;
	EN_terminalError_t transaction_amount = 0;
	EN_terminalError_t below_max_amount = 0;
	double  max_drawn_money = 0;

	/* server module local variables */
	EN_transState_t trans_state = 0;



	while (1)
	{
		//block of card.c
		{

			return_value_name = getCardHolderName(pointer_to_card);

			if (return_value_name != CARD_OK) {
				return_value_name = getCardHolderName(pointer_to_card);
				while (return_value_name != CARD_OK) {
					return_value_name = getCardHolderName(pointer_to_card);
				}


			}

			return_value_expiry = getCardExpiryDate(pointer_to_card);

			if (return_value_expiry != CARD_OK) {
				return_value_expiry = getCardExpiryDate(pointer_to_card);
				while (return_value_expiry != CARD_OK) {
					return_value_expiry = getCardExpiryDate(pointer_to_card);
				}


			}
			return_value_pan = getCardPAN(pointer_to_card);

			if (return_value_pan != CARD_OK) {
				return_value_pan = getCardPAN(pointer_to_card);
				while (return_value_pan != CARD_OK) {
					return_value_pan = getCardPAN(pointer_to_card);
				}


			}

		}


		setMaxAmount(terminalptr);

		max_drawn_money = terminalptr->maxTransAmount;

		/* terminal module */

		 getTransactionDate(terminalptr);


		card_expiry = isCardExpired(pointer_to_card, terminalptr);
		if (card_expiry != TERMINAL_OK)
		{
			printf("-----------------------------------------------------------------------------------\n");

			printf("Declined : not valid expired date\n");
			printf("-----------------------------------------------------------------------------------\n");

			continue;
		}


		getTransactionAmount(terminalptr);

    
		below_max_amount = isBelowMaxAmount(terminalptr);

		

	    if (below_max_amount != TERMINAL_OK)
		{
			printf("-----------------------------------------------------------------------------------\n");

			printf(" THE CARD IS DICLINED \"EXCEED MAX AMOUNT\"\n",max_drawn_money);
			printf("-----------------------------------------------------------------------------------\n");

		}
		

		else
		{
			trans_state = recieveTransactionData(transactionsDBptr);
		}
		/* server module */
		
	}

}
