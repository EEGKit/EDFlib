/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2017- 2021 Teunis van Beelen
*
* Email: teuniz@protonmail.com
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
      hdl1=-1,
      hdl2=-1,
      chns=2,
      *ibuf=NULL,
      line,
      ival1,
      ival2;

  char *str=NULL,
       *pbuf=NULL;

  short *sbuf=NULL;

  long long l_tmp;

  double *dbuf=NULL;

  union {
          unsigned int one;
          signed int one_signed;
          unsigned short two[2];
          signed short two_signed[2];
          unsigned char four[4];
        } var;

  struct edf_hdr_struct hdr1, hdr2;

  struct edf_annotation_struct annot;

  FILE *fp=NULL;

  setlocale(LC_ALL, "C");

  if(edflib_version() != 122)  JUMP_TO_EXIT_ERROR_PROC

  ibuf = (int *)malloc(100 * sizeof(int));
  if(ibuf == NULL)
  {
    JUMP_TO_EXIT_ERROR_PROC;
  }

  sbuf = (short *)malloc(100 * sizeof(short));
  if(sbuf == NULL)
  {
    JUMP_TO_EXIT_ERROR_PROC;
  }

  dbuf = (double *)malloc(10240 * sizeof(double));
  if(dbuf == NULL)
  {
    JUMP_TO_EXIT_ERROR_PROC;
  }

  str = (char *)malloc(4096 * sizeof(char));
  if(str == NULL)
  {
    JUMP_TO_EXIT_ERROR_PROC;
  }
  str[0] = 0;

  pbuf = (char *)malloc(300 * sizeof(char));
  if(pbuf == NULL)
  {
    JUMP_TO_EXIT_ERROR_PROC;
  }

/********************************** EDF writing ******************************/

  hdl1 = edfopen_file_writeonly_with_params("test.edf", EDFLIB_FILETYPE_EDFPLUS, 65, 633, 3000, "uV");

  if(hdl1 < 0)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<633; i++)
  {
    dbuf[i] = i;
  }

  if(!edfopen_file_readonly("test.edf", &hdr2, EDFLIB_READ_ALL_ANNOTATIONS))  JUMP_TO_EXIT_ERROR_PROC

  for(j=0; j<65; j++)
  {
    if(edfwrite_physical_samples(hdl1, dbuf))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfopen_file_readonly("test.edf", &hdr2, EDFLIB_READ_ALL_ANNOTATIONS))  JUMP_TO_EXIT_ERROR_PROC

  hdl2 = hdr2.handle;

  if(hdr2.datarecords_in_file != 1)  JUMP_TO_EXIT_ERROR_PROC

  for(j=0; j<65; j++)
  {
    if(hdr2.signalparam[j].smp_in_file != 633)  JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<9; i++)
  {
    for(j=0; j<65; j++)
    {
      if(edfwrite_physical_samples(hdl1, dbuf))  JUMP_TO_EXIT_ERROR_PROC
    }
  }

  if(hdr2.datarecords_in_file != 1)  JUMP_TO_EXIT_ERROR_PROC

  for(j=0; j<65; j++)
  {
    if(hdr2.signalparam[j].smp_in_file != 633)  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edf_update_header(hdl2, &hdr2))  JUMP_TO_EXIT_ERROR_PROC

  if(hdr2.datarecords_in_file != 10)  JUMP_TO_EXIT_ERROR_PROC

  for(j=0; j<65; j++)
  {
    if(hdr2.signalparam[j].smp_in_file != 6330)  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = -1;

  if(edf_update_header(hdl2, &hdr2))  JUMP_TO_EXIT_ERROR_PROC

  if(!edf_update_header(hdl2, &hdr2))  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl2))
  {
    hdl2 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl2 = -1;

/********************************** EDF reading ******************************/

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS))  JUMP_TO_EXIT_ERROR_PROC

  hdl1 = hdr1.handle;

  if(hdr1.filetype != 1)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.edfsignals != 65)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.file_duration != 100000000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.datarecord_duration != 10000000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.datarecords_in_file != 10)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].smp_in_file != 6330)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].phys_max != 3000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].phys_min != -3000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].dig_max != 32767)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].dig_min != -32768)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].smp_in_datarecord != 633)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.signalparam[0].physdimension, "uV      "))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.patientcode, ""))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.patient_name, "X"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.admincode, ""))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.technician, ""))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.equipment, ""))  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = -1;

/********************************** EDF writing ******************************/

  hdl1 = edfopen_file_writeonly_with_params("test.edf", EDFLIB_FILETYPE_EDFPLUS, 65, 633, 3000, "uV");

  if(hdl1 < 0)  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patientname(hdl1, "XY_Z"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patientcode(hdl1, "X2_3"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_admincode(hdl1, "X6_7"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_technician(hdl1, "X.Fo_o"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_equipment(hdl1, "Xe_q"))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<633; i++)
  {
    dbuf[i] = i;
  }

  for(i=0; i<10; i++)
  {
    for(j=0; j<65; j++)
    {
      if(edfwrite_physical_samples(hdl1, dbuf))  JUMP_TO_EXIT_ERROR_PROC
    }
  }

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

/********************************** EDF reading ******************************/

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS))  JUMP_TO_EXIT_ERROR_PROC

  hdl1 = hdr1.handle;

  if(hdr1.filetype != 1)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.edfsignals != 65)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.file_duration != 100000000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.datarecord_duration != 10000000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.datarecords_in_file != 10)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].smp_in_file != 6330)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].phys_max != 3000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].phys_min != -3000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].dig_max != 32767)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].dig_min != -32768)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].smp_in_datarecord != 633)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.signalparam[0].physdimension, "uV      "))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.patientcode, "X2 3"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.patient_name, "XY Z"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.admincode, "X6 7"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.technician, "X.Fo o"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.equipment, "Xe q"))  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = -1;

