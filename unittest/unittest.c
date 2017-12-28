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
#include <string.h>
#include <locale.h>

#include "../edflib.h"


#define  JUMP_TO_EXIT_ERROR_PROC   {line = __LINE__; goto OUT_ERROR;}


int dblcmp(double, double);
int dblcmp_lim(double, double, double);



int main(void)
{
  int i, j,
      tmp,
      hdl=-1,
      chns=2,
      ibuf[20000],
      line,
      ival1,
      ival2;

  char str[4096]={0,},
       pbuf[300];

  short sbuf[100];

  double dbuf[1000];

  struct edf_hdr_struct hdr;

  struct edf_annotation_struct annot;

  FILE *fp;

  setlocale(LC_ALL, "C");

  if(edflib_version() != 113)  JUMP_TO_EXIT_ERROR_PROC

/********************************** EDF writing ******************************/

  hdl = edfopen_file_writeonly("test.edf", EDFLIB_FILETYPE_EDFPLUS, 512);

  if(hdl < 0)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<512; i++)
  {
    if(edf_set_samplefrequency(hdl, i, 10239))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_maximum(hdl, i, -10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_minimum(hdl, i, -30000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_maximum(hdl, i, 10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_minimum(hdl, i, -10000))  JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<10239; i++)
  {
    dbuf[i] = 0;
  }

  if(edfwrite_physical_samples(hdl, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl))
  {
    hdl = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl = edfopen_file_writeonly("test.edf", EDFLIB_FILETYPE_EDFPLUS, 512);

  if(hdl < 0)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<512; i++)
  {
    if(edf_set_samplefrequency(hdl, i, 10240))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_maximum(hdl, i, -10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_minimum(hdl, i, -30000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_maximum(hdl, i, 10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_minimum(hdl, i, -10000))  JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<10240; i++)
  {
    dbuf[i] = 0;
  }

  if(edfwrite_physical_samples(hdl, dbuf) != EDFLIB_DATARECORD_SIZE_TOO_BIG)  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl) == 0)
  {
    hdl = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

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
  str[2] = 248;

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

  if(edfwrite_annotation_latin1(hdl, 9000, 1000, "Test 1"))  JUMP_TO_EXIT_ERROR_PROC

  if(edfwrite_annotation_latin1(hdl, 13000, -1, "Recording ends"))  JUMP_TO_EXIT_ERROR_PROC

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

  ival1 = -10100;

  ival2 = 9900;

  for(j=0; j<4; j++)
  {
    for(i=0; i<20; i++)
    {
      ibuf[i] = ival1;

      ival1 += 253;
    }

    if(edfwrite_digital_samples(hdl, ibuf))  JUMP_TO_EXIT_ERROR_PROC

    for(i=0; i<23; i++)
    {
      ibuf[i] = ival2;

      ival2 += 253;
    }

    if(edfwrite_digital_samples(hdl, ibuf))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfclose_file(hdl))
  {
    hdl = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

/********************************** BDF writing ******************************/

  hdl = edfopen_file_writeonly("test.bdf", EDFLIB_FILETYPE_BDFPLUS, 512);

  if(hdl < 0)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<512; i++)
  {
    if(edf_set_samplefrequency(hdl, i, 10239))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_maximum(hdl, i, -10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_minimum(hdl, i, -30000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_maximum(hdl, i, 10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_minimum(hdl, i, -10000))  JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<10239; i++)
  {
    dbuf[i] = 0;
  }

  if(edfwrite_physical_samples(hdl, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl))
  {
    hdl = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl = edfopen_file_writeonly("test.bdf", EDFLIB_FILETYPE_BDFPLUS, 512);

  if(hdl < 0)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<512; i++)
  {
    if(edf_set_samplefrequency(hdl, i, 10240))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_maximum(hdl, i, -10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_minimum(hdl, i, -30000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_maximum(hdl, i, 10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_minimum(hdl, i, -10000))  JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<10240; i++)
  {
    dbuf[i] = 0;
  }

  if(edfwrite_physical_samples(hdl, dbuf) != EDFLIB_DATARECORD_SIZE_TOO_BIG)  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl) == 0)
  {
    hdl = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

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
  str[2] = 248;

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

  if(edfwrite_annotation_latin1(hdl, 6000, 2000, "Test 2"))  JUMP_TO_EXIT_ERROR_PROC

  if(edfwrite_annotation_latin1(hdl, 11700, -1, "Recording ends"))  JUMP_TO_EXIT_ERROR_PROC

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

  ival1 = -1010000;

  ival2 = 990000;

  for(j=0; j<4; j++)
  {
    for(i=0; i<20; i++)
    {
      ibuf[i] = ival1;

      ival1 += 25300;
    }

    if(edfwrite_digital_samples(hdl, ibuf))  JUMP_TO_EXIT_ERROR_PROC

    for(i=0; i<23; i++)
    {
      ibuf[i] = ival2;

      ival2 += 25300;
    }

    if(edfwrite_digital_samples(hdl, ibuf))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfclose_file(hdl))
  {
    hdl = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl = -1;

/********************************** EDF reading ******************************/

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS))  JUMP_TO_EXIT_ERROR_PROC

  hdl = hdr.handle;

  if(hdr.filetype != 1)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.edfsignals != 2)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.file_duration != 13000000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.startdate_day != 5)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.startdate_month != 12)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.startdate_year != 2017)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.starttime_second != 8)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.starttime_minute != 23)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.starttime_hour != 12)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.starttime_subsecond != 0)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.patient_name, "John Doe"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.patientcode, "01234"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.gender, "Male"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.birthdate, "04 jul 2010"))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr.patient_additional, "nop", 3))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.admincode, "789"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.technician, "Richard Roe"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.equipment, "device"))  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.datarecord_duration != 1300000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.datarecords_in_file != 10)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.annotations_in_file != 3)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.signalparam[0].label, "trace1          "))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.signalparam[1].label, "trace2          "))  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[0].smp_in_file != 200)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[1].smp_in_file != 230)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[0].phys_max != 10000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[1].phys_max != -10000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[0].phys_min != -5000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[1].phys_min != -30000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[0].dig_max != 10000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[1].dig_max != 30000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[0].dig_min != -10000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[1].dig_min != 10000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[0].smp_in_datarecord != 20)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[1].smp_in_datarecord != 23)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.signalparam[0].physdimension, "uVxxxxxx"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.signalparam[1].physdimension, " C0xxxxx"))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr.signalparam[0].prefilter, "qwerty   ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr.signalparam[1].prefilter, "zxcvbn   ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr.signalparam[0].transducer, "asdfgh   ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr.signalparam[1].transducer, "poklhyg  ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_get_annotation(hdl, 0, &annot))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.onset != 0)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.duration, ""))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.annotation, "Recording starts"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_get_annotation(hdl, 1, &annot))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.onset != 9000000)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.duration, "0.1000"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.annotation, "Test 1"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_get_annotation(hdl, 2, &annot))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.onset != 13000000)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.duration, ""))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.annotation, "Recording ends"))  JUMP_TO_EXIT_ERROR_PROC

  if(edfseek(hdl, 1, 400, EDFSEEK_SET) == 400)  JUMP_TO_EXIT_ERROR_PROC

  if(edfseek(hdl, 0, 412, EDFSEEK_SET) == 412)  JUMP_TO_EXIT_ERROR_PROC

  if(edfseek(hdl, 0, 20, EDFSEEK_SET) != 20)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_physical_samples(hdl, 0, 20, dbuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
//    printf("%f   %i\n", dbuf[i], -5100 + (i * 800));

    if(i == 0)
    {
      if(dblcmp(dbuf[i], -5000))  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(i == 19)
    {
      if(dblcmp(dbuf[i], 10000))  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(dblcmp_lim(dbuf[i], -5100 + (i * 800), 0.75))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 1, 23, EDFSEEK_SET) != 23)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_physical_samples(hdl, 1, 23, dbuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
//    printf("%f   %i\n", dbuf[i], -30100 + (i * 909));

    if(i == 0)
    {
      if(dblcmp(dbuf[i], -30000))  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(dblcmp(dbuf[i], -30100 + (i * 909)))  JUMP_TO_EXIT_ERROR_PROC
  }

  edfrewind(hdl, 0);

  if(edfread_physical_samples(hdl, 0, 20, dbuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
//    printf("%f   %i\n", dbuf[i], -5100 + (i * 800));

    if(i == 0)
    {
      if(dblcmp(dbuf[i], -5000))  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(i == 19)
    {
      if(dblcmp(dbuf[i], 10000))  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(dblcmp_lim(dbuf[i], -5100 + (i * 800), 0.75))  JUMP_TO_EXIT_ERROR_PROC
  }

  edfrewind(hdl, 1);

  if(edfread_physical_samples(hdl, 1, 23, dbuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
//    printf("%f   %i\n", dbuf[i], -30100 + (i * 909));

    if(i == 0)
    {
      if(dblcmp(dbuf[i], -30000))  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(dblcmp(dbuf[i], -30100 + (i * 909)))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 0, 40, EDFSEEK_SET) != 40)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl, 0, 20, ibuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
//    printf("%i   %i\n", ibuf[i], -10100 + (i * 1053));

    if(i == 0)
    {
      if(ibuf[i] != -10000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != -10100 + (i * 1053))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 1, 46, EDFSEEK_SET) != 46)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl, 1, 23, ibuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
//    printf("%i:   %i   %i\n", i, ibuf[i], 9900 + (i * 1053));

    if(i == 0)
    {
      if(ibuf[i] != 10000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if((i == 20) || (i == 21))
    {
      if(ibuf[i] != 30000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(i == 22)
    {
      if(ibuf[i] != 10000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != 9900 + (i * 1053))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 0, 80, EDFSEEK_SET) != 80)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl, 0, 20, ibuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
//    printf("%i   %i\n", ibuf[i], -10100 + (i * 1053));

    if(i == 0)
    {
      if(ibuf[i] != -10000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != -10100 + (i * 1053))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 1, 92, EDFSEEK_SET) != 92)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl, 1, 23, ibuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
//    printf("%i:   %i   %i\n", i, ibuf[i], 9900 + (i * 1053));

    if(i == 0)
    {
      if(ibuf[i] != 10000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(i >= 20)
    {
      if(ibuf[i] != 30000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != 9900 + (i * 1053))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 0, 60, EDFSEEK_SET) != 60)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl, 0, 20, ibuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
//    printf("%i   %i\n", ibuf[i], -10100 + (i * 1053));

    if(i == 0)
    {
      if(ibuf[i] != -10000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != -10100 + (i * 1053))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 1, 69, EDFSEEK_SET) != 69)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl, 1, 23, ibuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
//    printf("%i:   %i   %i\n", i, ibuf[i], 9900 + (i * 1053));

    if(i == 0)
    {
      if(ibuf[i] != 10000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if((i == 20) || (i == 21))
    {
      if(ibuf[i] != 30000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(i == 22)
    {
      if(ibuf[i] != 10000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != 9900 + (i * 1053))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 0, 100, EDFSEEK_SET) != 100)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl, 0, 20, ibuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
//    printf("%i   %i\n", ibuf[i], -10100 + (i * 1053));

    if(i == 0)
    {
      if(ibuf[i] != -10000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != -10100 + (i * 1053))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 1, 115, EDFSEEK_SET) != 115)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl, 1, 23, ibuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
//    printf("%i:   %i   %i\n", i, ibuf[i], 9900 + (i * 1053));

    if(i == 0)
    {
      if(ibuf[i] != 10000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(i >= 20)
    {
      if(ibuf[i] != 30000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != 9900 + (i * 1053))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfread_digital_samples(hdl, 0, 80, ibuf) != 80)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<80; i++)
  {
//    printf("%i:   %i   %i\n", i,  ibuf[i], -10100 + (i * 253));

    if(i == 0)
    {
      if(ibuf[i] != -10000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != -10100 + (i * 253))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfread_digital_samples(hdl, 1, 92, ibuf) != 92)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<92; i++)
  {
//    printf("%i:   %i   %i\n", i,  ibuf[i], 9900 + (i * 253));

    if(i == 0)
    {
      if(ibuf[i] != 10000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(i >= 80)
    {
      if(ibuf[i] != 30000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != 9900 + (i * 253))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 0, 185, EDFSEEK_SET) != 185)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl, 0, 20, ibuf) != 15)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<15; i++)
  {
//    printf("%i   %i\n", ibuf[i], -10100 + ((i + 65) * 253));

    if(ibuf[i] != -10100 + ((i + 65) * 253))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfclose_file(hdl))
  {
    hdl = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl = -1;

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 1, SEEK_SET);

  fputc('1', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 1, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 16, SEEK_SET);

  fputc(' ', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 16, SEEK_SET);

  fputc('0', fp);

  fseek(fp, 0xaa, SEEK_SET);

  fputc(':', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xaa, SEEK_SET);

  fputc('.', fp);

  fseek(fp, 0xab, SEEK_SET);

  fputc('9', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xab, SEEK_SET);

  fputc('1', fp);

  fseek(fp, 0xac, SEEK_SET);

  fputc('q', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xac, SEEK_SET);

  fputc('2', fp);

  fseek(fp, 0xc4, SEEK_SET);

  fputc('D', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_IS_DISCONTINUOUS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xc4, SEEK_SET);

  fputc('C', fp);

  fseek(fp, 0x12e, SEEK_SET);

  fputc(' ', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x12e, SEEK_SET);

  fputc('s', fp);

  fseek(fp, 0x1ac, SEEK_SET);

  fputc(181, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x1ac, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0x308, SEEK_SET);

  fputc(' ', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x308, SEEK_SET);

  fputc('-', fp);

  fseek(fp, 0x30d, SEEK_SET);

  fputc(',', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x30d, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0x3a5, SEEK_SET);

  fputc('.', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x3a5, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0x3bc, SEEK_SET);

  fputc(207, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x3bc, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0x40b, SEEK_SET);

  fputc(247, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x40b, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0x560, SEEK_SET);

  fputc(127, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x560, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0x5ff, SEEK_SET);

  fputc(13, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x5ff, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0x54a, SEEK_SET);

  fputc('.', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x54a, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0xad, SEEK_SET);

  fputc('-', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xad, SEEK_SET);

  fputc('.', fp);

  fseek(fp, 0x803, SEEK_SET);

  fwrite("0.12", 4, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x803, SEEK_SET);

  fwrite("0.131", 5, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x803, SEEK_SET);

  fwrite("0.130", 5, 1, fp);

  fseek(fp, 0x802, SEEK_SET);

  fputc('0', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x802, SEEK_SET);

  fputc('-', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x802, SEEK_SET);

  fputc('+', fp);

  fseek(fp, 0x750, SEEK_SET);

  fputc(0, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x750, SEEK_SET);

  fputc(0x14, fp);

  fputc(1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x751, SEEK_SET);

  fputc(0, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS))  JUMP_TO_EXIT_ERROR_PROC

  hdl = hdr.handle;

  if(edfclose_file(hdl))
  {
    hdl = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl = -1;

  /****************************************/

  fp = fopen("test.edf", "rb");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x600, SEEK_SET);

  if(fread(str, 40, 1, fp) != 1)
  {
    fclose(fp);

    JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<20; i++)
  {
//    printf("buf[%i]: %i   value: %f\n", i, ((signed short *)str)[i], ((-5100 + (i * 800)) / 0.75) - 3333.333333);

    if(i == 0)
    {
      if(((signed short *)str)[i] != -10000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(i >= 19)
    {
      if(((signed short *)str)[i] != 10000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(dblcmp_lim(((signed short *)str)[i], ((-5100 + (i * 800)) / 0.75) - 3333.333333, 1.0001))
    {
      fclose(fp);

      JUMP_TO_EXIT_ERROR_PROC
    }
  }

  fclose(fp);

  /****************************************/

  fp = fopen("test.edf", "rb");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x628, SEEK_SET);

  if(fread(str, 46, 1, fp) != 1)
  {
    fclose(fp);

    JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<23; i++)
  {
//    printf("buf[%i]: %i   value: %i\n", i, ((signed short *)str)[i], (-30100 + (i * 909)) + 40000);

    if(i == 0)
    {
      if(((signed short *)str)[i] != 10000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(((signed short *)str)[i] != (-30100 + (i * 909)) + 40000)
    {
      fclose(fp);

      JUMP_TO_EXIT_ERROR_PROC
    }
  }

  fclose(fp);

  /****************************************/

  fp = fopen("test.edf", "rb");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x7ac, SEEK_SET);

  if(fread(str, 40, 1, fp) != 1)
  {
    fclose(fp);

    JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<20; i++)
  {
//    printf("buf[%i]: %i   value: %f\n", i, ((signed short *)str)[i], ((-5100 + (i * 800)) / 0.75) - 3333.333333);

    if(i == 0)
    {
      if(((signed short *)str)[i] != -10000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(i >= 19)
    {
      if(((signed short *)str)[i] != 10000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(dblcmp_lim(((signed short *)str)[i], ((-5100 + (i * 800)) / 0.75) - 3333.333333, 1.0001))
    {
      fclose(fp);

      JUMP_TO_EXIT_ERROR_PROC
    }
  }

  fclose(fp);

  /****************************************/

  fp = fopen("test.edf", "rb");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x7d4, SEEK_SET);

  if(fread(str, 46, 1, fp) != 1)
  {
    fclose(fp);

    JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<23; i++)
  {
//    printf("buf[%i]: %i   value: %i\n", i, ((signed short *)str)[i], (-30100 + (i * 909)) + 40000);

    if(i == 0)
    {
      if(((signed short *)str)[i] != 10000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(((signed short *)str)[i] != (-30100 + (i * 909)) + 40000)
    {
      fclose(fp);

      JUMP_TO_EXIT_ERROR_PROC
    }
  }

  fclose(fp);

  /****************************************/

  fp = fopen("test.edf", "rb");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x958, SEEK_SET);

  if(fread(str, 40, 1, fp) != 1)
  {
    fclose(fp);

    JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<20; i++)
  {
//    printf("buf[%i]: %i   value: %i\n", i, ((signed short *)str)[i], -10100 + (i * 1053));

    if(i == 0)
    {
      if(((signed short *)str)[i] != -10000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(((signed short *)str)[i] != -10100 + (i * 1053))
    {
      fclose(fp);

      JUMP_TO_EXIT_ERROR_PROC
    }
  }

  fclose(fp);

  /****************************************/

  fp = fopen("test.edf", "rb");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x980, SEEK_SET);

  if(fread(str, 46, 1, fp) != 1)
  {
    fclose(fp);

    JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<23; i++)
  {
//    printf("buf[%i]: %i   value: %i\n", i, ((signed short *)str)[i], 9900 + (i * 1053));

    if((i == 0) || (i == 22))
    {
      if(((signed short *)str)[i] != 10000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if((i == 20) || (i == 21))
    {
      if(((signed short *)str)[i] != 30000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(((signed short *)str)[i] != 9900 + (i * 1053))
    {
      fclose(fp);

      JUMP_TO_EXIT_ERROR_PROC
    }
  }

  fclose(fp);

  /****************************************/

  fp = fopen("test.edf", "rb");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xb04, SEEK_SET);

  if(fread(str, 40, 1, fp) != 1)
  {
    fclose(fp);

    JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<20; i++)
  {
//    printf("buf[%i]: %i   value: %i\n", i, ((signed short *)str)[i], -10100 + (i * 1053));

    if(i == 0)
    {
      if(((signed short *)str)[i] != -10000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(((signed short *)str)[i] != -10100 + (i * 1053))
    {
      fclose(fp);

      JUMP_TO_EXIT_ERROR_PROC
    }
  }

  fclose(fp);

  /****************************************/

  fp = fopen("test.edf", "rb");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xb2c, SEEK_SET);

  if(fread(str, 46, 1, fp) != 1)
  {
    fclose(fp);

    JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<23; i++)
  {
//    printf("buf[%i]: %i   value: %i\n", i, ((signed short *)str)[i], 9900 + (i * 1053));

    if((i == 0) || (i == 22))
    {
      if(((signed short *)str)[i] != 10000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if((i == 20) || (i == 21))
    {
      if(((signed short *)str)[i] != 30000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(((signed short *)str)[i] != 9900 + (i * 1053))
    {
      fclose(fp);

      JUMP_TO_EXIT_ERROR_PROC
    }
  }

  fclose(fp);

  /****************************************/

  fp = fopen("test.edf", "rb");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xcb0, SEEK_SET);

  if(fread(str, 40, 1, fp) != 1)
  {
    fclose(fp);

    JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<20; i++)
  {
//    printf("buf[%i]: %i   value: %i\n", i, ((signed short *)str)[i], -10100 + (i * 1053));

    if(i == 0)
    {
      if(((signed short *)str)[i] != -10000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(((signed short *)str)[i] != -10100 + (i * 1053))
    {
      fclose(fp);

      JUMP_TO_EXIT_ERROR_PROC
    }
  }

  fclose(fp);

  /****************************************/

  fp = fopen("test.edf", "rb");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xcd8, SEEK_SET);

  if(fread(str, 46, 1, fp) != 1)
  {
    fclose(fp);

    JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<23; i++)
  {
//    printf("buf[%i]: %i   value: %i\n", i, ((signed short *)str)[i], 9900 + (i * 1053));

    if(i == 0)
    {
      if(((signed short *)str)[i] != 10000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(i >= 20)
    {
      if(((signed short *)str)[i] != 30000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(((signed short *)str)[i] != 9900 + (i * 1053))
    {
      fclose(fp);

      JUMP_TO_EXIT_ERROR_PROC
    }
  }

  fclose(fp);

  /****************************************/

  fp = fopen("test.edf", "rb");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xe5c, SEEK_SET);

  if(fread(str, 40, 1, fp) != 1)
  {
    fclose(fp);

    JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<20; i++)
  {
//    printf("buf[%i]: %i   value: %i\n", i, ((signed short *)str)[i], -10100 + (i * 1053));

    if(i == 0)
    {
      if(((signed short *)str)[i] != -10000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(((signed short *)str)[i] != -10100 + (i * 1053))
    {
      fclose(fp);

      JUMP_TO_EXIT_ERROR_PROC
    }
  }

  fclose(fp);

  /****************************************/

  fp = fopen("test.edf", "rb");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xe84, SEEK_SET);

  if(fread(str, 46, 1, fp) != 1)
  {
    fclose(fp);

    JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<23; i++)
  {
//    printf("buf[%i]: %i   value: %i\n", i, ((signed short *)str)[i], 9900 + (i * 1053));

    if(i == 0)
    {
      if(((signed short *)str)[i] != 10000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(i >= 20)
    {
      if(((signed short *)str)[i] != 30000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(((signed short *)str)[i] != 9900 + (i * 1053))
    {
      fclose(fp);

      JUMP_TO_EXIT_ERROR_PROC
    }
  }

  fclose(fp);

/********************************** BDF reading ******************************/

  if(edfopen_file_readonly("test.bdf", &hdr, EDFLIB_READ_ALL_ANNOTATIONS))  JUMP_TO_EXIT_ERROR_PROC

  hdl = hdr.handle;

  if(hdr.filetype != 3)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.edfsignals != 2)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.file_duration != 11700000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.startdate_day != 5)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.startdate_month != 12)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.startdate_year != 2017)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.starttime_second != 8)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.starttime_minute != 23)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.starttime_hour != 12)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.starttime_subsecond != 0)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.patient_name, "John Doe"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.patientcode, "01234"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.gender, "Male"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.birthdate, "04 jul 2010"))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr.patient_additional, "nop", 3))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.admincode, "789"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.technician, "Richard Roe"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.equipment, "device"))  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.datarecord_duration != 1300000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.datarecords_in_file != 9)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.annotations_in_file != 3)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.signalparam[0].label, "trace1          "))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.signalparam[1].label, "trace2          "))  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[0].smp_in_file != 180)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[1].smp_in_file != 207)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[0].phys_max != 10000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[1].phys_max != -10000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[0].phys_min != -5000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[1].phys_min != -30000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[0].dig_max != 1000000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[1].dig_max != 3000000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[0].dig_min != -1000000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[1].dig_min != 1000000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[0].smp_in_datarecord != 20)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr.signalparam[1].smp_in_datarecord != 23)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.signalparam[0].physdimension, "uVxxxxxx"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr.signalparam[1].physdimension, " C0xxxxx"))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr.signalparam[0].prefilter, "qwerty   ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr.signalparam[1].prefilter, "zxcvbn   ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr.signalparam[0].transducer, "asdfgh   ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr.signalparam[1].transducer, "poklhyg  ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_get_annotation(hdl, 0, &annot))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.onset != 0)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.duration, ""))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.annotation, "Recording starts"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_get_annotation(hdl, 1, &annot))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.onset != 6000000)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.duration, "0.2000"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.annotation, "Test 2"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_get_annotation(hdl, 2, &annot))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.onset != 11700000)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.duration, ""))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.annotation, "Recording ends"))  JUMP_TO_EXIT_ERROR_PROC

  if(edfseek(hdl, 1, 500, EDFSEEK_SET) == 500)  JUMP_TO_EXIT_ERROR_PROC

  if(edfseek(hdl, 0, 333, EDFSEEK_SET) == 333)  JUMP_TO_EXIT_ERROR_PROC

  if(edfseek(hdl, 0, 20, EDFSEEK_SET) != 20)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_physical_samples(hdl, 0, 20, dbuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
//    printf("%i:   %f   %i\n", i, dbuf[i], -5100 + (i * 800));

    if(i == 0)
    {
      if(dblcmp_lim(dbuf[i], -5000, 0.00001))  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(i == 19)
    {
      if(dblcmp_lim(dbuf[i], 10000, 0.00001))  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(dblcmp_lim(dbuf[i], -5100 + (i * 800), 0.0075))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 1, 23, EDFSEEK_SET) != 23)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_physical_samples(hdl, 1, 23, dbuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
//    printf("%i:   %f   %i\n", i, dbuf[i], -30100 + (i * 909));

    if(i == 0)
    {
      if(dblcmp_lim(dbuf[i], -30000, 0.00001))  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(dblcmp(dbuf[i], -30100 + (i * 909)))  JUMP_TO_EXIT_ERROR_PROC
  }

  edfrewind(hdl, 0);

  if(edfread_physical_samples(hdl, 0, 20, dbuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
//    printf("%i:   %f   %i\n", i, dbuf[i], -5100 + (i * 800));

    if(i == 0)
    {
      if(dblcmp_lim(dbuf[i], -5000, 0.00001))  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(i == 19)
    {
      if(dblcmp_lim(dbuf[i], 10000, 0.00001))  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(dblcmp_lim(dbuf[i], -5100 + (i * 800), 0.0075))  JUMP_TO_EXIT_ERROR_PROC
  }

  edfrewind(hdl, 1);

  if(edfread_physical_samples(hdl, 1, 23, dbuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
//    printf("%i:   %f   %i\n", i, dbuf[i], -30100 + (i * 909));

    if(i == 0)
    {
      if(dblcmp_lim(dbuf[i], -30000, 0.00001))  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(dblcmp(dbuf[i], -30100 + (i * 909)))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 0, 40, EDFSEEK_SET) != 40)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl, 0, 20, ibuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
//    printf("%i:   %i   %i\n", i, ibuf[i], -1010000 + (i * 105300));

    if(ibuf[i] != -1010000 + (i * 105300))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 1, 46, EDFSEEK_SET) != 46)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl, 1, 23, ibuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
//    printf("%i:   %i   %i\n", i, ibuf[i], 990000 + (i * 105300));

    if(ibuf[i] != 990000 + (i * 105300))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 0, 60, EDFSEEK_SET) != 60)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl, 0, 20, ibuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
//    printf("%i:   %i   %i\n", i, ibuf[i], -1010000 + (i * 105300));

    if(i == 0)
    {
      if(ibuf[i] != -1000000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != -1010000 + (i * 105300))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 1, 69, EDFSEEK_SET) != 69)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl, 1, 23, ibuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
//    printf("%i:   %i   %i\n", i, ibuf[i], 990000 + (i * 105300));

    if(i == 0)
    {
      if(ibuf[i] != 1000000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(i >= 20)
    {
      if(ibuf[i] != 3000000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != 990000 + (i * 105300))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfread_digital_samples(hdl, 0, 20, ibuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
//    printf("%i:   %i   %i\n", i, ibuf[i], -1010000 + (i * 105300));

    if(i == 0)
    {
      if(ibuf[i] != -1000000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != -1010000 + (i * 105300))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfread_digital_samples(hdl, 1, 23, ibuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
//    printf("%i:   %i   %i\n", i, ibuf[i], 990000 + (i * 105300));

    if(i == 0)
    {
      if(ibuf[i] != 1000000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(i >= 20)
    {
      if(ibuf[i] != 3000000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != 990000 + (i * 105300))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfread_digital_samples(hdl, 0, 80, ibuf) != 80)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<80; i++)
  {
//    printf("%i:   %i   %i\n", i,  ibuf[i], -1010000 + (i * 25300));

    if(i == 0)
    {
      if(ibuf[i] != -1000000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != -1010000 + (i * 25300))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfread_digital_samples(hdl, 1, 92, ibuf) != 92)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<92; i++)
  {
//    printf("%i:   %i   %i\n", i,  ibuf[i], 990000 + (i * 25300));

    if(i == 0)
    {
      if(ibuf[i] != 1000000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(i >= 80)
    {
      if(ibuf[i] != 3000000)  JUMP_TO_EXIT_ERROR_PROC

      continue;
    }

    if(ibuf[i] != 990000 + (i * 25300))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfseek(hdl, 0, 165, EDFSEEK_SET) != 165)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl, 0, 20, ibuf) != 15)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<15; i++)
  {
//    printf("%i   %i\n", ibuf[i], -10100 + ((i + 65) * 253));

    if(ibuf[i] != -1010000 + ((i + 65) * 25300))  JUMP_TO_EXIT_ERROR_PROC
  }

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


int dblcmp(double val1, double val2)
{
  long double diff = (long double)val1 - (long double)val2;

  if(diff > 1e-13)
  {
    return 1;
  }
  else if(-diff > 1e-13)
    {
      return -1;
    }
    else
    {
      return 0;
    }
}


int dblcmp_lim(double val1, double val2, double lim)
{
  long double diff = (long double)val1 - (long double)val2;

  if(diff > lim)
  {
    return 1;
  }
  else if(-diff > lim)
    {
      return -1;
    }
    else
    {
      return 0;
    }
}

















