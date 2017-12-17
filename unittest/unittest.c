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


int main(void)
{
  int i,
      hdl,
      chns=2,
      ibuf[100],
      ival1,
      ival2;

  char str[4096]={0,};

  short sbuf[100],
        sval1,
        sval2;

  double dbuf[100],
         dval1,
         dval2;

  setlocale(LC_ALL, "C");

  if(edflib_version() != 113)  goto OUT_ERROR;

  hdl = edfopen_file_writeonly("test.edf", EDFLIB_FILETYPE_EDFPLUS, chns);

  if(hdl < 0)  goto OUT_ERROR;

  if(edf_set_samplefrequency(hdl, 0, 20))  goto OUT_ERROR;

  if(edf_set_samplefrequency(hdl, 1, 23))  goto OUT_ERROR;

  if(edf_set_physical_maximum(hdl, 0, 10000))  goto OUT_ERROR;

  if(edf_set_physical_minimum(hdl, 0, -5000))  goto OUT_ERROR;

  if(edf_set_physical_maximum(hdl, 1, -10000))  goto OUT_ERROR;

  if(edf_set_physical_minimum(hdl, 1, -30000))  goto OUT_ERROR;

  if(edf_set_digital_maximum(hdl, 0, 10000))  goto OUT_ERROR;

  if(edf_set_digital_minimum(hdl, 0, -10000))  goto OUT_ERROR;

  if(edf_set_digital_maximum(hdl, 1, 30000))  goto OUT_ERROR;

  if(edf_set_digital_minimum(hdl, 1, 10000))  goto OUT_ERROR;

  if(edf_set_label(hdl, 0, "trace1"))  goto OUT_ERROR;

  if(edf_set_label(hdl, 1, "trace2"))  goto OUT_ERROR;

  if(edf_set_prefilter(hdl, 0, "qwerty"))  goto OUT_ERROR;

  if(edf_set_prefilter(hdl, 1, "zxcvbn"))  goto OUT_ERROR;

  if(edf_set_transducer(hdl, 0, "asdfgh"))  goto OUT_ERROR;

  if(edf_set_transducer(hdl, 1, "poklhyg"))  goto OUT_ERROR;

  strcpy(str, "uVxxxxxxxxxxxxxxxxxxxx");

  str[0] = 181;

  if(edf_set_physical_dimension(hdl, 0, str))  goto OUT_ERROR;

  strcpy(str, "dCxxxxxxxxxxxxxxxxxxxx");

  str[0] = 176;

  if(edf_set_physical_dimension(hdl, 1, str))  goto OUT_ERROR;

  if(edf_set_startdatetime(hdl, 2017, 12, 5, 12, 23, 8))  goto OUT_ERROR;

  if(edf_set_patientname(hdl, "John Doe"))  goto OUT_ERROR;

  if(edf_set_patientcode(hdl, "01234"))  goto OUT_ERROR;

  if(edf_set_gender(hdl, 1))  goto OUT_ERROR;

  if(edf_set_birthdate(hdl, 2010, 7, 4))  goto OUT_ERROR;

  if(edf_set_patient_additional(hdl, "nop"))  goto OUT_ERROR;

  if(edf_set_admincode(hdl, "789"))  goto OUT_ERROR;

  if(edf_set_technician(hdl, "Richard Roe"))  goto OUT_ERROR;

  if(edf_set_equipment(hdl, "device"))  goto OUT_ERROR;

  if(edf_set_number_of_annotation_signals(hdl, 3))  goto OUT_ERROR;

  if(edf_set_datarecord_duration(hdl, 13000))  goto OUT_ERROR;

  if(edfwrite_annotation_latin1(hdl, 0, -1, "Recording Starts"))  goto OUT_ERROR;

  for(i=0; i<20; i++)
  {
    dbuf[i] = -5100 + (i * 800);
  }

  if(edfwrite_physical_samples(hdl, dbuf))  goto OUT_ERROR;

  for(i=0; i<23; i++)
  {
    dbuf[i] = -30100 + (i * 909);
  }

  if(edfwrite_physical_samples(hdl, dbuf))  goto OUT_ERROR;

  for(i=0; i<20; i++)
  {
    dbuf[i] = -5100 + (i * 800);
  }

  for(i=0; i<23; i++)
  {
    dbuf[i + 20] = -30100 + (i * 909);
  }

  if(edf_blockwrite_physical_samples(hdl, dbuf))  goto OUT_ERROR;

  for(i=0; i<20; i++)
  {
    sbuf[i] = -10100 + (i * 1053);
  }

  if(edfwrite_digital_short_samples(hdl, sbuf))  goto OUT_ERROR;

  for(i=0; i<23; i++)
  {
    sbuf[i] = 9900 + (i * 1053);
  }

  if(edfwrite_digital_short_samples(hdl, sbuf))  goto OUT_ERROR;

  for(i=0; i<20; i++)
  {
    sbuf[i] = -10100 + (i * 1053);
  }

  for(i=0; i<23; i++)
  {
    sbuf[i + 20] = 9900 + (i * 1053);
  }

  if(edf_blockwrite_digital_short_samples(hdl, sbuf))  goto OUT_ERROR;

  for(i=0; i<20; i++)
  {
    ibuf[i] = -10100 + (i * 1053);
  }

  if(edfwrite_digital_samples(hdl, ibuf))  goto OUT_ERROR;

  for(i=0; i<23; i++)
  {
    ibuf[i] = 9900 + (i * 1053);
  }

  if(edfwrite_digital_samples(hdl, ibuf))  goto OUT_ERROR;

  for(i=0; i<20; i++)
  {
    ibuf[i] = -10100 + (i * 1053);
  }

  for(i=0; i<23; i++)
  {
    ibuf[i + 20] = 9900 + (i * 1053);
  }

  if(edf_blockwrite_digital_samples(hdl, ibuf))  goto OUT_ERROR;






  if(edfclose_file(hdl))
  {
    hdl = -1;

    goto OUT_ERROR;
  }

  return EXIT_SUCCESS;

OUT_ERROR:

  if(hdl >= 0)
  {
    edfclose_file(hdl);
  }

  return EXIT_FAILURE;
}
















