#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

void VERIFY_k4a_result_t(k4a_result_t result)
{
    if(result != K4A_RESULT_SUCCEEDED)
    {
        printf("k4a_result_t != K4A_RESULT_SUCCEEDED\n");
        exit(-1);
    }
}

void VERIFY_k4a_wait_result_t(k4a_wait_result_t result)
{
    if (result == K4A_WAIT_RESULT_FAILED)
    {
        printf("K4A_WAIT_RESULT_FAILED\n");
        exit(-1);
    }
    else if (result == K4A_WAIT_RESULT_TIMEOUT)
    {
        printf("K4A_WAIT_RESULT_TIMEOUT\n");
    }
}
