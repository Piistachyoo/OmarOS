/*************************************************************************/
/* Author        : Omar Yamany                                    		 */
/* Project       : String_Library                                    	 */
/* File          : string_lib.c                           				 */
/* Date          : Jun 22, 2023                                          */
/* Version       : V1                                                    */
/* GitHub        : https://github.com/Piistachyoo             		     */
/*************************************************************************/

#include "string_lib.h"

/**=============================================
  * @Fn				- STRING_convert_upperCase
  * @brief 			- Converts all alphabetical characters in string "str" to upper case
  * @param [in] 	- str: Pointer to string to be converted to upper case
  * @param [out] 	- None
  * @retval 		- None
  * Note			- None
  */
void STRING_convert_upperCase(unsigned char* str){
	/* Validate that we are not accessing a null pointer */
	if(NULL != str){
		/* Loop on array characters until finding a terminating null */
		while(*str){
			/* If lower case character is found convert it to upper case */
			if(('a' <= *str) && ('z' >= *str)){
				*str -= 32;
			}
			else{ /* Do Nothing */ }
			str++;
		}
	}
	else{ /* Do Nothing */ }
}

/**=============================================
  * @Fn				- STRING_convert_lowerCase
  * @brief 			- Converts all alphabetical characters in string "str" to lowercase
  * @param [in] 	- str: Pointer to string to be converted to lowercase
  * @param [out] 	- None
  * @retval 		- None
  * Note			- None
  */
void STRING_convert_lowerCase(unsigned char* str){
	/* Validate that we are not accessing a null pointer */
	if(NULL != str){
		/* Loop on array characters until finding a terminating null */
		while(*str){
			/* If an upper case character is found convert it to lower case */
			if(('A' <= *str) && ('Z' >= *str)){
				*str += 32;
			}
			else{ /* Do Nothing */ }
			str++;
		}
	}
	else{ /* Do Nothing */ }
}

/**=============================================
  * @Fn				- STRING_set_memoryLocation
  * @brief 			- Initializes a specified number of bytes to null or any other value
  * @param [in] 	- str  : Pointer to the memory location to be initialized
  * @param [in] 	- value: Value to be initialized to every byte
  * @param [in] 	- size : Number of bytes to be initialized to equal "value"
  * @param [out] 	- None
  * @retval 		- None
  * Note			- None
  */
void STRING_set_memoryLocation(unsigned char* str, unsigned char value, unsigned short size){
	/* Validate that we are not accessing a null pointer */
	if(NULL != str){
		unsigned int index;
		/* Set every byte of array "str" to "value" */
		for(index = 0; index < size; index++){
			*str++ = value;
		}
	}
	else{ /* Do Nothing */ }
}

/**=============================================
  * @Fn				- STRING_concatenate
  * @brief 			- Concatenates "src_str" at the end of "des_str"
  * @param [in] 	- src_str: Pointer to the array containing the source string
  * @param [in] 	- des_str: Pointer to the array containing the destination string
  * @param [out] 	- None
  * @retval 		- None
  * Note			- None
  */
void STRING_concatenate(const unsigned char* src_str, unsigned char* des_str){
	/* Validate that we are not accessing a null pointer */
	if( (NULL != src_str) && (NULL != des_str) ){
		/* Navigate to the end of "des_str" */
		while(*des_str){
			des_str++;
		}

		/* Add "src_str" to the end of "des_str" */
		while(*src_str){
			*des_str++ = *src_str++;
		}

		/* Add terminating null character */
		*des_str = '\0';
	}
	else{ /* Do Nothing */ }
}

/**=============================================
  * @Fn				- STRING_concatenate_length
  * @brief 			- Concatenates part of "src_str" at the end of "des_str"
  * @param [in] 	- src_str: Pointer to the array containing the source string
  * @param [in] 	- des_str: Pointer to the array containing the destination string
  * @param [in] 	- length :  Number of characters to be concatenated at the end of "des_str"
  * @param [out] 	- None
  * @retval 		- None
  * Note			- None
  */
void STRING_concatenate_length(const unsigned char* src_str, unsigned char* des_str, const unsigned short length){

	/* Validate that we are not accessing a null pointer */
	if( (NULL != src_str) && (NULL != des_str) ){
		unsigned short index;

		/* Navigate to the end of "des_str" */
		while(*des_str){
			des_str++;
		}

		/* Add first "length" characters of "src_str" to the end of "des_str" */
		for(index = 0; (index < length) && (*src_str); index++){
			*des_str++ = *src_str++;
		}

		/* Add terminating null character */
		*des_str = '\0';
	}
	else{ /* Do Nothing */ }
}

