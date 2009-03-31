/* $Id$ */
#ifndef __RO_STRINGS_H
#define __RO_STRINGS_H

#include "roint_import.h"

#include <string>

namespace RO {
	/**
	 * Converts an string from EUC_KR encoding to UTF-8
	 */
	ROINT_DLLAPI std::string euc2utf8(const std::string& euc);

	/**
	 * Converts an stream from EUC_KR into a UTF-8 encoded string
	 */
	ROINT_DLLAPI std::string euc2utf8(std::istream& is);

	/**
	 * Converts an stream from EUC_KR into a UTF-8 stream
	 */
	ROINT_DLLAPI void euc2utf8(std::istream&, std::ostream&);

	/**
	 * Converts an string from UTF-8 encoding to EUC_KR
	 */
	ROINT_DLLAPI std::string utf82euc(const std::string& utf);
}

#endif /* __RO_STRINGS_H */

