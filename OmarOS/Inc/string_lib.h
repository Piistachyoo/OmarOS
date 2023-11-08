/*************************************************************************/
/* Author        : Omar Yamany                                    		 */
/* Project       : String_Library                                    	 */
/* File          : string_lib.h                              	         */
/* Date          : Jun 22, 2023                                          */
/* Version       : V1                                                    */
/* GitHub        : https://github.com/Piistachyoo             		     */
/*************************************************************************/
#ifndef STRING_LIB_H_
#define STRING_LIB_H_

#ifndef NULL
#define NULL	0
#endif

/*
 * =============================================
 * APIs Supported by "Driver Name"
 * =============================================
 */

/**=============================================
  * @Fn				- STRING_convert_upperCase
  * @brief 			- Converts all alphabetical characters in string "str" to upper case
  * @param [in] 	- str: Pointer to string to be converted to upper case
  * @param [out] 	- None
  * @retval 		- None
  * Note			- None
  */
void STRING_convert_upperCase(unsigned char* str);

/**=============================================
  * @Fn				- STRING_convert_lowerCase
  * @brief 			- Converts all alphabetical characters in string "str" to lowercase
  * @param [in] 	- str: Pointer to string to be converted to lowercase
  * @param [out] 	- None
  * @retval 		- None
  * Note			- None
  */
void STRING_convert_lowerCase(unsigned char* str);

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
void STRING_set_memoryLocation(unsigned char* str, unsigned char value, unsigned short size);

/**=============================================
  * @Fn				- STRING_concatenate
  * @brief 			- Concatenates "src_str" at the end of "des_str"
  * @param [in] 	- src_str: Pointer to the array containing the source string
  * @param [in] 	- des_str: Pointer to the array containing the destination string
  * @param [out] 	- None
  * @retval 		- None
  * Note			- None
  */
void STRING_concatenate(const unsigned char* src_str, unsigned char* des_str);

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
void STRING_concatenate_length(const unsigned char* src_str, unsigned char* des_str, const unsigned short length);

/**=============================================
  * @Fn				- STRING_char_firstOccurrence
  * @brief 			- Returns pointer to first occurrence of "value" in "str"
  * @param [in] 	- str	 : Pointer to the array containing a string
  * @param [in] 	- value  : Value to search for in "str"
  * @param [out] 	- None
  * @retval 		- Pointer to the location of the first occurrence of "value" in "str"
  * Note			- Returns NULL if character "value" was not found in string "str"
  */
unsigned char* STRING_char_firstOccurrence(const unsigned char* str, const unsigned char value);

/**=============================================
  * @Fn				- STRING_char_lastOccurrence
  * @brief 			- Returns pointer to last occurrence of "value" in "str"
  * @param [in] 	- str	 : Pointer to the array containing a string
  * @param [in] 	- value  : Value to search for in "str"
  * @param [out] 	- None
  * @retval 		- Pointer to the location of the last occurrence of "value" in "str"
  * Note			- None
  */
unsigned char* STRING_char_lastOccurrence(const unsigned char* str, const unsigned char value);

/**=============================================
  * @Fn				- STRING_compare_caseSensetive
  * @brief 			- Checks if two strings are identical (case sensitive)
  * @param [in] 	- str1: Pointer to the first string
  * @param [in] 	- str2: Pointer to the second string
  * @param [out] 	- None
  * @retval 		- Returns 0 if str1 is same as str2. Returns -1 if str1 < str2. Returns 1 if str1 > str2
  * Note			- None
  */
signed char STRING_compare_caseSensitive(const unsigned char* str1, const unsigned char* str2);

/**=============================================
  * @Fn				- STRING_compare_caseInsensetive
  * @brief 			- Checks if two strings are identical (case insensitive)
  * @param [in] 	- str1: Pointer to the first string
  * @param [in] 	- str2: Pointer to the second string
  * @param [out] 	- None
  * @retval 		- Returns 0 if str1 is same as str2. Returns -1 if str1 < str2. Returns 1 if str1 > str2
  * Note			- None
  */
signed char STRING_compare_caseInsensitive(const unsigned char* str1, const unsigned char* str2);

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
signed char STRING_compare_caseSensitive_length(const unsigned char* str1, const unsigned char* str2, const unsigned short length);

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
signed char STRING_compare_caseInsensitive_length(const unsigned char* str1, const unsigned char* str2, const unsigned short length);

/**=============================================
  * @Fn				- STRING_copy
  * @brief 			- Copies a string from "src_str" into "des_str"
  * @param [in] 	- src_str: Pointer to the source array that contains a string to be copied
  * @param [in] 	- des_str: Pointer to the destination array
  * @param [out] 	- None
  * @retval 		- Length of the copied string
  * Note			- None
  */
unsigned short STRING_copy(const unsigned char* src_str, unsigned char* des_str);

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
void STRING_copy_length(const unsigned char* src_str, unsigned char* des_str, const unsigned short length);

/**=============================================
  * @Fn				- STRING_length
  * @brief 			- Calculate the length of string "str" without counting the terminating null
  * @param [in] 	- str: Pointer to string to find its length
  * @param [out] 	- None
  * @retval 		- Length of string "str" without counting the terminating null
  * Note			- None
  */
unsigned short STRING_length(const unsigned char* str);

/**=============================================
  * @Fn				- STRING_word_firstOccurrence
  * @brief 			- Returns pointer to first occurrence of "word" in "str"
  * @param [in] 	- str : Pointer to the array containing the string to search for
  * @param [in] 	- word: Pointer to the string to find the first occurrence of it
  * @param [out] 	- None
  * @retval 		- Pointer to the location of the first occurrence of "word" in "str"
  * Note			- None
  */
unsigned char* STRING_word_firstOccurrence(const unsigned char* str, const unsigned char* word);

/**=============================================
  * @Fn				- STRING_word_lastOccurrence
  * @brief 			- Returns pointer to last occurrence of "word" in "str"
  * @param [in] 	- str : Pointer to the array containing the string to search for
  * @param [in] 	- word: Pointer to the string to find the last occurrence of it
  * @param [out] 	- None
  * @retval 		- Pointer to the location of the last occurrence of "word" in "str"
  * Note			- None
  */
unsigned char* STRING_word_lastOccurrence(const unsigned char* str, const unsigned char* word);

#endif /* STRING_LIB_H_ */
