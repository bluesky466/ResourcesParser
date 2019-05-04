/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//

#ifndef _LIBS_UTILS_BYTE_ORDER_H
#define _LIBS_UTILS_BYTE_ORDER_H

// 默认使用小端序
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1
#endif

#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif

static inline uint16_t android_swap_short(uint16_t v)
{
    return (v<<8) | (v>>8);
}

#define DEVICE_BYTE_ORDER LITTLE_ENDIAN

#if BYTE_ORDER == DEVICE_BYTE_ORDER

#define	dtohs(x)	(x)

#else

#define	dtohs(x)	(android_swap_short(x))

#endif

#endif // _LIBS_UTILS_BYTE_ORDER_H
