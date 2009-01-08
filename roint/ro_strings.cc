/* $Id$ */
#include "stdafx.h"

#include "ro_strings.h"

#define CP949_COUNT 17048

#include <istream>
#include <ostream>

namespace RO {
	const unsigned short cp949_start = 0x8141;
	const unsigned short cp949[][2] = {
		#include "cp949.h"
	};

	unsigned short cp949_conv(const unsigned short& x) {
	  unsigned int i = 0;
	  for (i = 0; i < CP949_COUNT; i++)
		if (cp949[i][1] == x)
		  return(cp949[i][0]);

	  return(0);
	}

	unsigned short cp949_convr(const unsigned short& x) {
	  unsigned int i = 0;
	  for (i = 0; i < CP949_COUNT; i++)
		if (cp949[i][0] == x)
		  return(cp949[i][1]);

	  return(0);
	}

	std::string euc2utf8(const std::string& euc) {
	  std::string utf;
	  unsigned char c_utf[5];
	  unsigned int i;
	  unsigned short s_euc;
	  unsigned short s_utf;

	  unsigned char c;

	  for (i = 0; i < euc.length(); i++) {
		c = (unsigned char)euc[i];
		if ((0x80 & c) > 0) {
		  s_euc = 0;
		  s_euc = c;
		  s_euc <<= 8;
		  i++;
		  c = (unsigned char)euc[i];
		  s_euc += c;
		  if (s_euc < cp949_start) {
			printf("Invalid value 0x%04x\n", s_euc);
			continue;
		  }
		  s_utf = cp949_conv(s_euc);
		  // printf("0x%04x -> 0x%04x\n", s_euc, s_utf);
		  // printf("0x%04x ", s_utf & 0xf000); printf("0x%04x ", s_utf & 0x0fc0); printf("0x%04x ", s_utf & 0x003f); printf("\n");

		  c_utf[0] = (s_utf & 0xf000) >> 12;
		  c_utf[1] = (s_utf & 0x0fc0) >> 6;
		  c_utf[2] = (s_utf & 0x003f);

		  // printf("0x%04x ", c_utf[0]); printf("0x%04x ", c_utf[1]); printf("0x%04x ", c_utf[2]); printf("\n");

		  c_utf[0] |= 0xe0;
		  c_utf[1] |= 0x80;
		  c_utf[2] |= 0x80;
		  c_utf[3] = 0;

		  // printf("0x%04x ", c_utf[0]); printf("0x%04x ", c_utf[1]); printf("0x%04x ", c_utf[2]); printf("\n\n");

		  utf = utf + (char*)c_utf;
		}
		else {
		  utf = utf + euc[i];
		}
	  }

	  return(utf);
	}

	std::string euc2utf8(std::istream& is) {
	  std::string utf;
	  char euc[3];
	  euc[0] = euc[1] = euc[2] = 0;

	  while (!is.eof()) {
		is.read(euc, 1);
		if (euc[0] & 0x80) {
		  is.read(euc + 1, 1);
		  utf = utf + euc2utf8(euc);
		}
		utf = utf + euc[0];
	  }

	  return(utf);
	}

	void euc2utf8(std::istream& i, std::ostream& o) {
		std::string utf;
		char buf[1024];
		while (!i.eof()) {
			i.getline(buf, 1024);
			utf = euc2utf8(buf);
			o << utf << std::endl;
		}
	}

	std::string utf82euc(const std::string& utf) {
	  std::string euc;
	  unsigned char c_utf[5];
	  char *c_euc;
	  unsigned int i;
	  unsigned short s_euc;
	  unsigned short s_utf;

	  c_euc = (char*)&s_euc;

	  unsigned char c;

	  for (i = 0; i < utf.length(); i++) {
		c = (unsigned char)utf[i];
		if ((0x80 & c) > 0) {
			s_utf = 0;
			if ((0xe0 & c) == 0xe0) {
				// 3-byte encoded. Starts with 1110xxxx = 0xe0
				c_utf[0] = c;
				i++; c = (unsigned char)utf[i];
				c_utf[1] = c;
				i++; c = (unsigned char)utf[i];
				c_utf[2] = c;
				c_utf[3] = 0;

				s_utf  = (c_utf[0] & 0x0f) << 12;
				s_utf |= (c_utf[1] & 0x3f) << 6;
				s_utf |= (c_utf[2] & 0x3f);
			}
		  s_euc = cp949_convr(s_utf);
		  if (s_euc == 0) {
			  // Not in the codepage... Copy as is
			  euc += (char*)c_utf;
		  }
		  else {
			  euc += c_euc[1];
			  euc += c_euc[0];
		  }
		}
		else {
		  euc = euc + utf[i];
		}
	  }

	  return(euc);
	}
}

