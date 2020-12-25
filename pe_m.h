/*** coff information for Intel 386/486.  */
#ifndef _PE_H
#define _PE_H


/********************** FILE HEADER **********************/
typedef struct {
  unsigned short f_magic;         /* magic number             */
  unsigned short f_nscns;         /* number of sections       */
  unsigned long  f_timdat;        /* time & date stamp        */
  unsigned long  f_symptr;        /* file pointer to symtab   */
  unsigned long  f_nsyms;         /* number of symtab entries */
  unsigned short f_opthdr;        /* sizeof(optional hdr)     */
  unsigned short f_flags;         /* flags                    */
} FILHDR;
//--------------------------------------------------------------------
/* Bits for f_flags:
 *      F_RELFLG        relocation info stripped from file
 *      F_EXEC          file is executable (no unresolved external references)
 *      F_LNNO          line numbers stripped from file
 *      F_LSYMS         local symbols stripped from file
 *      F_AR32WR        file has byte ordering of an AR32WR machine (e.g. vax)
 */

#define F_RELFLG        (0x0001)
#define F_EXEC          (0x0002)
#define F_LNNO          (0x0004)
#define F_LSYMS         (0x0008)



#define I386MAGIC       0x14c
#define I386PTXMAGIC    0x154
#define I386AIXMAGIC    0x175

/* This is Lynx's all-platform magic number for executables. */

#define LYNXCOFFMAGIC   0415

#define I386BADMAG(x) (((x).f_magic != I386MAGIC) \
                       && (x).f_magic != I386AIXMAGIC \
                       && (x).f_magic != I386PTXMAGIC \
                       && (x).f_magic != LYNXCOFFMAGIC)

#define FILHSZ  20
//--------------------------------------------------------------------

/********************** AOUT "OPTIONAL HEADER" **********************/
typedef struct {
  unsigned short magic;          /* type of file                         */
  unsigned short vstamp;         /* version stamp                        */
  unsigned long  tsize;          /* text size in bytes, padded to FW bdry*/
  unsigned long  dsize;          /* initialized data    "  "             */
  unsigned long  bsize;          /* uninitialized data  "  "             */
  unsigned long  entry;          /* entry pt.                            */
  unsigned long  text_start;     /* base of text used for this file      */
  unsigned long  data_start;     /* base of data used for this file      */
} AOUTHDR;

#define AOUTSZ 28
#define AOUTHDRSZ 28

#define OMAGIC          0404    /* object files, eg as output */
#define ZMAGIC          0413    /* demand load format, eg normal ld output */
#define STMAGIC         0401    /* target shlib */
#define SHMAGIC         0443    /* host   shlib */


/* define some NT default values */
/*  #define NT_IMAGE_BASE        0x400000 moved to internal.h */
#define NT_SECTION_ALIGNMENT 0x1000
#define NT_FILE_ALIGNMENT    0x200
#define NT_DEF_RESERVE       0x100000
#define NT_DEF_COMMIT        0x1000
//----------------------------------------------------------------------


/********************** SECTION HEADER **********************/
typedef struct {
  char           s_name[8];  /* section name                     */
  unsigned long  s_paddr;    /* physical address, aliased s_nlib */
  unsigned long  s_vaddr;    /* virtual address                  */
  unsigned long  s_size;     /* section size                     */
  unsigned long  s_scnptr;   /* file ptr to raw data for section */
  unsigned long  s_relptr;   /* file ptr to relocation           */
  unsigned long  s_lnnoptr;  /* file ptr to line numbers         */
  unsigned short s_nreloc;   /* number of relocation entries     */
  unsigned short s_nlnno;    /* number of line number entries    */
  unsigned long  s_flags;    /* flags                            */
} SCNHDR;

#define SCNHSZ  40

/*
 * names of "special" sections
 */
#ifndef   _main_
 extern   char   _TEXT   [];
 extern   char   _DATA   [];
 extern   char   _BSS    [];
 extern   char   _COMMENT[];
 extern   char   _LIB    [];
#else
 char   _TEXT   []=".text"   ;
 char   _DATA   []=".data"   ;
 char   _BSS    []=".bss"    ;
 char   _COMMENT[]=".comment";
 char   _LIB    []=".lib"    ;
#endif

//----------------------------------------------------------------------
typedef struct {
  unsigned long  r_vaddr;   /* address of relocation      */
  unsigned long  r_symndx;  /* symbol we're adjusting for */
  unsigned short r_type;    /* type of relocation         */
} RELOC;

