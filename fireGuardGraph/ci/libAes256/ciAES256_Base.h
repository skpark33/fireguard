/*--------------------------------------------------
 *
 *   This file is in important parts based on the C implementation
 *   of AES-256 by Ilya O. Levin and Hal Finney, to be downloaded
 *   from <http://www.literatecode.com/aes256>. It contains the fol-
 *   lowing copyright notice:
 *
 *  --------------------------------------------------------------------
 *   Byte-oriented AES-256 implementation.
 *   All lookup tables replaced with 'on the fly' calculations.
 *
 *   Copyright (c) 2007-2009 Ilya O. Levin, http://www.literatecode.com
 *   Other contributors: Hal Finney
 *
 *   Permission to use, copy, modify, and distribute this software for any
 *   purpose with or without fee is hereby granted, provided that the above
 *   copyright notice and this permission notice appear in all copies.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *   ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *  --------------------------------------------------------------------
 *
 *  The original code also has been modified (beside the conversion
 *  from C to C++) in several places. Since it's derived work it
 *  is distributed under the same conditions as those chosen by the
 *  original authors.
 *
 *  Author: Jens Thoms Toerring <jt@toerring.de>
 *
 *--------------------------------------------------*/


#pragma once
#if ! defined ciAES256_Base_hpp_
#define ciAES256_Base_hpp_


#include "ci/libAes256/ciByte_Block.h"
#include "ci/libAes256/ciS_Box.h"
#include <string>


/*---------------------------------------------*
 * Class implementing the basic AES-256 encryption
 * and decryption methods on 16-byte wide buffers
 * (the 'BO' stands for "byte oriented"). This
 * class is used in all others for doing the low
 * level work.
 *---------------------------------------------*/

class ciAES256_Base
{
	typedef  unsigned char  byte;

  public :

    // Constructor with key given as a std::string

    ciAES256_Base( std::string const & key );


    // Constructor with key given as 32-byte block

    ciAES256_Base( ciByte_Block< 32 > const & key );


    // Change the key

    void
    set_key( std::string const & key );

    // Returns the key

    ciByte_Block< 32 > const &
    get_key( ) const;


    // Encrypts a block of 16 bytes (in place or returning new block)

    ciByte_Block< 16 > &
    encrypt( ciByte_Block< 16 > & buf );


    ciByte_Block< 16 >
    encrypt( ciByte_Block< 16 > const & buf );


    // Decrypts a block of 16 bytes (in place or returning new block)

    ciByte_Block< 16 > &
    decrypt( ciByte_Block< 16 > & buf );

    ciByte_Block< 16 >
    decrypt( ciByte_Block< 16 > const & buf );


  private :

    // Methods doing all the low level work

    void
    expand_enc_key( ciByte_Block< 32 > & key,
                    byte             & rcon );

    void
    expand_dec_key( ciByte_Block< 32 > & key,
                    byte             & rcon );

    void
    sub_bytes( ciByte_Block< 16 > & buf );

    void
    inv_sub_bytes( ciByte_Block< 16 > & buf );

    void
    add_round_key( ciByte_Block< 16 >       & buf,
                   ciByte_Block< 16 > const & key );

    void
    shift_rows( ciByte_Block< 16 > & buf );

    void
    inv_shift_rows( ciByte_Block< 16 > &  buf );

    void
    mix_columns( ciByte_Block< 16 > & buf );

    void
    inv_mix_columns( ciByte_Block< 16 > & buf );

    byte
    rj_xtime( byte x );

    // Initialization method, sets up the keys

	void
	init( ciByte_Block< 32 > const & key );


    ciByte_Block< 32 > m_enc_key; 
    ciByte_Block< 32 > m_dec_key;

    // Rijndael S-box and its inverse

    static ciS_Box< false > const s_sbox;
    static ciS_Box< true  > const s_inv_sbox;

    static bool s_rcon[ ];
};


#endif


/*
 * Local variables:
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