/**=============================================
  * @Fn				- STRING_char_firstOccurrence
  * @brief 			- Returns pointer to first occurrence of "value" in "str"
  * @param [in] 	- str	 : Pointer to the array containing a string
  * @param [in] 	- value  : Value to search for in "str"
  * @param [out] 	- None
  * @retval 		- Pointer to the location of the first occurrence of "value" in "str"
  * Note			- Returns NULL if character "value" was not found in string "str"
  */
unsigned char* STRING_char_firstOccurrence(const unsigned char* str, const unsigned char value){
	unsigned char *ret_val = NULL;

	/* Validate that we are not accessing a null pointer */
	if(NULL != str){
		/* Search for "value" in "str" */
		while(*str){
			if(value == *str){
				ret_val = (unsigned char*)str;
				break;
			}
			else{ /* Do Nothing */ }
			str++;
		}
	}
	else{ /* Do Nothing */ }
	return ret_val;
}

/**=============================================
  * @Fn				- STRING_char_lastOccurrence
  * @brief 			- Returns pointer to last occurrence of "value" in "str"
  * @param [in] 	- str	 : Pointer to the array containing a string
  * @param [in] 	- value  : Value to search for in "str"
  * @param [out] 	- None
  * @retval 		- Pointer to the location of the last occurrence of "value" in "str"
  * Note			- Returns NULL if character "value" was not found in string "str"
  */
unsigned char* STRING_char_lastOccurrence(const unsigned char* str, const unsigned char value){
	unsigned char *ret_val = NULL;

	/* Validate that we are not accessing a null pointer */
	if(NULL != str){
		/* Search for "value" in "str" */
		while(*str){
			if(value == *str){
				ret_val = (unsigned char*)str;
			}
			else{ /* Do Nothing */ }
			str++;
		}
	}
	else{ /* Do Nothing */ }
	return ret_val;
}

/**=============================================
  * @Fn				- STRING_compare_caseSensetive
  * @brief 			- Checks if two strings are identical (case sensitive)
  * @param [in] 	- str1: Pointer to the first string
  * @param [in] 	- str2: Pointer to the second string
  * @param [out] 	- None
  * @retval 		- Returns 0 if str1 is same as str2. Returns -1 if str1 < str2. Returns 1 if str1 > str2
  * Note			- None
  */
signed char STRING_compare_caseSensitive(const unsigned char* str1, const unsigned char* str2){
	signed char ret_val = 0;
	/* Validate that we are not accessing a null pointer */
	if( (NULL != str1) && (NULL != str2) ){
		/* Navigate in the two strings */
		while( ('\0' != *str1) && ('\0' != *str2) ){
			/* Check if two characters are identicals */
			if(*str1 != *str2){
				if(*str1 < *str2){
					ret_val = -1;
				}
				else{
					ret_val = 1;
				}
				break;
			}
			else{ /* Do Nothing */ }
		}
	}
	else{ /* Do Nothing */ }
	return ret_val;
}

/**=============================================
  * @Fn				- STRING_compare_caseInsensetive
  * @brief 			- Checks if two strings are identical (case insensitive)
  * @param [in] 	- str1: Pointer to the first string
  * @param [in] 	- str2: Pointer to the second string
  * @param [out] 	- None
  * @retval 		- Returns 0 if str1 is same as str2. Returns -1 if str1 < str2. Returns 1 if str1 > str2
  * Note			- None
  */
signed char STRING_compare_caseInsensitive(const unsigned char* str1, const unsigned char* str2){
	signed char ret_val = 0;
	/* Validate that we are not accessing a null pointer */
	if( (NULL != str1) && (NULL != str2) ){
		unsigned char temp_compare_value;
		/* Navigate in the two strings */
		while( ('\0' != *str1) && ('\0' != *str2) ){
			/* Check if two characters are identicals */
			if(*str1 != *str2){
				/* Check if *str1 is just the other case character of *str2 */
				if( ('A' <= *str1) && ('Z' >= *str1) ){
					/* If *str1 is upper case, convert to lower case and recheck if identical to *str2 */
					temp_compare_value = *str1 + 32;
					if(*str2 == temp_compare_value){
						/* Character is the same but different in case */
						continue;
					}
					else{
						/* Character is different */
						if(*str1 < *str2){
							ret_val = -1;
						}
						else{
							ret_val = 1;
						}
						break;
					}
				}
				else if( ('a' <= *str1) && ('z' >= *str1) ){
					/* If *str1 is lower case, convert to upper case and recheck if identical to *str2 */
					temp_compare_value = *str1 - 32;
					if(*str2 == temp_compare_value){
						/* Character is the same but different in case */
						continue;
					}
					else{
						/* Character is different */
						if(*str1 < *str2){
							ret_val = -1;
						}
						else{
							ret_val = 1;
						}
						break;
					}
				}
				else{
					/* Characters are not an alphabetic and are different */
					if(*str1 < *str2){
						ret_val = -1;
					}
					else{
						ret_val = 1;
					}
					break;
				}
			}
			else{ /* Do Nothing */ }
		}
	}
	else{ /* Do Nothing */ }
	return ret_val;
}

