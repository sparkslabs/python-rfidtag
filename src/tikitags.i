/*
 *  pytiki
 *  Copyright (C) 2008 -
 *     Younghun Kim
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* tikitag.i*/
%module pytiki
%{
extern char INIT1[15];
extern char INIT2[15];
extern char INIT3[21];
extern char INIT4[15];
extern char status1[10];
extern char power_up[10];
extern char power_down[10];
extern char abort_pipe[10];
extern char poll[20];
extern char get1[15];

extern int tiki_init(void);
extern int tiki_config(int time, int rate);
extern int tiki_close(void);
extern int tiki_seek(void);
extern unsigned long long tiki_getID1();
extern unsigned long long tiki_getID2();
extern int tiki_getReaderID();
%}
extern char INIT1[15];
extern char INIT2[15];
extern char INIT3[21];
extern char INIT4[15];
extern char status1[10];
extern char power_up[10];
extern char power_down[10];
extern char abort_pipe[10];
extern char poll[20];
extern char get1[15];


extern int tiki_init(void);
extern int tiki_config(int time, int rate);
extern int tiki_close(void);
extern int tiki_seek(void);
extern unsigned long long tiki_getID1();
extern unsigned long long tiki_getID2();
extern int tiki_getReaderID();