#define RELSZ 10
#define RELOC_ADDR32 6
#define RELOC_REL32  20

//----------------------------------------------------------------------
typedef struct {
  union {
    unsigned long l_symndx;  /* function name symbol index */
    unsigned long l_paddr;   /* address of line number     */
  } l_addr;
  unsigned short l_lnno;     /* line number                */
} LINENO;
#define LINESZ  6

//----------------------------------------------------------------------
/********************** SYMBOLS **********************/

#define E_SYMNMLEN      8       /* # characters in a symbol name        */
#define E_FILNMLEN      14      /* # characters in a file name          */
#define E_DIMNUM        4       /* # array dimensions in auxiliary entry */



typedef struct {
  union {
    char e_name[E_SYMNMLEN];
    struct {
      unsigned long e_zeroes;
      unsigned long e_offset;
    } e;
  } e;
  unsigned long e_value;
  short e_scnum;
  unsigned short e_type;
  unsigned char e_sclass;
  unsigned char e_numaux;
} SYMENT;

#define N_BTMASK        (0xf)
#define N_TMASK         (0x30)
#define N_BTSHFT        (4)
#define N_TSHIFT        (2)
//----------------------------------------------------------------------

/* NT specific file attributes */
#define IMAGE_FILE_RELOCS_STRIPPED           0x0001
#define IMAGE_FILE_EXECUTABLE_IMAGE          0x0002
#define IMAGE_FILE_LINE_NUMS_STRIPPED        0x0004
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED       0x0008
#define IMAGE_FILE_BYTES_REVERSED_LO         0x0080
#define IMAGE_FILE_32BIT_MACHINE             0x0100
#define IMAGE_FILE_DEBUG_STRIPPED            0x0200
#define IMAGE_FILE_SYSTEM                    0x1000
#define IMAGE_FILE_DLL                       0x2000
#define IMAGE_FILE_BYTES_REVERSED_HI         0x8000

/* additional flags to be set for section headers to allow the NT loader to
   read and write to the section data (to replace the addresses of data in
   dlls for one thing); also to execute the section in .text's case */
#define IMAGE_SCN_MEM_DISCARDABLE 0x02000000
#define IMAGE_SCN_MEM_EXECUTE     0x20000000
#define IMAGE_SCN_MEM_READ        0x40000000
#define IMAGE_SCN_MEM_WRITE       0x80000000

/*
 * Section characteristics added for ppc-nt
 */

#define IMAGE_SCN_TYPE_NO_PAD                0x00000008  /* Reserved. */

#define IMAGE_SCN_CNT_CODE                   0x00000020  /* Section contains code. */
#define IMAGE_SCN_CNT_INITIALIZED_DATA       0x00000040  /* Section contains initialized data. */
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA     0x00000080  /* Section contains uninitialized data. */

#define IMAGE_SCN_LNK_OTHER                  0x00000100  /* Reserved. */
#define IMAGE_SCN_LNK_INFO                   0x00000200  /* Section contains comments or some other type of information. */
#define IMAGE_SCN_LNK_REMOVE                 0x00000800  /* Section contents will not become part of image. */
#define IMAGE_SCN_LNK_COMDAT                 0x00001000  /* Section contents comdat. */

#define IMAGE_SCN_MEM_FARDATA                0x00008000

#define IMAGE_SCN_MEM_PURGEABLE              0x00020000
#define IMAGE_SCN_MEM_16BIT                  0x00020000
#define IMAGE_SCN_MEM_LOCKED                 0x00040000
#define IMAGE_SCN_MEM_PRELOAD                0x00080000

#define IMAGE_SCN_ALIGN_1BYTES               0x00100000
#define IMAGE_SCN_ALIGN_2BYTES               0x00200000
#define IMAGE_SCN_ALIGN_4BYTES               0x00300000
#define IMAGE_SCN_ALIGN_8BYTES               0x00400000
#define IMAGE_SCN_ALIGN_16BYTES              0x00500000  /* Default alignment if no others are specified. */
#define IMAGE_SCN_ALIGN_32BYTES              0x00600000
#define IMAGE_SCN_ALIGN_64BYTES              0x00700000


#define IMAGE_SCN_LNK_NRELOC_OVFL            0x01000000  /* Section contains extended relocations. */
#define IMAGE_SCN_MEM_NOT_CACHED             0x04000000  /* Section is not cachable.               */
#define IMAGE_SCN_MEM_NOT_PAGED              0x08000000  /* Section is not pageable.               */
#define IMAGE_SCN_MEM_SHARED                 0x10000000  /* Section is shareable.                  */

