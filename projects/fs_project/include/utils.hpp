#if cplusplus < 201104L
#define noexcept throw()
#endif // #if cplusplus < 201103L 

#ifndef __TEST_H__
#define __TEST_H__


#include <stdio.h> /*printf*/

typedef enum {PASSED, FAILED} test_status_t;

#define YELLOW "\033[01;33m"
#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define MAGENTA "\033[1;35m"

#define REQUIRE(condition) do{\
								if (0 == (condition)) \
							 	{ \
						 			printf("\tcheck of " #condition YELLOW " FAILED\n" );\
								  	printf(MAGENTA "\tFile - %s\n\tLine - %d\n" RESET, __FILE__, __LINE__);\
								  	return FAILED; \
								}\
						 	 }while(0)


#define RUNTEST(test) do\
					 	{\
							 if (PASSED == test())\
							 {\
							 	printf("%s" GREEN " PASSED\n" RESET, #test);\
							 }\
							 else\
							 {\
								 printf(#test RED " FAILED\n" RESET);\
							 }\
						} while(0)

#endif /* __TEST_H__*/

