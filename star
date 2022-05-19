HEXDUMP(1)                              User Commands                             HEXDUMP(1)

NNAAMMEE
       hexdump - display file contents in hexadecimal, decimal, octal, or ascii

       hheexxdduummpp _o_p_t_i_o_n_s _f_i_l_e ...

       hhdd _o_p_t_i_o_n_s _f_i_l_e ...

DDEESSCCRRIIPPTTIIOONN
       The hheexxdduummpp utility is a filter which displays the specified files, or standard input
       if no files are specified, in a user-specified format.

OOPPTTIIOONNSS
       Below, the _l_e_n_g_t_h and _o_f_f_s_e_t arguments may be followed by the multiplicative suffixes
       KiB (=1024), MiB (=1024*1024), and so on for GiB, TiB, PiB, EiB, ZiB and YiB (the
       "iB" is optional, e.g., "K" has the same meaning as "KiB"), or the suffixes KB
       (=1000), MB (=1000*1000), and so on for GB, TB, PB, EB, ZB and YB.

       --bb, ----oonnee--bbyyttee--ooccttaall
           _O_n_e_-_b_y_t_e _o_c_t_a_l _d_i_s_p_l_a_y. Display the input offset in hexadecimal, followed by
           sixteen space-separated, three-column, zero-filled bytes of input data, in octal,
           per line.

       --cc, ----oonnee--bbyyttee--cchhaarr
           _O_n_e_-_b_y_t_e _c_h_a_r_a_c_t_e_r _d_i_s_p_l_a_y. Display the input offset in hexadecimal, followed by
           sixteen space-separated, three-column, space-filled characters of input data per
           line.

       --CC, ----ccaannoonniiccaall
           _C_a_n_o_n_i_c_a_l _h_e_x_+_A_S_C_I_I _d_i_s_p_l_a_y. Display the input offset in hexadecimal, followed by
           sixteen space-separated, two-column, hexadecimal bytes, followed by the same
           sixteen bytes in %%__pp format enclosed in || characters. Invoking the program as hhdd
           implies this option.

       --dd, ----ttwwoo--bbyytteess--ddeecciimmaall
           _T_w_o_-_b_y_t_e _d_e_c_i_m_a_l _d_i_s_p_l_a_y. Display the input offset in hexadecimal, followed by
           eight space-separated, five-column, zero-filled, two-byte units of input data, in
           unsigned decimal, per line.

       --ee, ----ffoorrmmaatt _f_o_r_m_a_t___s_t_r_i_n_g
           Specify a format string to be used for displaying data.

       --ff, ----ffoorrmmaatt--ffiillee _f_i_l_e
           Specify a file that contains one or more newline-separated format strings. Empty
           lines and lines whose first non-blank character is a hash mark (#) are ignored.

       --LL, ----ccoolloorr[=_w_h_e_n]
           Accept color units for the output. The optional argument _w_h_e_n can be aauuttoo, nneevveerr
           or aallwwaayyss. If the _w_h_e_n argument is omitted, it defaults to aauuttoo. The colors can
           be disabled; for the current built-in default see the ----hheellpp output. See also the
           CCoolloorrss subsection and the CCOOLLOORRSS section below.

       --nn, ----lleennggtthh _l_e_n_g_t_h
           Interpret only _l_e_n_g_t_h bytes of input.

       --oo, ----ttwwoo--bbyytteess--ooccttaall
           _T_w_o_-_b_y_t_e _o_c_t_a_l _d_i_s_p_l_a_y. Display the input offset in hexadecimal, followed by
           eight space-separated, six-column, zero-filled, two-byte quantities of input
           data, in octal, per line.

       --ss, ----sskkiipp _o_f_f_s_e_t
           Skip _o_f_f_s_e_t bytes from the beginning of the input.

       --vv, ----nnoo--ssqquueeeezziinngg
           The --vv option causes hheexxdduummpp to display all input data. Without the --vv option,
           any number of groups of output lines which would be identical to the immediately
           preceding group of output lines (except for the input offsets), are replaced with
           a line comprised of a single asterisk.

       --xx, ----ttwwoo--bbyytteess--hheexx
           _T_w_o_-_b_y_t_e _h_e_x_a_d_e_c_i_m_a_l _d_i_s_p_l_a_y. Display the input offset in hexadecimal, followed
           by eight space-separated, four-column, zero-filled, two-byte quantities of input
           data, in hexadecimal, per line.

       --hh, ----hheellpp
           Display help text and exit.

       --VV, ----vveerrssiioonn
           Print version and exit.

       For each input file, hheexxdduummpp sequentially copies the input to standard output,
       transforming the data according to the format strings specified by the --ee and --ff
       options, in the order that they were specified.

FFOORRMMAATTSS
       A format string contains any number of format units, separated by whitespace. A
       format unit contains up to three items: an iteration count, a byte count, and a
       format.

       The iteration count is an optional positive integer, which defaults to one. Each
       format is applied iteration count times.

       The byte count is an optional positive integer. If specified it defines the number of
       bytes to be interpreted by each iteration of the format.

       If an iteration count and/or a byte count is specified, a single slash must be placed
       after the iteration count and/or before the byte count to disambiguate them. Any
       whitespace before or after the slash is ignored.

       The format is required and must be surrounded by double quote (" ") marks. It is
       interpreted as a fprintf-style format string (see ffpprriinnttff(3)), with the following
       exceptions:

       1.
           An asterisk (*) may not be used as a field width or precision.

       2.
           A byte count or field precision _i_s required for each ss conversion character
           (unlike the ffpprriinnttff(3) default which prints the entire string if the precision is
           unspecified).

       3.
           The conversion characters hh, ll, nn, pp, and qq are not supported.

       4.
           The single character escape sequences described in the C standard are supported:

          ┌──────────────────┬────┐
          │                  │    │
          │NULL              │ \0 │
          ├──────────────────┼────┤
          │                  │    │
          │<alert character> │ \a │
          ├──────────────────┼────┤
          │                  │    │
          │<backspace>       │ \b │
          ├──────────────────┼────┤
          │                  │    │
          │<form-feed>       │ \f │
          ├──────────────────┼────┤
          │                  │    │
          │<newline>         │ \n │
          ├──────────────────┼────┤
          │                  │    │
          │<carriage return> │ \r │
          ├──────────────────┼────┤
          │                  │    │
          │<tab>             │ \t │
          ├──────────────────┼────┤
          │                  │    │
          │<vertical tab>    │ \v │
          └──────────────────┴────┘

   CCoonnvveerrssiioonn ssttrriinnggss
       The hheexxdduummpp utility also supports the following additional conversion strings.

       __aa[[ddooxx]]
           Display the input offset, cumulative across input files, of the next byte
           to be displayed. The appended characters dd, oo, and xx specify the display
           base as decimal, octal or hexadecimal respectively.

       __AA[[ddooxx]]
           Almost identical to the __aa conversion string except that it is only
           performed once, when all of the input data has been processed.

       __cc
           Output characters in the default character set. Non-printing characters are
           displayed in three-character, zero-padded octal, except for those
           representable by standard escape notation (see above), which are displayed
           as two-character strings.

       __pp
           Output characters in the default character set. Non-printing characters are
           displayed as a single '..'.

       __uu
           Output US ASCII characters, with the exception that control characters are
           displayed using the following, lower-case, names. Characters greater than
           0xff, hexadecimal, are displayed as hexadecimal strings.

          ┌────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
          │        │         │         │         │         │         │
          │000 nul │ 001 soh │ 002 stx │ 003 etx │ 004 eot │ 005 enq │
          ├────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
          │        │         │         │         │         │         │
          │006 ack │ 007 bel │ 008 bs  │ 009 ht  │ 00A lf  │ 00B vt  │
          ├────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
          │        │         │         │         │         │         │
          │00C ff  │ 00D cr  │ 00E so  │ 00F si  │ 010 dle │ 011 dc1 │
          ├────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
          │        │         │         │         │         │         │
          │012 dc2 │ 013 dc3 │ 014 dc4 │ 015 nak │ 016 syn │ 017 etb │
          ├────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
          │        │         │         │         │         │         │
          │018 can │ 019 em  │ 01A sub │ 01B esc │ 01C fs  │ 01D gs  │
          ├────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
          │        │         │         │         │         │         │
          │01E rs  │ 01F us  │ 0FF del │         │         │         │
          └────────┴─────────┴─────────┴─────────┴─────────┴─────────┘

   CCoolloorrss
       When put at the end of a format specifier, hheexxdduummpp highlights the
       respective string with the color specified. Conditions, if present, are
       evaluated prior to highlighting.

       __LL[[ccoolloorr__uunniitt__11,,ccoolloorr__uunniitt__22,,......,,ccoolloorr__uunniitt__nn]]

       The full syntax of a color unit is as follows:

       [[!!]]CCOOLLOORR[[::VVAALLUUEE]][[@@OOFFFFSSEETT__SSTTAARRTT[[--EENNDD]]]]

       !!
           Negate the condition. Please note that it only makes sense to negate
           a unit if both a value/string and an offset are specified. In that
           case the respective output string will be highlighted if and only if
           the value/string does not match the one at the offset.

       CCOOLLOORR
           One of the 8 basic shell colors.

       VVAALLUUEE
           A value to be matched specified in hexadecimal, or octal base, or as
           a string. Please note that the usual C escape sequences are not
           interpreted by hheexxdduummpp inside the color_units.

       OOFFFFSSEETT
           An offset or an offset range at which to check for a match. Please
           note that lone OFFSET_START uses the same value as END offset.

   CCoouunntteerrss
       The default and supported byte counts for the conversion characters are
       as follows:

       %%__cc, %%__pp, %%__uu, %%cc
           One byte counts only.

       %%dd, %%ii, %%oo, %%uu, %%XX, %%xx
           Four byte default, one, two and four byte counts supported.

       %%EE, %%ee, %%ff, %%GG, %%gg
           Eight byte default, four byte counts supported.

       The amount of data interpreted by each format string is the sum of the
       data required by each format unit, which is the iteration count times the
       byte count, or the iteration count times the number of bytes required by
       the format if the byte count is not specified.

       The input is manipulated in _b_l_o_c_k_s, where a block is defined as the
       largest amount of data specified by any format string. Format strings
       interpreting less than an input block’s worth of data, whose last format
       unit both interprets some number of bytes and does not have a specified
       iteration count, have the iteration count incremented until the entire
       input block has been processed or there is not enough data remaining in
       the block to satisfy the format string.

       If, either as a result of user specification or hheexxdduummpp modifying the
       iteration count as described above, an iteration count is greater than
       one, no trailing whitespace characters are output during the last
       iteration.

       It is an error to specify a byte count as well as multiple conversion
       characters or strings unless all but one of the conversion characters or
       strings is __aa or __AA.

       If, as a result of the specification of the --nn option or end-of-file
       being reached, input data only partially satisfies a format string, the
       input block is zero-padded sufficiently to display all available data
       (i.e., any format units overlapping the end of data will display some
       number of the zero bytes).

       Further output by such format strings is replaced by an equivalent number
       of spaces. An equivalent number of spaces is defined as the number of
       spaces output by an ss conversion character with the same field width and
       precision as the original conversion character or conversion string but
       with any '++', ' ', '##' conversion flag characters removed, and
       referencing a NULL string.

       If no format strings are specified, the default display is very similar
       to the --xx output format (the --xx option causes more space to be used
       between format units than in the default output).

EEXXIITT SSTTAATTUUSS
       hheexxdduummpp exits 0 on success and > 0 if an error occurred.

CCOONNFFOORRMMIINNGG TTOO
       The hheexxdduummpp utility is expected to be IEEE Std 1003.2 ("POSIX.2")
       compatible.

EEXXAAMMPPLLEESS
       Display the input in perusal format:

              "%06.6_ao "  12/1 "%3_u "
              "\t" "%_p "
              "\n"

       Implement the --xx option:

              "%07.7_Ax\n"
              "%07.7_ax  " 8/2 "%04x " "\n"

       MBR Boot Signature example: Highlight the addresses cyan and the bytes at
       offsets 510 and 511 green if their value is 0xAA55, red otherwise.

              "%07.7_Ax_L[cyan]\n"
              "%07.7_ax_L[cyan]  " 8/2 "   %04x_L[green:0xAA55@510-511,!red:0xAA55@510-511] " "\n"

CCOOLLOORRSS
       The output colorization is implemented by tteerrmmiinnaall--ccoolloorrss..dd(5)
       functionality. Implicit coloring can be disabled by an empty file

          _/_e_t_c_/_t_e_r_m_i_n_a_l_-_c_o_l_o_r_s_._d_/_h_e_x_d_u_m_p_._d_i_s_a_b_l_e

       for the hheexxdduummpp command or for all tools by

          _/_e_t_c_/_t_e_r_m_i_n_a_l_-_c_o_l_o_r_s_._d_/_d_i_s_a_b_l_e

       The user-specific _$_X_D_G___C_O_N_F_I_G___H_O_M_E_/_t_e_r_m_i_n_a_l_-_c_o_l_o_r_s_._d or
       _$_H_O_M_E_/_._c_o_n_f_i_g_/_t_e_r_m_i_n_a_l_-_c_o_l_o_r_s_._d overrides the global setting.

       Note that the output colorization may be enabled by default, and in this
       case _t_e_r_m_i_n_a_l_-_c_o_l_o_r_s_._d directories do not have to exist yet.

RREEPPOORRTTIINNGG BBUUGGSS
       For bug reports, use the issue tracker at
       https://github.com/util-linux/util-linux/issues.

AAVVAAIILLAABBIILLIITTYY
       The hheexxdduummpp command is part of the util-linux package which can be
       downloaded from Linux Kernel Archive
       <https://www.kernel.org/pub/linux/utils/util-linux/>.

util-linux 2.38                          2022-02-17                               HEXDUMP(1)
