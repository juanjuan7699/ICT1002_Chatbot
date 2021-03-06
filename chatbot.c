/*
 * ICT1002 Assignment 2, 2018-19 Trimester 2.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */
 
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chat1002.h"

 
/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname() {

	return "Javis";
	
}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username() {

	return "User";
	
}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char *inv[], char *response, int n) {
	
	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, "");
		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_smalltalk(inv[0]))
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}

}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char *intent) {
	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0 || compare_token(intent, "goodbye") == 0;
	
}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n) {
	knowledge_reset();
	snprintf(response, n, "Goodbye!");
	 
	return 1;
	 
}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  0, if the intent is "load"
 *  1, otherwise
 */
int chatbot_is_load(const char *intent) {

	if (compare_token(intent, "load")){	// Check if Intent is to LOAD
		return 0;					// Return 1 if Intent is to LOAD
	}
	else {
		return 1;						// Return 0 if intent not to LOAD
	}
}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0  (the chatbot always continues chatting after loading knowledge, even if file not found
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n) {
	int linecount = 0;

    if (inv[1] == NULL){
        strcpy(response,"No file inputted!");					// Error Response for No Input for File
    }
    else {
        FILE *file;
        
        if (file = fopen(inv[1], "r")){							// Open File for Reading
        	linecount = knowledge_read(file);					// Get number of entries inserted
            fclose(file);										// Close file pointer
           	snprintf(response, n,"Read %d responses from %s", linecount, inv[1]);
        }
        else{
            strcpy(response,"File not found!");					// Error response for File Not Found
        }
    }
    return 0;
	 
}


/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char *intent) {
	
	if (compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 || compare_token(intent, "who") == 0) {
		return 1;
	}
	
	return 0;
	
}


/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char *inv[], char *response, int n) {
	
	char userintent[MAX_INTENT];				/* Define a char array to store input intent of size MAX_INTENT */
	strncpy(userintent, inv[0], sizeof(userintent)/sizeof(userintent[0]));

	char usernoun[MAX_INPUT];					/* Define a char array to store input noun */
	strcpy(usernoun, "\0");

	char userentity[MAX_ENTITY];				/* Define a char array to store input entity of size MAX_ENTITY */
	snprintf(userentity, n, "");				/* Formats entity portion */

	int get_reply_code;							/* Define an int to store flag of knowlede_get */
	char chatbot_entity[MAX_RESPONSE];			/* Define a char array to store chatbot response from knowledge_get of size MAX_RESPONSE */
	char userresponse_notfound[MAX_RESPONSE];	/* Define a char array to store user input of size MAX_RESPONSE */

	int put_reply_code;							/* Define an int to store flag of knowledge_put */


	/* Validation: Checks if Intent is accompanied by entity */
	if (inc == 1) {
		if (compare_token(userintent, "what") == 0) {
			snprintf(response, n, "Sorry, I did not understand the phrase. Did you mean something like, \"What is ICT?\"");
		} else if (compare_token(userintent, "who") == 0) {
			snprintf(response, n, "Sorry, I did not understand the phrase. Did you mean something like, \"Who is the cluster director of ICT?\"");
		} else if (compare_token(userintent, "where") == 0) {
			snprintf(response, n, "Sorry, I did not understand the phrase. Did you mean something like, \"Where is SIT?\"");
		}
		return 0;
	} else if ((inc == 2 && compare_token(inv[1], "is") == 0) || (inc == 2 && compare_token(inv[1], "are") == 0)) {
		snprintf(response, n, "Sorry, I did not understand the phrase. Please describe your noun.");
		return 0;
	}


	/* Simple Validation */
	for (int i=1; i<inc; i++) {
		/* Checks for the nouns "is" or "are" */
		if ((i == 1 && compare_token(inv[i], "is") == 0) || (i == 1 && compare_token(inv[i], "are") == 0)) {
			strncpy(usernoun, inv[i], sizeof(inv[i])/sizeof(inv[i][0]));						/* Store into usernoun */
			continue;
		}
		strcat(strcat(userentity, " "), inv[i]);		/* Store into userentity */
	}
	memmove(userentity, userentity+1, strlen(userentity));	/* Formats entity portion */


	/* Calls knowledge_get and perform checks for various return values */
	get_reply_code = knowledge_get(userintent, userentity, chatbot_entity, n);	/* Arguments: Intent, Entity, Buffer to store response from knowledge */
	if (get_reply_code == KB_OK) {												/* If a response was found for the intent and entity, */
		snprintf(response, n, "%s", chatbot_entity);							/* 	the response is copied to the response buffer. */

	} else if (get_reply_code == KB_NOTFOUND) {																			/* If no response could be found, */
		if (compare_token(usernoun, "\0") == 0) {
			prompt_user(userresponse_notfound, MAX_INPUT, "I don't know. %s %s?", userintent, userentity);				/*	asks for user input IF usernoun is not declared, then call knowledge_put. */
		} else {
			prompt_user(userresponse_notfound, MAX_INPUT, "I don't know. %s %s %s?", userintent, usernoun, userentity);	/*	asks for user input IF usernoun is declared, then call knowledge_put. */
		}

		if (strcmp(userresponse_notfound, "") == 0){
			strcpy(response, "-(");
		} else {

			/* Calls knowledge_put to insert user response into knowledge base */
			put_reply_code = knowledge_put(userintent, userentity, userresponse_notfound);		/* Arguments: Intent, Entity, Buffer to store user input */

			if (put_reply_code == KB_OK) {				/* If knowledge_put is successful */
				snprintf(response, n, "Thank you.");
			} else if (put_reply_code == KB_NOMEM) {	/* Else if there is insufficient memory */
				snprintf(response, n, "Memory allocation failure! Failed to create note for:\nIntent '%s'\nEntity '%s'\nResponse '%s'\n",userintent, userentity, userresponse_notfound);
				exit(1);
			} else if (put_reply_code == KB_INVALID) {	/* Else if the intent is not valid */
				snprintf(response, n, "Sorry, I didn't get '%s'.", userintent);
			}
		}

	} else if (get_reply_code == KB_INVALID) {
		snprintf(response, n, "Sorry, I didn't get '%s'.", userintent);
	}

	
	return 0;
	 
}


