//
//  testsuite.h
//  Diskco
//
//  Created by Richard Bronkhorst on 23/03/15.
//  Copyright (c) 2015 Richard Bronkhorst. All rights reserved.
//

#ifndef Diskco_testsuite_h
#define Diskco_testsuite_h
#include "options.h"
#include "buffer.h"
Options *parse_options(int argc, const char* args[]);
void prepare_buffer(Buffer* buffer);
#endif
