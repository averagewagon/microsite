/* C code produced by gperf version 3.1 */
/* Command-line: gperf --output-file=mime_gperf_output.c mime.gperf  */
/* Computed positions: -k'1-4' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "mime.gperf"

#include <string.h>
#line 10 "mime.gperf"
struct mime_entry {
    const char *extension;
    const char *mime_type;
};

#define TOTAL_KEYWORDS 77
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 6
#define MIN_HASH_VALUE 10
#define MAX_HASH_VALUE 274
/* maximum key range = 265, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (str, len)
     register const char *str;
     register size_t len;
{
  static const unsigned short asso_values[] =
    {
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
       60,  55,  75, 275, 275,   5, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275,  20, 110,   0,
       25,  85,   0,  55,   7,  20,  75,   0,  92,  45,
       30,  30,  55, 275,   2,  10,   5,   0,  60,   5,
        2, 275,  17, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275, 275, 275, 275, 275,
      275, 275, 275, 275, 275, 275
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

const struct mime_entry *
in_word_set (str, len)
     register const char *str;
     register size_t len;
{
  static const struct mime_entry wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 68 "mime.gperf"
      {"rtf", "application/rtf"},
      {""}, {""},
#line 75 "mime.gperf"
      {"ttf", "font/ttf"},
      {""},
#line 76 "mime.gperf"
      {"txt", "text/plain"},
      {""},
#line 74 "mime.gperf"
      {"ts", "video/mp2t"},
      {""},
#line 69 "mime.gperf"
      {"sh", "application/x-sh"},
#line 28 "mime.gperf"
      {"csh", "application/x-csh"},
      {""}, {""},
#line 29 "mime.gperf"
      {"css", "text/css"},
#line 92 "mime.gperf"
      {"7z", "application/x-7z-compressed"},
#line 19 "mime.gperf"
      {"arc", "application/x-freearc"},
      {""},
#line 67 "mime.gperf"
      {"rar", "application/vnd.rar"},
#line 72 "mime.gperf"
      {"tif", "image/tiff"},
#line 73 "mime.gperf"
      {"tiff", "image/tiff"},
#line 71 "mime.gperf"
      {"tar", "application/x-tar"},
      {""}, {""},
#line 40 "mime.gperf"
      {"ics", "text/calendar"},
      {""}, {""}, {""}, {""},
#line 61 "mime.gperf"
      {"otf", "font/otf"},
#line 82 "mime.gperf"
      {"woff", "font/woff"},
#line 83 "mime.gperf"
      {"woff2", "font/woff2"},
      {""}, {""},
#line 16 "mime.gperf"
      {"aac", "audio/aac"},
      {""},
#line 22 "mime.gperf"
      {"azw", "application/vnd.amazon.ebook"},
      {""}, {""},
#line 27 "mime.gperf"
      {"cda", "application/x-cdf"},
      {""}, {""}, {""}, {""},
#line 39 "mime.gperf"
      {"ico", "image/vnd.microsoft.icon"},
      {""}, {""}, {""}, {""},
#line 31 "mime.gperf"
      {"doc", "application/msword"},
      {""},
#line 37 "mime.gperf"
      {"htm", "text/html"},
#line 32 "mime.gperf"
      {"docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
      {""},
#line 56 "mime.gperf"
      {"odt", "application/vnd.oasis.opendocument.text"},
#line 84 "mime.gperf"
      {"xhtml", "application/xhtml+xml"},
      {""}, {""}, {""},
#line 55 "mime.gperf"
      {"ods", "application/vnd.oasis.opendocument.spreadsheet"},
      {""}, {""}, {""}, {""},
#line 30 "mime.gperf"
      {"csv", "text/csv"},
#line 35 "mime.gperf"
      {"gz", "application/gzip"},
      {""}, {""}, {""},
#line 36 "mime.gperf"
      {"gif", "image/gif"},
      {""}, {""}, {""}, {""},
#line 63 "mime.gperf"
      {"pdf", "application/pdf"},
      {""}, {""}, {""},
#line 44 "mime.gperf"
      {"js", "text/javascript"},
#line 78 "mime.gperf"
      {"wav", "audio/wav"},
      {""},
#line 59 "mime.gperf"
      {"ogx", "application/ogg"},
      {""}, {""},
#line 47 "mime.gperf"
      {"mid", "audio/midi"},
      {""},
#line 89 "mime.gperf"
      {"zip", "application/zip"},
      {""},
#line 88 "mime.gperf"
      {"xul", "application/vnd.mozilla.xul+xml"},
#line 77 "mime.gperf"
      {"vsd", "application/vnd.visio"},
#line 60 "mime.gperf"
      {"opus", "audio/opus"},
#line 41 "mime.gperf"
      {"jar", "application/java-archive"},
      {""}, {""},
#line 21 "mime.gperf"
      {"avi", "video/x-msvideo"},
#line 20 "mime.gperf"
      {"avif", "image/avif"},
      {""}, {""},
#line 85 "mime.gperf"
      {"xls", "application/vnd.ms-excel"},
#line 57 "mime.gperf"
      {"oga", "audio/ogg"},
      {""},
#line 86 "mime.gperf"
      {"xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
      {""}, {""},
#line 54 "mime.gperf"
      {"odp", "application/vnd.oasis.opendocument.presentation"},
#line 48 "mime.gperf"
      {"midi", "audio/midi"},
      {""}, {""}, {""},
#line 65 "mime.gperf"
      {"ppt", "application/vnd.ms-powerpoint"},
      {""},
#line 64 "mime.gperf"
      {"php", "application/x-httpd-php"},
#line 66 "mime.gperf"
      {"pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
      {""},
#line 33 "mime.gperf"
      {"eot", "application/vnd.ms-fontobject"},
      {""}, {""}, {""}, {""},
#line 70 "mime.gperf"
      {"svg", "image/svg+xml"},
#line 25 "mime.gperf"
      {"bz", "application/x-bzip"},
      {""}, {""}, {""},
#line 49 "mime.gperf"
      {"mjs", "text/javascript"},
      {""}, {""}, {""}, {""},
#line 17 "mime.gperf"
      {"abw", "application/x-abiword"},
      {""}, {""}, {""},
#line 87 "mime.gperf"
      {"xml", "application/xml"},
#line 62 "mime.gperf"
      {"png", "image/png"},
      {""}, {""}, {""}, {""},
#line 58 "mime.gperf"
      {"ogv", "video/ogg"},
#line 45 "mime.gperf"
      {"json", "application/json"},
      {""},
#line 46 "mime.gperf"
      {"jsonld", "application/ld+json"},
      {""},
#line 38 "mime.gperf"
      {"html", "text/html"},
      {""}, {""}, {""}, {""},
#line 50 "mime.gperf"
      {"mp3", "audio/mpeg"},
#line 53 "mime.gperf"
      {"mpkg", "application/vnd.apple.installer+xml"},
      {""}, {""}, {""},
#line 23 "mime.gperf"
      {"bin", "application/octet-stream"},
#line 18 "mime.gperf"
      {"apng", "image/apng"},
      {""}, {""}, {""},
#line 90 "mime.gperf"
      {"3gp", "video/3gpp"},
      {""}, {""}, {""}, {""},
#line 91 "mime.gperf"
      {"3g2", "video/3gpp2"},
      {""}, {""}, {""}, {""},
#line 51 "mime.gperf"
      {"mp4", "video/mp4"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 43 "mime.gperf"
      {"jpg", "image/jpeg"},
      {""},
#line 26 "mime.gperf"
      {"bz2", "application/x-bzip2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 24 "mime.gperf"
      {"bmp", "image/bmp"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 79 "mime.gperf"
      {"weba", "audio/webm"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 52 "mime.gperf"
      {"mpeg", "video/mpeg"},
      {""}, {""}, {""}, {""},
#line 80 "mime.gperf"
      {"webm", "video/webm"},
      {""}, {""}, {""}, {""},
#line 34 "mime.gperf"
      {"epub", "application/epub+zip"},
      {""}, {""}, {""}, {""},
#line 81 "mime.gperf"
      {"webp", "image/webp"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 42 "mime.gperf"
      {"jpeg", "image/jpeg"}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].extension;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
