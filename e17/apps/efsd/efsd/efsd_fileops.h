/*

Copyright (C) 2000, 2001 Christian Kreibich <kreibich@aciri.org>.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies of the Software and its documentation and acknowledgment shall be
given in the documentation and software packages that this Software was
used.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
#ifndef efsd_fileops_h
#define efsd_fileops_h

#include <efsd.h>

int  send_reply(EfsdCommand *cmd, EfsdStatus status, int errorcode, 
		int data_len, void *data, int client);

int  efsd_remove(EfsdCommand *cmd, int client);
int  efsd_move(EfsdCommand *cmd, int client);
int  efsd_copy(EfsdCommand *cmd, int client);
int  efsd_symlink(EfsdCommand *cmd, int client);
int  efsd_listdir(EfsdCommand *cmd, int client);
int  efsd_makedir(EfsdCommand *cmd, int client);
int  efsd_chmod(EfsdCommand *cmd, int client);
int  efsd_set_metadata(EfsdCommand *cmd, int client);
int  efsd_get_metadata(EfsdCommand *cmd, int client);
int  efsd_start_monitor(EfsdCommand *cmd, int client);
int  efsd_stop_monitor(EfsdCommand *cmd, int client);
int  efsd_stat(EfsdCommand *cmd, int client);

#endif