/********************************** EDF writing ******************************/

  hdl1 = edfopen_file_writeonly("test.edf", EDFLIB_FILETYPE_EDFPLUS, 512);

  if(hdl1 < 0)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<512; i++)
  {
    if(edf_set_samplefrequency(hdl1, i, 10239))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_maximum(hdl1, i, -10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_minimum(hdl1, i, -30000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_maximum(hdl1, i, 10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_minimum(hdl1, i, -10000))  JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<10239; i++)
  {
    dbuf[i] = 0;
  }

  if(edfwrite_physical_samples(hdl1, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = edfopen_file_writeonly("test.edf", EDFLIB_FILETYPE_EDFPLUS, 512);

  if(hdl1 < 0)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<512; i++)
  {
    if(edf_set_samplefrequency(hdl1, i, 10240))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_maximum(hdl1, i, -10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_minimum(hdl1, i, -30000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_maximum(hdl1, i, 10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_minimum(hdl1, i, -10000))  JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<10240; i++)
  {
    dbuf[i] = 0;
  }

  if(edfwrite_physical_samples(hdl1, dbuf) != EDFLIB_DATARECORD_SIZE_TOO_BIG)  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl1) == 0)
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = edfopen_file_writeonly("test.edf", EDFLIB_FILETYPE_EDFPLUS, chns);

  if(hdl1 < 0)  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_samplefrequency(hdl1, 0, 20))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_samplefrequency(hdl1, 1, 23))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_maximum(hdl1, 0, 10000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_minimum(hdl1, 0, -5000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_maximum(hdl1, 1, -10000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_minimum(hdl1, 1, -30000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_maximum(hdl1, 0, 10000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_minimum(hdl1, 0, -10000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_maximum(hdl1, 1, 30000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_minimum(hdl1, 1, 10000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_label(hdl1, 0, "trace1"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_label(hdl1, 1, "trace2"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_prefilter(hdl1, 0, "qwerty"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_prefilter(hdl1, 1, "zxcvbn"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_transducer(hdl1, 0, "asdfgh"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_transducer(hdl1, 1, "poklhyg"))  JUMP_TO_EXIT_ERROR_PROC

  strcpy(str, "uVxxxxxxxxxxxxxxxxxxxx");

  str[0] = 181;

  if(edf_set_physical_dimension(hdl1, 0, str))  JUMP_TO_EXIT_ERROR_PROC

  strcpy(str, "dCxxxxxxxxxxxxxxxxxxxx");

  str[0] = 176;
  str[2] = 248;

  if(edf_set_physical_dimension(hdl1, 1, str))  JUMP_TO_EXIT_ERROR_PROC

  if(!edf_set_startdatetime(hdl1, 2085, 12, 5, 12, 23, 8))  JUMP_TO_EXIT_ERROR_PROC

  if(!edf_set_startdatetime(hdl1, 1984, 12, 5, 12, 23, 8))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_startdatetime(hdl1, 2017, 12, 5, 12, 23, 8))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patientname(hdl1, "John Doü"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patientcode(hdl1, "01234"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_gender(hdl1, 1))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_birthdate(hdl1, 2010, 7, 4))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patient_additional(hdl1, "nop"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_admincode(hdl1, "789"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_technician(hdl1, "Rìchard Roë"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_equipment(hdl1, "device"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_number_of_annotation_signals(hdl1, 3))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_datarecord_duration(hdl1, 13000))  JUMP_TO_EXIT_ERROR_PROC

  if(edfwrite_annotation_latin1(hdl1, 0, -1, "Recording starts"))  JUMP_TO_EXIT_ERROR_PROC

  if(edfwrite_annotation_latin1(hdl1, 9000, 1000, "Test 1"))  JUMP_TO_EXIT_ERROR_PROC

  if(edfwrite_annotation_latin1(hdl1, 13000, -1, "Recording ends"))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    dbuf[i] = -5100 + (i * 800);
  }

  if(edfwrite_physical_samples(hdl1, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
    dbuf[i] = -30100 + (i * 909);
  }

  if(edfwrite_physical_samples(hdl1, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    dbuf[i] = -5100 + (i * 800);
  }

  for(i=0; i<23; i++)
  {
    dbuf[i + 20] = -30100 + (i * 909);
  }

  if(edf_blockwrite_physical_samples(hdl1, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    sbuf[i] = -10100 + (i * 1053);
  }

  if(edfwrite_digital_short_samples(hdl1, sbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
    sbuf[i] = 9900 + (i * 1053);
  }

  if(edfwrite_digital_short_samples(hdl1, sbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    sbuf[i] = -10100 + (i * 1053);
  }

  for(i=0; i<23; i++)
  {
    sbuf[i + 20] = 9900 + (i * 1053);
  }

  if(edf_blockwrite_digital_short_samples(hdl1, sbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    ibuf[i] = -10100 + (i * 1053);
  }

  if(edfwrite_digital_samples(hdl1, ibuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
    ibuf[i] = 9900 + (i * 1053);
  }

  if(edfwrite_digital_samples(hdl1, ibuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    ibuf[i] = -10100 + (i * 1053);
  }

  for(i=0; i<23; i++)
  {
    ibuf[i + 20] = 9900 + (i * 1053);
  }

  if(edf_blockwrite_digital_samples(hdl1, ibuf))  JUMP_TO_EXIT_ERROR_PROC

  ival1 = -10100;

  ival2 = 9900;

  for(j=0; j<4; j++)
  {
    for(i=0; i<20; i++)
    {
      ibuf[i] = ival1;

      ival1 += 253;
    }

    if(edfwrite_digital_samples(hdl1, ibuf))  JUMP_TO_EXIT_ERROR_PROC

    for(i=0; i<23; i++)
    {
      ibuf[i] = ival2;

      ival2 += 253;
    }

    if(edfwrite_digital_samples(hdl1, ibuf))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

/********************************** BDF writing ******************************/

  hdl1 = edfopen_file_writeonly("test.bdf", EDFLIB_FILETYPE_BDFPLUS, 512);

  if(hdl1 < 0)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<512; i++)
  {
    if(edf_set_samplefrequency(hdl1, i, 10239))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_maximum(hdl1, i, -10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_minimum(hdl1, i, -30000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_maximum(hdl1, i, 10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_minimum(hdl1, i, -10000))  JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<10239; i++)
  {
    dbuf[i] = 0;
  }

  if(edfwrite_physical_samples(hdl1, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = edfopen_file_writeonly("test.bdf", EDFLIB_FILETYPE_BDFPLUS, 512);

  if(hdl1 < 0)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<512; i++)
  {
    if(edf_set_samplefrequency(hdl1, i, 10240))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_maximum(hdl1, i, -10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_physical_minimum(hdl1, i, -30000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_maximum(hdl1, i, 10000))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_set_digital_minimum(hdl1, i, -10000))  JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<10240; i++)
  {
    dbuf[i] = 0;
  }

  if(edfwrite_physical_samples(hdl1, dbuf) != EDFLIB_DATARECORD_SIZE_TOO_BIG)  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl1) == 0)
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = edfopen_file_writeonly("test.bdf", EDFLIB_FILETYPE_BDFPLUS, chns);

  if(hdl1 < 0)  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_samplefrequency(hdl1, 0, 20))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_samplefrequency(hdl1, 1, 23))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_maximum(hdl1, 0, 10000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_minimum(hdl1, 0, -5000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_maximum(hdl1, 1, -10000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_minimum(hdl1, 1, -30000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_maximum(hdl1, 0, 1000000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_minimum(hdl1, 0, -1000000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_maximum(hdl1, 1, 3000000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_minimum(hdl1, 1, 1000000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_label(hdl1, 0, "trace1"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_label(hdl1, 1, "trace2"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_prefilter(hdl1, 0, "qwerty"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_prefilter(hdl1, 1, "zxcvbn"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_transducer(hdl1, 0, "asdfgh"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_transducer(hdl1, 1, "poklhyg"))  JUMP_TO_EXIT_ERROR_PROC

  strcpy(str, "uVxxxxxxxxxxxxxxxxxxxx");

  str[0] = 181;

  if(edf_set_physical_dimension(hdl1, 0, str))  JUMP_TO_EXIT_ERROR_PROC

  strcpy(str, "dCxxxxxxxxxxxxxxxxxxxx");

  str[0] = 176;
  str[2] = 248;

  if(edf_set_physical_dimension(hdl1, 1, str))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_startdatetime(hdl1, 2017, 12, 5, 12, 23, 8))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patientname(hdl1, "John Doe"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patientcode(hdl1, "01234"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_gender(hdl1, 1))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_birthdate(hdl1, 2010, 7, 4))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patient_additional(hdl1, "nop"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_admincode(hdl1, "789"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_technician(hdl1, "Richard Roe"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_equipment(hdl1, "device"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_number_of_annotation_signals(hdl1, 3))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_datarecord_duration(hdl1, 13000))  JUMP_TO_EXIT_ERROR_PROC

  if(edfwrite_annotation_latin1(hdl1, 0, -1, "Recording starts"))  JUMP_TO_EXIT_ERROR_PROC

  if(edfwrite_annotation_latin1(hdl1, 6000, 2000, "Test 2"))  JUMP_TO_EXIT_ERROR_PROC

  if(edfwrite_annotation_latin1(hdl1, 11700, -1, "Recording ends"))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    dbuf[i] = -5100 + (i * 800);
  }

  if(edfwrite_physical_samples(hdl1, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
    dbuf[i] = -30100 + (i * 909);
  }

  if(edfwrite_physical_samples(hdl1, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    dbuf[i] = -5100 + (i * 800);
  }

  for(i=0; i<23; i++)
  {
    dbuf[i + 20] = -30100 + (i * 909);
  }

  if(edf_blockwrite_physical_samples(hdl1, dbuf))  JUMP_TO_EXIT_ERROR_PROC

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

  if(edf_blockwrite_digital_3byte_samples(hdl1, pbuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    ibuf[i] = -1010000 + (i * 105300);
  }

  if(edfwrite_digital_samples(hdl1, ibuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
    ibuf[i] = 990000 + (i * 105300);
  }

  if(edfwrite_digital_samples(hdl1, ibuf))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
    ibuf[i] = -1010000 + (i * 105300);
  }

  for(i=0; i<23; i++)
  {
    ibuf[i + 20] = 990000 + (i * 105300);
  }

  if(edf_blockwrite_digital_samples(hdl1, ibuf))  JUMP_TO_EXIT_ERROR_PROC

  ival1 = -1010000;

  ival2 = 990000;

  for(j=0; j<4; j++)
  {
    for(i=0; i<20; i++)
    {
      ibuf[i] = ival1;

      ival1 += 25300;
    }

    if(edfwrite_digital_samples(hdl1, ibuf))  JUMP_TO_EXIT_ERROR_PROC

    for(i=0; i<23; i++)
    {
      ibuf[i] = ival2;

      ival2 += 25300;
    }

    if(edfwrite_digital_samples(hdl1, ibuf))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = -1;

/********************************** EDF reading ******************************/

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS))  JUMP_TO_EXIT_ERROR_PROC

  hdl1 = hdr1.handle;

  if(hdr1.filetype != 1)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.edfsignals != 2)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.file_duration != 13000000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.startdate_day != 5)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.startdate_month != 12)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.startdate_year != 2017)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.starttime_second != 8)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.starttime_minute != 23)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.starttime_hour != 12)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.starttime_subsecond != 0)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.patient_name, "John Dou"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.patientcode, "01234"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.gender, "Male"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.birthdate, "04 jul 2010"))  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.birthdate_day != 4)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.birthdate_month != 7)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.birthdate_year != 2010)  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.patient_additional, "nop", 3))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.admincode, "789"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.technician, "Richard Roe"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.equipment, "device"))  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.datarecord_duration != 1300000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.datarecords_in_file != 10)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.annotations_in_file != 3)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.signalparam[0].label, "trace1          "))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.signalparam[1].label, "trace2          "))  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].smp_in_file != 200)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[1].smp_in_file != 230)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].phys_max != 10000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[1].phys_max != -10000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].phys_min != -5000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[1].phys_min != -30000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].dig_max != 10000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[1].dig_max != 30000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].dig_min != -10000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[1].dig_min != 10000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].smp_in_datarecord != 20)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[1].smp_in_datarecord != 23)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.signalparam[0].physdimension, "uVxxxxxx"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.signalparam[1].physdimension, ".C0xxxxx"))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.signalparam[0].prefilter, "qwerty   ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.signalparam[1].prefilter, "zxcvbn   ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.signalparam[0].transducer, "asdfgh   ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.signalparam[1].transducer, "poklhyg  ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_get_annotation(hdl1, 0, &annot))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.onset != 0)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.duration, ""))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.duration_l != -10000000LL)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.annotation, "Recording starts"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_get_annotation(hdl1, 1, &annot))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.onset != 9000000)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.duration, "0.1000"))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.duration_l != 1000000LL)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.annotation, "Test 1"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_get_annotation(hdl1, 2, &annot))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.onset != 13000000)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.duration, ""))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.duration_l != -10000000LL)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.annotation, "Recording ends"))  JUMP_TO_EXIT_ERROR_PROC

  if(edfseek(hdl1, 1, 400, EDFSEEK_SET) == 400)  JUMP_TO_EXIT_ERROR_PROC

  if(edfseek(hdl1, 0, 412, EDFSEEK_SET) == 412)  JUMP_TO_EXIT_ERROR_PROC

  if(edfseek(hdl1, 0, 20, EDFSEEK_SET) != 20)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_physical_samples(hdl1, 0, 20, dbuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfseek(hdl1, 1, 23, EDFSEEK_SET) != 23)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_physical_samples(hdl1, 1, 23, dbuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

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

  edfrewind(hdl1, 0);

  if(edfread_physical_samples(hdl1, 0, 20, dbuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

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

  edfrewind(hdl1, 1);

  if(edfread_physical_samples(hdl1, 1, 23, dbuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfseek(hdl1, 0, 40, EDFSEEK_SET) != 40)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl1, 0, 20, ibuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfseek(hdl1, 1, 46, EDFSEEK_SET) != 46)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl1, 1, 23, ibuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfseek(hdl1, 0, 80, EDFSEEK_SET) != 80)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl1, 0, 20, ibuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfseek(hdl1, 1, 92, EDFSEEK_SET) != 92)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl1, 1, 23, ibuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfseek(hdl1, 0, 60, EDFSEEK_SET) != 60)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl1, 0, 20, ibuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfseek(hdl1, 1, 69, EDFSEEK_SET) != 69)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl1, 1, 23, ibuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfseek(hdl1, 0, 100, EDFSEEK_SET) != 100)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl1, 0, 20, ibuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfseek(hdl1, 1, 115, EDFSEEK_SET) != 115)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl1, 1, 23, ibuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfread_digital_samples(hdl1, 0, 80, ibuf) != 80)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfread_digital_samples(hdl1, 1, 92, ibuf) != 92)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfseek(hdl1, 0, 185, EDFSEEK_SET) != 185)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl1, 0, 20, ibuf) != 15)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<15; i++)
  {
//    printf("%i   %i\n", ibuf[i], -10100 + ((i + 65) * 253));

    if(ibuf[i] != -10100 + ((i + 65) * 253))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = -1;

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 1, SEEK_SET);

  fputc('1', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 1, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 16, SEEK_SET);

  fputc(' ', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 16, SEEK_SET);

  fputc('0', fp);

  fseek(fp, 0xaa, SEEK_SET);

  fputc(':', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xaa, SEEK_SET);

  fputc('.', fp);

  fseek(fp, 0xab, SEEK_SET);

  fputc('9', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xab, SEEK_SET);

  fputc('1', fp);

  fseek(fp, 0xac, SEEK_SET);

  fputc('q', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xac, SEEK_SET);

  fputc('2', fp);

  fseek(fp, 0xc4, SEEK_SET);

  fputc('D', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_IS_DISCONTINUOUS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xc4, SEEK_SET);

  fputc('C', fp);

  fseek(fp, 0x12e, SEEK_SET);

  fputc(' ', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x12e, SEEK_SET);

  fputc('s', fp);

  fseek(fp, 0x1ac, SEEK_SET);

  fputc(181, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x1ac, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0x308, SEEK_SET);

  fputc(' ', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x308, SEEK_SET);

  fputc('-', fp);

  fseek(fp, 0x30d, SEEK_SET);

  fputc(',', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x30d, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0x3a5, SEEK_SET);

  fputc('.', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x3a5, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0x3bc, SEEK_SET);

  fputc(207, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x3bc, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0x40b, SEEK_SET);

  fputc(247, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x40b, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0x560, SEEK_SET);

  fputc(127, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x560, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0x5ff, SEEK_SET);

  fputc(13, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x5ff, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0x54a, SEEK_SET);

  fputc('.', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x54a, SEEK_SET);

  fputc(' ', fp);

  fseek(fp, 0xad, SEEK_SET);

  fputc('-', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xad, SEEK_SET);

  fputc('.', fp);

  fseek(fp, 0x803, SEEK_SET);

  fwrite("0.12", 4, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x803, SEEK_SET);

  fwrite("0.131", 5, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x803, SEEK_SET);

  fwrite("0.130", 5, 1, fp);

  fseek(fp, 0x802, SEEK_SET);

  fputc('0', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x802, SEEK_SET);

  fputc('-', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x802, SEEK_SET);

  fputc('+', fp);

  fseek(fp, 0x750, SEEK_SET);

  fputc(0, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x750, SEEK_SET);

  fputc(0x14, fp);

  fputc(1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x751, SEEK_SET);

  fputc(0, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS))  JUMP_TO_EXIT_ERROR_PROC

  hdl1 = hdr1.handle;

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = -1;

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x358, SEEK_SET);

  fwrite("-32769  ", 8, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x358, SEEK_SET);

  fwrite("-10000  ", 8, 1, fp);

  fseek(fp, 0x380, SEEK_SET);

  fwrite("32768   ", 8, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x380, SEEK_SET);

  fwrite("10000   ", 8, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) != 0)  JUMP_TO_EXIT_ERROR_PROC

  hdl1 = hdr1.handle;

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = -1;

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

  if(edfopen_file_readonly("test.bdf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS))  JUMP_TO_EXIT_ERROR_PROC

  hdl1 = hdr1.handle;

  if(hdr1.filetype != 3)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.edfsignals != 2)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.file_duration != 11700000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.startdate_day != 5)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.startdate_month != 12)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.startdate_year != 2017)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.starttime_second != 8)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.starttime_minute != 23)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.starttime_hour != 12)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.starttime_subsecond != 0)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.patient_name, "John Doe"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.patientcode, "01234"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.gender, "Male"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.birthdate, "04 jul 2010"))  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.birthdate_day != 4)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.birthdate_month != 7)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.birthdate_year != 2010)  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.patient_additional, "nop", 3))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.admincode, "789"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.technician, "Richard Roe"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.equipment, "device"))  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.datarecord_duration != 1300000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.datarecords_in_file != 9)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.annotations_in_file != 3)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.signalparam[0].label, "trace1          "))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.signalparam[1].label, "trace2          "))  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].smp_in_file != 180)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[1].smp_in_file != 207)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].phys_max != 10000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[1].phys_max != -10000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].phys_min != -5000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[1].phys_min != -30000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].dig_max != 1000000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[1].dig_max != 3000000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].dig_min != -1000000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[1].dig_min != 1000000)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[0].smp_in_datarecord != 20)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.signalparam[1].smp_in_datarecord != 23)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.signalparam[0].physdimension, "uVxxxxxx"))  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(hdr1.signalparam[1].physdimension, ".C0xxxxx"))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.signalparam[0].prefilter, "qwerty   ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.signalparam[1].prefilter, "zxcvbn   ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.signalparam[0].transducer, "asdfgh   ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.signalparam[1].transducer, "poklhyg  ", 9))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_get_annotation(hdl1, 0, &annot))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.onset != 0)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.duration, ""))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.duration_l != -10000000LL)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.annotation, "Recording starts"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_get_annotation(hdl1, 1, &annot))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.onset != 6000000)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.duration, "0.2000"))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.duration_l != 2000000LL)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.annotation, "Test 2"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_get_annotation(hdl1, 2, &annot))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.onset != 11700000)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.duration, ""))  JUMP_TO_EXIT_ERROR_PROC

  if(annot.duration_l != -10000000LL)  JUMP_TO_EXIT_ERROR_PROC

  if(strcmp(annot.annotation, "Recording ends"))  JUMP_TO_EXIT_ERROR_PROC

  if(edfseek(hdl1, 1, 500, EDFSEEK_SET) == 500)  JUMP_TO_EXIT_ERROR_PROC

  if(edfseek(hdl1, 0, 333, EDFSEEK_SET) == 333)  JUMP_TO_EXIT_ERROR_PROC

  if(edfseek(hdl1, 0, 20, EDFSEEK_SET) != 20)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_physical_samples(hdl1, 0, 20, dbuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfseek(hdl1, 1, 23, EDFSEEK_SET) != 23)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_physical_samples(hdl1, 1, 23, dbuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

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

  edfrewind(hdl1, 0);

  if(edfread_physical_samples(hdl1, 0, 20, dbuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

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

  edfrewind(hdl1, 1);

  if(edfread_physical_samples(hdl1, 1, 23, dbuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfseek(hdl1, 0, 40, EDFSEEK_SET) != 40)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl1, 0, 20, ibuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<20; i++)
  {
//    printf("%i:   %i   %i\n", i, ibuf[i], -1010000 + (i * 105300));

    if(i == 0)
    {
      if(ibuf[i] != -1000000)  JUMP_TO_EXIT_ERROR_PROC
    }
    else
    {
      if(ibuf[i] != -1010000 + (i * 105300))  JUMP_TO_EXIT_ERROR_PROC
    }
  }

  if(edfseek(hdl1, 1, 46, EDFSEEK_SET) != 46)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl1, 1, 23, ibuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<23; i++)
  {
//    printf("%i:   %i   %i\n", i, ibuf[i], 990000 + (i * 105300));

    if(i == 0)
    {
      if(ibuf[i] != 1000000)  JUMP_TO_EXIT_ERROR_PROC
    }
    else if(i >= 20)
      {
        if(ibuf[i] != 3000000)  JUMP_TO_EXIT_ERROR_PROC
      }
      else
      {
        if(ibuf[i] != 990000 + (i * 105300))  JUMP_TO_EXIT_ERROR_PROC
      }
  }

  if(edfseek(hdl1, 0, 60, EDFSEEK_SET) != 60)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl1, 0, 20, ibuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfseek(hdl1, 1, 69, EDFSEEK_SET) != 69)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl1, 1, 23, ibuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfread_digital_samples(hdl1, 0, 20, ibuf) != 20)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfread_digital_samples(hdl1, 1, 23, ibuf) != 23)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfread_digital_samples(hdl1, 0, 80, ibuf) != 80)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfread_digital_samples(hdl1, 1, 92, ibuf) != 92)  JUMP_TO_EXIT_ERROR_PROC

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

  if(edfseek(hdl1, 0, 165, EDFSEEK_SET) != 165)  JUMP_TO_EXIT_ERROR_PROC

  if(edfread_digital_samples(hdl1, 0, 20, ibuf) != 15)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<15; i++)
  {
//    printf("%i   %i\n", ibuf[i], -10100 + ((i + 65) * 253));

    if(ibuf[i] != -1010000 + ((i + 65) * 25300))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  /****************************************/

  fp = fopen("test.bdf", "rb");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x600, SEEK_SET);

  if(fread(str, 60, 1, fp) != 1)
  {
    fclose(fp);

    JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<20; i++)
  {
    var.four[0] = str[i * 3];
    var.four[1] = str[i * 3 + 1];
    var.four[2] = str[i * 3 + 2];

    if(var.four[2]&0x80)
    {
      var.four[3] = 0xff;
    }
    else
    {
      var.four[3] = 0x00;
    }

//    printf("buf[%i]: %i   value: %f\n", i, var.one_signed, ((-5100 + (i * 800)) / 0.0075) - 333333.333333);

    if(i == 0)
    {
      if(var.one_signed != -1000000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(i >= 19)
    {
      if(var.one_signed != 1000000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(dblcmp_lim(var.one_signed, ((-5100 + (i * 800)) / 0.0075) - 333333.333333, 1.0001))
    {
      fclose(fp);

      JUMP_TO_EXIT_ERROR_PROC
    }
  }

  fclose(fp);

  /****************************************/

  fp = fopen("test.bdf", "rb");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x63c, SEEK_SET);

  if(fread(str, 69, 1, fp) != 1)
  {
    fclose(fp);

    JUMP_TO_EXIT_ERROR_PROC
  }

  for(i=0; i<23; i++)
  {
    var.four[0] = str[i * 3];
    var.four[1] = str[i * 3 + 1];
    var.four[2] = str[i * 3 + 2];

    if(var.four[2]&0x80)
    {
      var.four[3] = 0xff;
    }
    else
    {
      var.four[3] = 0x00;
    }

//    printf("buf[%i]: %i   value: %f\n", i, var.one_signed, ((-30100 + (i * 909)) / 0.01) + 4000000.0);

    if(i == 0)
    {
      if(var.one_signed != 1000000)
      {
        fclose(fp);

        JUMP_TO_EXIT_ERROR_PROC
      }

      continue;
    }

    if(dblcmp_lim(var.one_signed, ((-30100 + (i * 909)) / 0.01) + 4000000.0, 1.0001))
    {
      fclose(fp);

      JUMP_TO_EXIT_ERROR_PROC
    }
  }

  fclose(fp);

  /****************************************/

  fp = fopen("test.bdf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x37f, SEEK_SET);

  fputc('7', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.bdf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.bdf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x37f, SEEK_SET);

  fputc('8', fp);

  fseek(fp, 0x39e, SEEK_SET);

  fputc('6', fp);

  fclose(fp);

  if(edfopen_file_readonly("test.bdf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.bdf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x39e, SEEK_SET);

  fputc('7', fp);

  fseek(fp, 0x318, SEEK_SET);

  fwrite("1 ", 2, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.bdf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.bdf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x318, SEEK_SET);

  fwrite("-1", 2, 1, fp);

  fseek(fp, 0x358, SEEK_SET);

  fwrite("2000000 ", 8, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.bdf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.bdf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x358, SEEK_SET);

  fwrite("1000000 ", 8, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.bdf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.bdf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x358, SEEK_SET);

  fwrite("-1000000", 8, 1, fp);

  fseek(fp, 0xec, SEEK_SET);

  fwrite("+9", 2, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.bdf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) != 0)  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdr1.handle))  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.bdf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xec, SEEK_SET);

  fwrite("-9", 2, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.bdf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.bdf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xec, SEEK_SET);

  fwrite("-1", 2, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.bdf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.bdf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xec, SEEK_SET);

  fwrite("0 ", 2, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.bdf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.bdf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xec, SEEK_SET);

  fwrite(" 9", 2, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.bdf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.bdf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xec, SEEK_SET);

  fwrite("9 ", 2, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.bdf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) != 0)  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdr1.handle))  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.bdf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x358, SEEK_SET);

  fwrite("-8388609", 8, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.bdf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.bdf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x358, SEEK_SET);

  fwrite("-1000000", 8, 1, fp);

  fseek(fp, 0x380, SEEK_SET);

  fwrite("8388608 ", 8, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.bdf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.bdf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0x380, SEEK_SET);

  fwrite("1000000 ", 8, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.bdf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) != 0)  JUMP_TO_EXIT_ERROR_PROC

  hdl1 = hdr1.handle;

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = -1;

    /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xae, SEEK_SET);

  fwrite("15", 2, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) == 0)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.filetype != EDFLIB_FILE_CONTAINS_FORMAT_ERRORS)  JUMP_TO_EXIT_ERROR_PROC

  /****************************************/

  fp = fopen("test.edf", "r+b");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  fseek(fp, 0xae, SEEK_SET);

  fwrite("17 ", 2, 1, fp);

  fclose(fp);

  if(edfopen_file_readonly("test.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS) != 0)  JUMP_TO_EXIT_ERROR_PROC

  hdl1 = hdr1.handle;

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = -1;

/********************************** EDF writing ******************************/

  hdl1 = edfopen_file_writeonly("test2.edf", EDFLIB_FILETYPE_EDFPLUS, 1);

  if(hdl1 < 0)  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_samplefrequency(hdl1, 0, 100))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_maximum(hdl1, 0, 1000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_minimum(hdl1, 0, -1000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_maximum(hdl1, 0, 32767))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_minimum(hdl1, 0, -32768))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patient_additional(hdl1, "Test"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_recording_additional(hdl1, "tEST"))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<100; i++)
  {
    dbuf[i] = 0;
  }

  if(edfwrite_physical_samples(hdl1, dbuf))  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = -1;

