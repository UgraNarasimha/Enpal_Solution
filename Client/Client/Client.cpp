// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "Example1_h.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

int main()
{
    RPC_STATUS status;
    RPC_CSTR szStringBinding = NULL;

    // Creates a string binding handle.
    // This function is nothing more than a printf.
    // Connection is not done here.
    status = RpcStringBindingComposeA(
        NULL,                                        // UUID to bind to.
        (RPC_CSTR)("ncacn_ip_tcp"),// Use TCP/IP protocol.
        (RPC_CSTR)("localhost"),   // TCP/IP network address to use.
        (RPC_CSTR)("4747"),        // TCP/IP port to use.
        NULL,                                    // Protocol dependent network options to use.
        &szStringBinding);                       // String binding output.

    if (status)
        exit(status);

    // Validates the format of the string binding handle and converts
    // it to a binding handle.
    // Connection is not done here either.
    status = RpcBindingFromStringBindingA(
        szStringBinding,        // The string binding to validate.
        &hExample1Binding);     // Put the result in the implicit binding
                                // handle defined in the IDL file.

    if (status)
        exit(status);

    RpcTryExcept
    {
        // Calls the RPC function. The hExample1Binding binding handle
        // is used implicitly.
        // Connection is done here.
        long int iSecret;
        char buffer[10];
        /* initialize random seed: */
        srand(time(NULL));

        while (1)
        {
            /* generate secret number between 1 and 10000: */
            iSecret = rand() % 1000 + 1;

            sprintf_s(buffer, "%i", iSecret);
            Output((const unsigned char*)buffer);
            Sleep(1000);
        }
        
    }
        RpcExcept(1)
    {
        std::cerr << "Runtime reported exception " << RpcExceptionCode()
            << std::endl;
    }
    RpcEndExcept

        // Free the memory allocated by a string.
        status = RpcStringFreeA(
            &szStringBinding); // String to be freed.

    if (status)
        exit(status);

    // Releases binding handle resources and disconnects from the server.
    status = RpcBindingFree(
        &hExample1Binding); // Frees the implicit binding handle defined in the IDL file.

    if (status)
        exit(status);
}

// Memory allocation function for RPC.
// The runtime uses these two functions for allocating/deallocating
// enough memory to pass the string to the server.
void* __RPC_USER midl_user_allocate(size_t size)
{
    return malloc(size);
}

// Memory deallocation function for RPC.
void __RPC_USER midl_user_free(void* p)
{
    free(p);
}