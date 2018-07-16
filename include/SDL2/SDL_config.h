/* This file is auto-generated by multilib-build.eclass
 * as a multilib-friendly wrapper. For the original content,
 * please see the files that are #included below.
 */

#if defined(__x86_64__) /* amd64 */
#	if defined(__ILP32__) /* x32 ABI */
#		error "abi_x86_x32 not supported by the package."
#	else /* 64-bit ABI */
#		include <x86_64-pc-linux-gnu/SDL2/SDL_config.h>
#	endif
#elif defined(__i386__) /* plain x86 */
#	include <i686-pc-linux-gnu/SDL2/SDL_config.h>
#elif defined(__mips__)
#   if(_MIPS_SIM == _ABIN32) /* n32 */
#       error "abi_mips_n32 not supported by the package."
#   elif(_MIPS_SIM == _ABI64) /* n64 */
#       error "abi_mips_n64 not supported by the package."
#   elif(_MIPS_SIM == _ABIO32) /* o32 */
#       error "abi_mips_o32 not supported by the package."
#   endif
#elif defined(__sparc__)
#	if defined(__arch64__)
#       error "abi_sparc_64 not supported by the package."
#	else
#       error "abi_sparc_32 not supported by the package."
#	endif
#elif defined(__s390__)
#	if defined(__s390x__)
#       error "abi_s390_64 not supported by the package."
#	else
#       error "abi_s390_32 not supported by the package."
#	endif
#elif defined(__powerpc__) || defined(__ppc__)
#	if defined(__powerpc64__) || defined(__ppc64__)
#       error "abi_ppc_64 not supported by the package."
#	else
#       error "abi_ppc_32 not supported by the package."
#	endif
#elif defined(SWIG) /* https://sourceforge.net/p/swig/bugs/799/ */
#	include <x86_64-pc-linux-gnu/SDL2/SDL_config.h>
#elif defined(_WIN64) 
#include <SDL2/WIN/SDL_config.h>

#else
#	error "No ABI matched, please report a bug to bugs.gentoo.org"
#endif
