#if defined(__OS2__)
 #define NAME_MAX   255     /* maximum filename for HPFS */
#elif defined(__NT__) || defined(__WATCOM_LFN__)
 #define NAME_MAX   259     /* maximum filename for NTFS and FAT LFN */
#else
 #define NAME_MAX   12      /* 8 chars + '.' +  3 chars */
#endif

#define _LFN_SIGN   0x004e464cUL    // "LFN"