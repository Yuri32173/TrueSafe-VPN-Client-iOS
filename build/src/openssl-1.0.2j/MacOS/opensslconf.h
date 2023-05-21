/* MacOS/opensslconf.h */

#if !(defined(VMS) || defined(__VMS))
# if defined(HEADER_CRYPTLIB_H) && !defined(OPENSSLDIR)
#  define OPENSSLDIR "/usr/local/ssl"
# endif
#endif

#if defined(HEADER_IDEA_H) && !defined(IDEA_INT)
# define IDEA_INT unsigned int
#endif

#if defined(HEADER_MD2_H) && !defined(MD2_INT)
# define MD2_INT unsigned int
#endif

#if defined(HEADER_RC2_H) && !defined(RC2_INT)
# define RC2_INT unsigned int
#endif

#if defined(HEADER_RC4_H)
# if !defined(RC4_INT)
#  define RC4_INT unsigned char
# endif
# if !defined(RC4_CHUNK)
#  define RC4_CHUNK unsigned long
# endif
#endif

#if defined(HEADER_DES_H) && !defined(DES_LONG)
# ifndef DES_LONG
#  define DES_LONG unsigned long
# endif
#endif

#if defined(HEADER_BN_H) && !defined(CONFIG_HEADER_BN_H)
# define CONFIG_HEADER_BN_H
# if __option(longlong)
#  define BN_LLONG
# else
#  undef BN_LLONG
# endif
# undef SIXTY_FOUR_BIT_LONG
# undef SIXTY_FOUR_BIT
# define THIRTY_TWO_BIT
# undef SIXTEEN_BIT
# undef EIGHT_BIT
#endif

#if defined(HEADER_RC4_LOCL_H) && !defined(CONFIG_HEADER_RC4_LOCL_H)
# define CONFIG_HEADER_RC4_LOCL_H
# undef RC4_INDEX
#endif

#if defined(HEADER_BF_LOCL_H) && !defined(CONFIG_HEADER_BF_LOCL_H)
# define CONFIG_HEADER_BF_LOCL_H
# define BF_PTR
#endif

#if defined(HEADER_DES_LOCL_H) && !defined(CONFIG_HEADER_DES_LOCL_H)
# define CONFIG_HEADER_DES_LOCL_H
# ifndef DES_PTR
#  define DES_PTR
# endif
# ifndef DES_RISC1
#  define DES_RISC1
# endif
# ifndef DES_RISC2
#  undef DES_RISC2
# endif
# if defined(DES_RISC1) && defined(DES_RISC2)
YOU SHOULD NOT HAVE BOTH DES_RISC1 AND DES_RISC2 DEFINED ! !!!!
# endif
# ifndef DES_UNROLL
#  define DES_UNROLL
# endif
#endif

#ifndef __POWERPC__
# define MD32_XARRAY
#endif
