/* semihosting.h
 *
 *  Created on: 17.04.2016
 *      Author: JS
 */
#ifndef SEMIHOSTING_H
#define SEMIHOSTING_H

#include "main.h"

#ifdef USE_DEBUG
    #define dbg_log(msg, ...)     printf(msg,##__VA_ARGS__)
#else
    #define dbg_log(msg, ...)
#endif

#endif /* SEMIHOSTING_H */
