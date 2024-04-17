#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
// Enable large files support
#ifndef LARGEFILES
#	define	LARGEFILES
#endif
#ifndef _LARGEFILE_SOURCE
#	define _LARGEFILE_SOURCE
#endif
#ifndef _LARGEFILE64_SOURCE
#	define _LARGEFILE64_SOURCE
#endif
#ifndef __USE_LARGEFILE64
#	define __USE_LARGEFILE64
#endif
#ifndef __USE_FILE_OFFSET64
#	define __USE_FILE_OFFSET64
#endif

#define _FILE_OFFSET_BITS 64

#include <string.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "dtconfig.h"
#include "dtsearch.h"
#include "dtsviewr.h"
#include "dinputstream.h"
#include "dtsfc.h"
#include "xmlmaker.h"
#include "settings.h"