/********************************** EDF reading ******************************/

  if(edfopen_file_readonly("test2.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS))  JUMP_TO_EXIT_ERROR_PROC

  hdl1 = hdr1.handle;

  if(hdr1.filetype != 1)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.edfsignals != 1)  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.patient_additional, "Test", 4))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.recording_additional, "tEST", 4))  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = -1;

/********************************** EDF writing ******************************/

  hdl1 = edfopen_file_writeonly("test3.edf", EDFLIB_FILETYPE_EDFPLUS, 1);

  if(hdl1 < 0)  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_datarecord_duration(hdl1, 77777))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_subsecond_starttime(hdl1, 1234000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_number_of_annotation_signals(hdl1, 3))  JUMP_TO_EXIT_ERROR_PROC

  char str_korea[]={0xeb,0x8c,0x80,0xed,0x95,0x9c,0xeb,0xaf,0xbc,0xea,0xb5,0xad,0x00};

  char str_accented[]={0xc3,0xb1,0xe1,0xb8,0xb1,0xc3,0xb6,0xc3,0xa8,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x00};

  for(i=0; i<60; i++)
  {
    l_tmp = 10000LL * (i + 1);

    snprintf(str, 4096, "test %i sec", (int)(l_tmp / 10000LL));

    if(i != 0)
    {
     if(edfwrite_annotation_latin1(hdl1, l_tmp, -1LL, str))  JUMP_TO_EXIT_ERROR_PROC
    }
    else
    {
     if(edfwrite_annotation_utf8(hdl1, l_tmp, -1LL, str_accented))  JUMP_TO_EXIT_ERROR_PROC
    }

    l_tmp += 3333LL;

    snprintf(str, 4096, "test %i.%04i sec", (int)(l_tmp / 10000LL), (int)(l_tmp % 10000LL));

    if(i != 0)
    {
      if(edfwrite_annotation_latin1(hdl1, l_tmp, -1LL, str))  JUMP_TO_EXIT_ERROR_PROC
    }
    else
    {
     if(edfwrite_annotation_utf8(hdl1, l_tmp, -1LL, str_korea))  JUMP_TO_EXIT_ERROR_PROC
    }
  }

  if(edf_set_samplefrequency(hdl1, 0, 100))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_maximum(hdl1, 0, 1000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_physical_minimum(hdl1, 0, -1000))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_maximum(hdl1, 0, 32767))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_digital_minimum(hdl1, 0, -32768))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_startdatetime(hdl1, 2008, 12, 31, 23, 59, 58))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patientname(hdl1, "Ãlpha"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patientcode(hdl1, "Bràvó"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_gender(hdl1, 1))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_birthdate(hdl1, 2005, 7, 4))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_patient_additional(hdl1, "Charlie"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_admincode(hdl1, "Dëlta"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_technician(hdl1, "Ëcho"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_equipment(hdl1, "Foxtröt"))  JUMP_TO_EXIT_ERROR_PROC

  if(edf_set_recording_additional(hdl1, "Golf"))  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<100; i++)
  {
    dbuf[i] = 0;
  }

  for(i=0; i<40; i++)
  {
    if(edfwrite_physical_samples(hdl1, dbuf))  JUMP_TO_EXIT_ERROR_PROC
  }

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = -1;

/********************************** EDF reading ******************************/

  if(edfopen_file_readonly("test3.edf", &hdr1, EDFLIB_READ_ALL_ANNOTATIONS))  JUMP_TO_EXIT_ERROR_PROC

  hdl1 = hdr1.handle;

  if(hdr1.filetype != 1)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.edfsignals != 1)  JUMP_TO_EXIT_ERROR_PROC

  for(i=0; i<60; i++)
  {
    if(edf_get_annotation(hdl1, i * 2, &annot))  JUMP_TO_EXIT_ERROR_PROC

//    printf("i: %i   onset: %lli\n", i, annot.onset);

    if(annot.onset != 10000000LL * (i + 1))  JUMP_TO_EXIT_ERROR_PROC

    if(edf_get_annotation(hdl1, i * 2 + 1, &annot))  JUMP_TO_EXIT_ERROR_PROC

    if(annot.onset != 10000000LL * (i + 1) + 3333000LL)  JUMP_TO_EXIT_ERROR_PROC

//    printf("i: %i   onset: %lli\n", i * 2 + 1, annot.onset);
  }

  if(strncmp(hdr1.patientcode, "Bravo", 5))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.gender, "Male", 4))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.birthdate, "04 jul 2005", 11))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.patient_additional, "Charlie", 7))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.admincode, "Delta", 5))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.technician, "Echo", 4))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.equipment, "Foxtrot", 7))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(hdr1.recording_additional, "Golf", 4))  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = -1;

  /****************************************/

  fp = fopen("test3.edf", "rb");

  if(fp == NULL)  JUMP_TO_EXIT_ERROR_PROC

  if(fread(str, 256, 1, fp) != 1)  JUMP_TO_EXIT_ERROR_PROC

  fclose(fp);

  if(strncmp(str + 8, "Bravo M 04-JUL-2005 Alpha Charlie"
    "                                               ", 80))  JUMP_TO_EXIT_ERROR_PROC

  if(strncmp(str + 88, "Startdate 31-DEC-2008 Delta Echo Foxtrot Golf"
    "                                   ", 80))  JUMP_TO_EXIT_ERROR_PROC

/********************************** EDF reading ******************************/

  if(edfopen_file_readonly("test3.edf", &hdr1, EDFLIB_DO_NOT_READ_ANNOTATIONS))  JUMP_TO_EXIT_ERROR_PROC

  hdl1 = hdr1.handle;

  if(hdr1.filetype != 1)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.edfsignals != 1)  JUMP_TO_EXIT_ERROR_PROC

  if(hdr1.starttime_subsecond!=1234000LL)  JUMP_TO_EXIT_ERROR_PROC

  if(edfclose_file(hdl1))
  {
    hdl1 = -1;

    JUMP_TO_EXIT_ERROR_PROC
  }

  hdl1 = -1;

  /****************************************/

  free(ibuf);
  free(sbuf);
  free(dbuf);
  free(str);
  free(pbuf);

  return EXIT_SUCCESS;

OUT_ERROR:

  if(hdl1 >= 0)
  {
    edfclose_file(hdl1);
  }

  free(ibuf);
  free(sbuf);
  free(dbuf);
  free(str);
  free(pbuf);

  fprintf(stderr, "Error, line %i file %s\n", line, __FILE__);

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

