/**=============================================
  * @Fn				- STRING_compare_caseSensitive_length
  * @brief 			- Checks if two strings are identical up to length "length" (case sensitive)
  * @param [in] 	- str1  : Pointer to the first string
  * @param [in] 	- str2  : Pointer to the second string
  * @param [in] 	- length: Length of characters to check
  * @param [out] 	- None
  * @retval 		- Returns 0 if str1 is same as str2 for length "length". Returns -1 if str1 < str2. Returns 1 if str1 > str2
  * Note			- None
  */
signed char STRING_compare_caseSensitive_length(const unsigned char* str1, const unsigned char* str2, const unsigned short length){
	signed char ret_val = 0;
	/* Validate that we are not accessing a null pointer */
	if( (NULL != str1) && (NULL != str2) ){
		unsigned short index;
		/* Navigate in the two strings */
		for(index = 0; (index < length) && ('\0' != *str1) && ('\0' != *str2); index++){
			/* Check if two characters are identicals */
			if(str1[index] != str2[index]){
				if(str1[index] < str2[index]){
					ret_val = -1;
				}
				else{
					ret_val = 1;
				}
				break;
			}
			else{ /* Do Nothing */ }
		}
	}
	else{ /* Do Nothing */ }
	return ret_val;
}

/**=============================================
  * @Fn				- STRING_compare_caseInsensitive_length
  * @brief 			- Checks if two strings are identical up to length "length" (case insensitive)
  * @param [in] 	- str1  : Pointer to the first string
  * @param [in] 	- str2  : Pointer to the second string
  * @param [in] 	- length: Length of characters to check
  * @param [out] 	- None
  * @retval 		- Returns 0 if str1 is same as str2 for length "length". Returns -1 if str1 < str2. Returns 1 if str1 > str2
  * Note			- None
  */
signed char STRING_compare_caseInsensitive_length(const unsigned char* str1, const unsigned char* str2, const unsigned short length){
	signed char ret_val = 0;
	/* Validate that we are not accessing a null pointer */
	if( (NULL != str1) && (NULL != str2) ){
		unsigned char temp_compare_value;
		unsigned short index;
		/* Navigate in the two strings */
		for(index = 0; (index < length) && ('\0' != *str1) && ('\0' != *str2); index++){
			/* Check if two characters are identicals */
			if(str1[index] != str2[index]){
				/* Check if str1[index] is just the other case character of str2[index] */
				if( ('A' <= str1[index]) && ('Z' >= str1[index]) ){
					/* If str1[index] is upper case, convert to lower case and recheck if identical to str2[index] */
					temp_compare_value = str1[index] + 32;
					if(str2[index] == temp_compare_value){
						/* Character is the same but different in case */
						continue;
					}
					else{
						/* Character is different */
						if(str1[index] < str2[index]){
							ret_val = -1;
						}
						else{
							ret_val = 1;
						}
						break;
					}
				}
				else if( ('a' <= str1[index]) && ('z' >= str1[index]) ){
					/* If str1[index] is lower case, convert to upper case and recheck if identical to str2[index] */
					temp_compare_value = str1[index] - 32;
					if(str2[index] == temp_compare_value){
						/* Character is the same but different in case */
						continue;
					}
					else{
						/* Character is different */
						if(str1[index] < str2[index]){
							ret_val = -1;
						}
						else{
							ret_val = 1;
						}
						break;
					}
				}
				else{
					/* Characters are not an alphabetic and are different */
					if(str1[index] < str2[index]){
						ret_val = -1;
					}
					else{
						ret_val = 1;
					}
					break;
				}
			}
			else{ /* Do Nothing */ }
		}
	}
	else{ /* Do Nothing */ }
	return ret_val;
}

/**=============================================
  * @Fn				- STRING_copy
  * @brief 			- Copies a string from "src_str" into "des_str"
  * @param [in] 	- src_str: Pointer to the source array that contains a string to be copied
  * @param [in] 	- des_str: Pointer to the destination array
  * @param [out] 	- None
  * @retval 		- Length of the copied string
  * Note			- None
  */