/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char *intent) {
	
	if (compare_token(intent, "reset") == 0) {
		return 1;
	}
	
	return 0;
	
}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n) {
	knowledge_reset();
	snprintf(response, n, "Chatbot reset.");
	return 0;
	int reset_count = 0;

	/* ------------------------------- If 'WHO' linked-list is not empty (headofWHO not pointing to NULL) ---------------------------------- */
    if (headofWHO!=NULL) {
        /* While not end of linked-list, delete/free memory of node pointed by headofWHO currently */
        response_node *temp = headofWHO;
        do {
            printf("\nRemoving Node '%s' '%s\n'",temp->intent,temp->entity);
            temp = temp->next;
            free(headofWHO);                        /* Free memory allocation of node currently pointed to by headofWHO */
            headofWHO=temp;              /* Point headofWHO to the next node (will point to NULL if current node is last in the list) */
        } while (temp != NULL);
        reset_count++;
    }

    /* ------------------------------ If 'WHAT' linked-list is not empty (headofWHAT not pointing to NULL) --------------------------------- */
    if (headofWHAT!=NULL) {
        /* While not end of linked-list, delete/free memory of node pointed by headofWHAT currently */
        response_node *temp = headofWHAT;
        do {
            printf("\nRemoving Node '%s' '%s\n'",temp->intent,temp->entity);
            temp = temp->next;
            free(headofWHAT);                        /* Free memory allocation of node currently pointed to by headofWHAT */
            headofWHAT=temp;             /* Point headofWHAT to the next node (will point to NULL if current node is last in the list) */
        } while (temp != NULL);
        reset_count++;
    }

    /* ------------------------------ If 'WHERE' linked-list is not empty (headofWHERE not pointing to NULL) --------------------------------- */
    if (headofWHERE!=NULL) {
        /* While not end of linked-list, delete/free memory of node pointed by headofWHERE currently */
        response_node *temp = headofWHERE;
        do {
            printf("\nRemoving Node '%s' '%s\n'",temp->intent,temp->entity);
            temp = temp->next;
            free(headofWHERE);                       /* Free memory allocation of node currently pointed to by headofWHERE */
            headofWHERE=temp;           /* Point headofWHERE to the next node (will point to NULL if current node is last in the list) */
        } while (temp != NULL);
        reset_count++;
    }

    if (reset_count == 0) {
    	snprintf(response, n, "There is nothing to reset.");
    } else {
    	snprintf(response, n, "Chatbot reset.");
    }

	return 0;
}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  0, if the intent is "what", "where", or "who"
 *  1, otherwise
 */
int chatbot_is_save(const char *intent) {
	
	if (compare_token(intent,"save")){       // Check if Intent is to SAVE
        return 0;                           // Return 0 if Intent is to SAVE
    }
    else {
        return 1;                      // Return -2 if intent not to SAVE
    }
	
	
}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char *inv[], char *response, int n) {
	
	char filename[64];

    if (inv[1] == NULL){
        strcpy(response,"No file inputted!");                   // Error Response for No Input for File
    }
    else if (headofWHERE == NULL && headofWHAT == NULL && headofWHO == NULL) {
    	strcpy(response, "Database is empty!");
    }
    else{

    	strcpy(filename, inv[1]);

    	int num;
   		FILE *fptr;
   		fptr = fopen(filename, "w");
    	if(fptr == NULL){  
			strcpy(response,"Error creating file");             
		}
		else {
			knowledge_write(fptr);
    		snprintf(response, n, "My knowledge has been saved to %s.", filename);
		}

		fclose(fptr);
		


    }
	
	return 0;
	 
}
 
 
/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char *intent) {
	
	char *smalltalk[] = {"Hi", "Sup", "Hey", "Howdy", "Yo", "Hiya", "Greetings", "Hello"};
	size_t smalltalkLength = sizeof(smalltalk) / sizeof(smalltalk[0]);
	for(int i = 0; i< smalltalkLength; i++)
	{
		if(compare_token(intent,smalltalk[i]) == 0)
		{
			return 1;
		}
	}
	
	return 0;
 
}


/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n) {
	
	char *smalltalkOutput[] = {"Hi, Welcome to ICT1002 Chatbot programmed by Jordan, Xiu Qi, Jun Ming, Dominic and Guang Jun", "What's up?",  "Hi!", "Sup!", "Hey!", "Howdy!", "Yo!", "Hiya!", "Greetings!"};
	size_t smalltalkLength = sizeof(smalltalkOutput) / sizeof(smalltalkOutput[0]);
	int randomOutput = rand() % smalltalkLength;
	snprintf(response, n, smalltalkOutput[randomOutput]);
		
    
	
	return 0;
	
}