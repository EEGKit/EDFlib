/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2017 Teunis van Beelen
*
* Email: teuniz@gmail.com
*
***************************************************************************
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************
*/


#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "../edflib.h"


#define  JUMP_TO_EXIT_ERROR_PROC   {line = __LINE__; goto OUT_ERROR;}



int main(void)
{
  int i,
      tmp,
      hdl,
      chns=2,
      ibuf[100],
      ival1,
      ival2,
      line;

  char str[4096]={0,},
       pbuf[300];

  short sbuf[100],
        sval1,
        sval2;

  double dbuf[100],
         dval1,
         dval2;

  setlocale(LC_ALL, "C");

  if(edflib_version() != 113)  JUMP_TO_EXIT_ERROR_PROC

/********************************** EDF **************************************/

  hdl = edfopen_file_writeonly("test.edf", EDFLIB_FILETYPE_EDFPLUS, chns);

  if(hdl < 0)  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_samplefrequency(hdl, 0, 20))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_samplefrequency(hdl, 1, 23))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_maximum(hdl, 0, 10000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_minimum(hdl, 0, -5000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_maximum(hdl, 1, -10000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_minimum(hdl, 1, -30000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_maximum(hdl, 0, 10000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_minimum(hdl, 0, -10000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_maximum(hdl, 1, 30000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_minimum(hdl, 1, 10000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_label(hdl, 0, "trace1"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_label(hdl, 1, "trace2"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_prefilter(hdl, 0, "qwerty"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_prefilter(hdl, 1, "zxcvbn"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_transducer(hdl, 0, "asdfgh"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_transducer(hdl, 1, "poklhyg"))  JUMP_TO_EXIT_ERROR_PROC

  strcpy(str, "uVxxxxxxxxxxxxxxxxxxxx");

  str[0] = 181;

  if(edf_set_physical_dimension(hdl, 0, str))  JUMP_TO_EXIT_ERROR_PROC

  strcpy(str, "dCxxxxxxxxxxxxxxxxxxxx");

  str[0] = 176;

  if(edf_set_physical_dimension(hdl, 1, str))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_startdatetime(hdl, 2017, 12, 5, 12, 23, 8))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patientname(hdl, "John Doe"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patientcode(hdl, "01234"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_gender(hdl, 1))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_birthdate(hdl, 2010, 7, 4))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patient_additional(hdl, "nop"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_admincode(hdl, "789"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_technician(hdl, "Richard Roe"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_equipment(hdl, "device"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_number_of_annotation_signals(hdl, 3))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_datarecord_duration(hdl, 13000))  JUMP_TO_EXIT_ERROR_PROC

  if(edfwrite_annotation_latin1(hdl, 0, -1, "Recording starts"))  JUMP_TO_EXIT_ERROR_PROC

  if(edfwrite_annotation_latin1(hdl, 6500, -1, "Recording ends"))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    dbuf[i] = -5100 + (i * 800);
  }

  if(edfwrite_physical_samples(hdl, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
    dbuf[i] = -30100 + (i * 909);
  }

  if(edfwrite_physical_samples(hdl, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    dbuf[i] = -5100 + (i * 800);
  }

  for(i=0; i<23; i++)
  {
    dbuf[i + 20] = -30100 + (i * 909);
  }

  if(edf_blockwrite_physical_samples(hdl, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    sbuf[i] = -10100 + (i * 1053);
  }

  if(edfwrite_digital_short_samples(hdl, sbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
    sbuf[i] = 9900 + (i * 1053);
  }

  if(edfwrite_digital_short_samples(hdl, sbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    sbuf[i] = -10100 + (i * 1053);
  }

  for(i=0; i<23; i++)
  {
    sbuf[i + 20] = 9900 + (i * 1053);
  }

  if(edf_blockwrite_digital_short_samples(hdl, sbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    ibuf[i] = -10100 + (i * 1053);
  }

  if(edfwrite_digital_samples(hdl, ibuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
    ibuf[i] = 9900 + (i * 1053);
  }

  if(edfwrite_digital_samples(hdl, ibuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    ibuf[i] = -10100 + (i * 1053);
  }

  for(i=0; i<23; i++)
  {
    ibuf[i + 20] = 9900 + (i * 1053);
  }

  if(edf_blockwrite_digital_samples(hdl, ibuf))  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl))
  {
    hdl = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

/********************************** BDF **************************************/

  hdl = edfopen_file_writeonly("test.bdf", EDFLIB_FILETYPE_BDFPLUS, chns);

  if(hdl < 0)  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_samplefrequency(hdl, 0, 20))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_samplefrequency(hdl, 1, 23))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_maximum(hdl, 0, 10000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_minimum(hdl, 0, -5000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_maximum(hdl, 1, -10000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_minimum(hdl, 1, -30000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_maximum(hdl, 0, 1000000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_minimum(hdl, 0, -1000000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_maximum(hdl, 1, 3000000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_minimum(hdl, 1, 1000000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_label(hdl, 0, "trace1"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_label(hdl, 1, "trace2"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_prefilter(hdl, 0, "qwerty"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_prefilter(hdl, 1, "zxcvbn"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_transducer(hdl, 0, "asdfgh"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_transducer(hdl, 1, "poklhyg"))  JUMP_TO_EXIT_ERROR_PROC

  strcpy(str, "uVxxxxxxxxxxxxxxxxxxxx");

  str[0] = 181;

  if(edf_set_physical_dimension(hdl, 0, str))  JUMP_TO_EXIT_ERROR_PROC

  strcpy(str, "dCxxxxxxxxxxxxxxxxxxxx");

  str[0] = 176;

  if(edf_set_physical_dimension(hdl, 1, str))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_startdatetime(hdl, 2017, 12, 5, 12, 23, 8))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patientname(hdl, "John Doe"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patientcode(hdl, "01234"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_gender(hdl, 1))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_birthdate(hdl, 2010, 7, 4))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patient_additional(hdl, "nop"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_admincode(hdl, "789"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_technician(hdl, "Richard Roe"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_equipment(hdl, "device"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_number_of_annotation_signals(hdl, 3))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_datarecord_duration(hdl, 13000))  JUMP_TO_EXIT_ERROR_PROC

  if(edfwrite_annotation_latin1(hdl, 0, -1, "Recording starts"))  JUMP_TO_EXIT_ERROR_PROC

  if(edfwrite_annotation_latin1(hdl, 6500, -1, "Recording ends"))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    dbuf[i] = -5100 + (i * 800);
  }

  if(edfwrite_physical_samples(hdl, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
    dbuf[i] = -30100 + (i * 909);
  }

  if(edfwrite_physical_samples(hdl, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    dbuf[i] = -5100 + (i * 800);
  }

  for(i=0; i<23; i++)
  {
    dbuf[i + 20] = -30100 + (i * 909);
  }

  if(edf_blockwrite_physical_samples(hdl, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    tmp = -1010000 + (i * 105300);

    pbuf[i * 3] = tmp & 0xff;

    pbuf[i * 3 + 1] = (tmp >> 8) & 0xff;

    pbuf[i * 3 + 2] = (tmp >> 16) & 0xff;
  }

  for(i=0; i<23; i++)
  {
    tmp = 990000 + (i * 105300);

    pbuf[i * 3 + 60] = tmp & 0xff;

    pbuf[i * 3 + 61] = (tmp >> 8) & 0xff;

    pbuf[i * 3 + 62] = (tmp >> 16) & 0xff;
  }

  if(edf_blockwrite_digital_3byte_samples(hdl, pbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    ibuf[i] = -1010000 + (i * 105300);
  }

  if(edfwrite_digital_samples(hdl, ibuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
    ibuf[i] = 990000 + (i * 105300);
  }

  if(edfwrite_digital_samples(hdl, ibuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    ibuf[i] = -1010000 + (i * 105300);
  }

  for(i=0; i<23; i++)
  {
    ibuf[i + 20] = 990000 + (i * 105300);
  }

  if(edf_blockwrite_digital_samples(hdl, ibuf))  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl))
  {
    hdl = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  return EXIT_SUCCESS;

OUT_ERROR:

  if(hdl >= 0)
  {
    edfclose_file(hdl);
  }

  printf("Error, line %i file %s\n", line, __FILE__);

  return EXIT_FAILURE;
}
