unsigned short STRING_copy(const unsigned char* src_str, unsigned char* des_str){
	unsigned short index = 0;
	/* Validate that we are not accessing a null pointer */
	if( (NULL != src_str) && (NULL != des_str) ){
		for(; '\0' != src_str[index]; index++){
			des_str[index] = src_str[index];
		}
		des_str[index] = '\0';
	}
	else{ /* Do Nothing */ }
	return index;
}

/**=============================================
  * @Fn				- STRING_copy_length
  * @brief 			- Copies a string with length "length" from "src_str" into "des_str"
  * @param [in] 	- src_str: Pointer to the source array that contains the string to be copied
  * @param [in] 	- des_str: Pointer to the destination array
  * @param [in] 	- length : Length of string in "src_str"
  * @param [out] 	- None
  * @retval 		- None
  * Note			- None
  */
void STRING_copy_length(const unsigned char* src_str, unsigned char* des_str, const unsigned short length){
	/* Validate that we are not accessing a null pointer */
	if( (NULL != src_str) && (NULL != des_str) ){
		unsigned short index;
		for(index = 0; ((index < length) && ('\0' != src_str[index])) ; index++){
			des_str[index] = src_str[index];
		}
	}
	else{ /* Do Nothing */ }
}

/**=============================================
  * @Fn				- STRING_length
  * @brief 			- Calculate the length of string "str" without counting the terminating null
  * @param [in] 	- str: Pointer to string to find its length
  * @param [out] 	- None
  * @retval 		- Length of string "str" without counting the terminating null
  * Note			- None
  */
unsigned short STRING_length(const unsigned char* str){
	unsigned short length = 0;
	/* Validate that we are not accessing a null pointer */
	if(NULL != str){
		while('\0' != str[length]){
			length++;
		}
	}
	else{ /* Do Nothing */ }
	return length;
}

/**=============================================
  * @Fn				- STRING_word_firstOccurrence
  * @brief 			- Returns pointer to first occurrence of "word" in "str"
  * @param [in] 	- str : Pointer to the array containing the string to search for
  * @param [in] 	- word: Pointer to the string to find the first occurrence of it
  * @param [out] 	- None
  * @retval 		- Pointer to the location of the first occurrence of "word" in "str"
  * Note			- None
  */
unsigned char* STRING_word_firstOccurrence(const unsigned char* str, const unsigned char* word){
	unsigned char* ret_val = NULL;

	/* Validate that we are not accessing a null pointer */
	if( (NULL != str) && (NULL != word) ){
		unsigned short str_index = 0;
		unsigned short word_index;
		while('\0' != str[str_index]){
			/* Reset flag */
			unsigned int found_flag = 0;
			for(word_index = 0; ( ('\0' != word[word_index]) && ('\0' != str[str_index + word_index]) ) ; word_index++){
				/* If characters are not equal, set a flag */
				if(str[str_index + word_index] != word[word_index]){
					found_flag |= 1;
				}
				else{ /* Do Nothing */ }
			}
			if(0 == found_flag){
				/* If found, set return value and break while loop */
				ret_val = (unsigned char*)&(str[str_index]);
				break;
			}
			else{ /* Do Nothing */ }
			str_index++;
		}
	}
	else{ /* Do Nothing */ }
	return ret_val;
}

/**=============================================
  * @Fn				- STRING_word_lastOccurrence
  * @brief 			- Returns pointer to last occurrence of "word" in "str"
  * @param [in] 	- str : Pointer to the array containing the string to search for
  * @param [in] 	- word: Pointer to the string to find the last occurrence of it
  * @param [out] 	- None
  * @retval 		- Pointer to the location of the last occurrence of "word" in "str"
  * Note			- None
  */
unsigned char* STRING_word_lastOccurrence(const unsigned char* str, const unsigned char* word){
	unsigned char* ret_val = NULL;

	/* Validate that we are not accessing a null pointer */
	if( (NULL != str) && (NULL != word) ){
		unsigned short str_index = 0;
		unsigned short word_index;
		while('\0' != str[str_index]){
			/* Reset flag */
			unsigned int found_flag = 0;
			for(word_index = 0; ( ('\0' != word[word_index]) && ('\0' != str[str_index + word_index]) ) ; word_index++){
				/* If characters are not equal, set a flag */
				if(str[str_index + word_index] != word[word_index]){
					found_flag |= 1;
				}
				else{ /* Do Nothing */ }
			}
			if(0 == found_flag){
				/* If found, set return value and continue to check if there is one more occurence */
				ret_val = (unsigned char*)&(str[str_index]);
			}
			else{ /* Do Nothing */ }
			str_index++;
		}
	}
	else{ /* Do Nothing */ }
	return ret_val;
}