/* COMDAT selection codes.  */

#define IMAGE_COMDAT_SELECT_NODUPLICATES     (1) /* Warn if duplicates.  */
#define IMAGE_COMDAT_SELECT_ANY              (2) /* No warning.  */
#define IMAGE_COMDAT_SELECT_SAME_SIZE        (3) /* Warn if different size.  */
#define IMAGE_COMDAT_SELECT_EXACT_MATCH      (4) /* Warn if different.  */
#define IMAGE_COMDAT_SELECT_ASSOCIATIVE      (5) /* Base on other section.  */

/* Magic values that are true for all dos/nt implementations */
#define DOSMAGIC       0x5a4d
#define NT_SIGNATURE   0x00004550

  /* NT allows long filenames, we want to accommodate this.  This may break
     some of the bfd functions */

//----------------------------------------------------------------------
struct external_PE_filehdr
{
  /* DOS header fields */
  char e_magic[2];              /* Magic number, 0x5a4d              +0  */
  char e_cblp[2];               /* Bytes on last page of file, 0x90  +2  */
  char e_cp[2];                 /* Pages in file, 0x3                +4  */
  char e_crlc[2];               /* Relocations, 0x0                  +6  */
  char e_cparhdr[2];            /* Size of header in paragraphs, 0x4 +8  */
  char e_minalloc[2];           /* Minimum extra paragraphs needed, 0x0    +0Ah */
  char e_maxalloc[2];           /* Maximum extra paragraphs needed, 0xFFFF +0Ch */
  char e_ss[2];                 /* Initial (relative) SS value, 0x0  +0Eh   */
  char e_sp[2];                 /* Initial SP value, 0xb8            +10h   */
  char e_csum[2];               /* Checksum, 0x0                     +12h   */
  char e_ip[2];                 /* Initial IP value, 0x0             +14h   */
  char e_cs[2];                 /* Initial (relative) CS value, 0x0  +16h   */
  char e_lfarlc[2];             /* File address of relocation table, 0x40 +18h */
  char e_ovno[2];               /* Overlay number, 0x0               +1Ah   */
  char e_res[4][2];             /* Reserved words, all 0x0           +1Ch   */
  char e_oemid[2];              /* OEM identifier (for e_oeminfo), 0x0 +24h */
  char e_oeminfo[2];            /* OEM information; e_oemid specific, 0x0 +26h */
  char e_res2[10][2];           /* Reserved words, all 0x0                +28h */
  char e_lfanew[4];             /* File address of new exe header, 0x80   +3Ch */
  char dos_message[16][4];      /* other stuff, always follow DOS header  +40h */
  char nt_signature[4];         /* required NT signature, 0x4550          +80h */

  /* From standard header */


  char f_magic[2];              /* magic number                 +84h */
  char f_nscns[2];              /* number of sections           +86h */
  char f_timdat[4];             /* time & date stamp            +88h */
  char f_symptr[4];             /* file pointer to symtab       +8Ch */
  char f_nsyms[4];              /* number of symtab entries     +90h */
  char f_opthdr[2];             /* sizeof(optional hdr)         +94h */
  char f_flags[2];              /* flags                        +96h */

};
#define PEFILHSZ 152
//----------------------------------------------------------------------
typedef struct
{
  AOUTHDR standard;

  /* NT extra fields; see internal.h for descriptions */
  long  ImageBase;
  long  SectionAlignment;
  long  FileAlignment;
  short  MajorOperatingSystemVersion;
  short  MinorOperatingSystemVersion;
  short  MajorImageVersion;
  short  MinorImageVersion;
  short  MajorSubsystemVersion;
  short  MinorSubsystemVersion;
  long  Reserved1;
  long  SizeOfImage;
  long  SizeOfHeaders;
  long  CheckSum;
  short Subsystem;
  short DllCharacteristics;
  long  SizeOfStackReserve;
  long  SizeOfStackCommit;
  long  SizeOfHeapReserve;
  long  SizeOfHeapCommit;
  long  LoaderFlags;
  long  NumberOfRvaAndSizes;
  /* IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES]; */
  long  DataDirectory[16][2]; /* 16 entries, 2 elements/entry, 4 chars */
} PEAOUTHDR;

#define PEAOUTSZ (AOUTHDRSZ + 196)
//----------------------------------------------------------------------
#endif


