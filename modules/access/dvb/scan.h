/*****************************************************************************
 * scan.h : functions to ease DVB scanning
 *****************************************************************************
 * Copyright (C) 2008,2010 VLC authors and VideoLAN
 *
 * Authors: Laurent Aimar <fenrir@videolan.org>
 *          David Kaplan <david@2of1.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.    See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA    02111, USA.
 *****************************************************************************/

typedef enum
{
    SCAN_NONE = 0,
    SCAN_DVB_T,
    SCAN_DVB_S,
    SCAN_DVB_C,
} scan_type_t;

typedef struct
{
    unsigned i_frequency;
    union
    {
        unsigned i_bandwidth;
        unsigned i_symbolrate;
    };
    int i_fec;
    int i_modulation;
    char c_polarization;

} scan_tuner_config_t;

typedef struct scan_parameter_t
{
    scan_type_t type;
    bool b_exhaustive;
    bool b_use_nit;
    bool b_free_only;
    bool b_modulation_set;
    bool b_symbolrate_set;

    int i_modulation;
    int i_symbolrate;
    struct
    {
        unsigned i_min;
        unsigned i_max;
        unsigned i_step;

        unsigned i_count;    /* Number of frequency test to do */
    } frequency,
      bandwidth; /* Bandwidth should be 6, 7 or 8 */

    char *psz_scanlist_file;
    enum
    {
        FORMAT_DVBv3,
        FORMAT_DVBv5,
    } scanlist_format;

} scan_parameter_t;

typedef struct scan_t scan_t;

void scan_parameter_Init( scan_parameter_t * );
void scan_parameter_Clean( scan_parameter_t * );

scan_t *scan_New( vlc_object_t *p_obj, const scan_parameter_t *p_parameter );
void scan_Destroy( scan_t *p_scan );

int scan_Next( scan_t *p_scan, scan_tuner_config_t *p_cfg );

block_t *scan_GetM3U( scan_t *p_scan );
bool scan_IsCancelled( scan_t *p_scan );

typedef struct scan_session_t scan_session_t;

scan_session_t *scan_session_New( scan_t *, const scan_tuner_config_t * );
void scan_session_Destroy( scan_t *, scan_session_t * );
bool scan_session_Push( scan_session_t *p_scan, block_t *p_block );
void scan_session_SetSNR( scan_session_t *p_scan, int i_snr );

