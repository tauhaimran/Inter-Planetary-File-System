#pragma once
#pragma once

// headers to include....
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>


///.................................................................................................................
///..................................SHA1x class definition w/ function signatures..................................
///.................................................................................................................


class SHA1x
{


private:

    //using macros manipulate memory correctly
    typedef unsigned long int int32;   /* just needs to be at least 32bit */
    typedef unsigned long long int64;  /* just needs to be at least 64bit */

    //contents...
    static const unsigned int DIGEST_h = 5;  /*sha1_h[5] integers per SHA1 digest  */
    static const unsigned int BLOCK_INTS = 16;  /* number of 32bit integers per SHA1 block */
    static const unsigned int BLOCK_BYTES = BLOCK_INTS * 4;

    //makes 32 bit integers
    int32 digest[DIGEST_h];
    std::string buffer; //our output string
    int64 numTansforms;//encryptions stored in here

    void reset();
    void sha1x_function(int32 block[BLOCK_BYTES]);

    static void buffer_to_block(const std::string& buffer, int32 block[BLOCK_BYTES]);
    static void readXtract(std::istream& is, std::string& s, int max);

public:
    SHA1x();//empty constructor
    void sha1x_update(const std::string& s);
    void sha1x_update(std::istream& is);
    std::string sha1x_final();
    static std::string from_file(const std::string& filename);

};

//-------the function to call---------
std::string sha1x(const std::string& string);
//------------------------------------


///.................................................................................................................
///..........................SHA1x helping functions for ops & logical arithematic..................................
///.................................................................................................................


/* macros helper functions to get stuff working */

//rotate bitwise function
#define sha1x_ops_rotateWord(value, bits) ( ((value) << (bits)) | (((value) & 0xffffffff) >> (32 - (bits))) )


#define sha1x_block(i) (block[i&15] = sha1x_ops_rotateWord(block[(i+13)&15] ^ block[(i+8)&15] ^ block[(i+2)&15] ^ block[i&15],1))

//max value returner
#define MAX(i1, i2) (i1 > i2 ? i1 : i2)

//min value returner
#define MIN(i1, i2) (i1 < i2 ? i1 : i2)

//the different operations used in SHA1...

//operation0 - a constant operation 
// t>=16 ::  W[s] = sha1_rotateWord(W[(s + 13) & 0xf] ^ W[(s + 8) & 0xf] ^ W[(s + 2) & 0xf] ^ W[s], 1);
#define sha1x_operation0(v,w,x,y,z,i) z += ((w&(x^y))^y)     + block[i]    + 0x5a827999 + sha1x_ops_rotateWord(v,5); w=sha1x_ops_rotateWord(w,30) ;

//operation1 - 0>= t<20 :: W[s]  = sha1_k[0] + ((B & C) | (~B & D));
#define sha1x_operation1(v,w,x,y,z,i) z += ((w&(x^y))^y)     + sha1x_block(i) + 0x5a827999 + sha1x_ops_rotateWord(v,5); w=sha1x_ops_rotateWord(w,30);

//operation2 - t<40 :: W[s] = sha1_k[1] + (B ^ C ^ D);
#define sha1x_operation2(v,w,x,y,z,i) z += (w^x^y)           + sha1x_block(i) + 0x6ed9eba1 + sha1x_ops_rotateWord(v,5); w=sha1x_ops_rotateWord(w,30);

//operation3 - t<60 :: W[s] =  sha1_k[2] + ((B & C) | (B & D) | (C & D));
#define sha1x_operation3(v,w,x,y,z,i) z += (((w|x)&y)|(w&x)) + sha1x_block(i) + 0x8f1bbcdc + sha1x_ops_rotateWord(v,5); w=sha1x_ops_rotateWord(w,30);

//operation4 - t<80 :: W[s] = sha1_k[3] + (B ^ C ^ D);
#define sha1x_operation4(v,w,x,y,z,i) z += (w^x^y)           + sha1x_block(i) + 0xca62c1d6 + sha1x_ops_rotateWord(v,5); w=sha1x_ops_rotateWord(w,30);



///.......................................................................................................................
///...........................................SHA1x class functions code&mechanics........................................
///.......................................................................................................................


// - constructor for class resets constant values
SHA1x::SHA1x()
{
    reset();
}


// function to turn string into istringstream
void SHA1x::sha1x_update(const std::string& s)
{
    std::istringstream is(s);
    sha1x_update(is);
}


// function to update values as istringstream
void SHA1x::sha1x_update(std::istream& in)
{
    std::string rest_of_buffer;
    readXtract(in, rest_of_buffer, BLOCK_BYTES - buffer.size());
    buffer += rest_of_buffer;

    while (in)
    {
        int32 block[BLOCK_INTS]; //makes a block of 32bit integer
        buffer_to_block(buffer, block);
        sha1x_function(block);
        readXtract(in, buffer, BLOCK_BYTES);
    }
}

//Adding padding 


std::string SHA1x::sha1x_final()
{
    /* Total number of hashed bits */
    int64 total_bits = (numTansforms * BLOCK_BYTES + buffer.size()) * 8;

    /* Padding */
    buffer += 0x80;
    unsigned int orig_size = buffer.size();
    while (buffer.size() < BLOCK_BYTES)
    {
        buffer += (char)0x00;
    }

    int32 block[BLOCK_INTS];
    buffer_to_block(buffer, block);

    if (orig_size > BLOCK_BYTES - 8)
    {
        sha1x_function(block);
        for (unsigned int i = 0; i < BLOCK_INTS - 2; i++)
        {
            block[i] = 0;
        }
    }

    /* Append total_bits, split this int64 into two int32 */
    block[BLOCK_INTS - 1] = total_bits;
    block[BLOCK_INTS - 2] = (total_bits >> 32);
    sha1x_function(block);

    /* Hex std::string */
    std::ostringstream result;
    for (unsigned int i = 0; i < DIGEST_h; i++)
    {
        result << std::hex << std::setfill('0') << std::setw(8);
        result << (digest[i] & 0xffffffff);
    }

    /* Reset for next run */
    reset();

    return result.str();
}


std::string SHA1x::from_file(const std::string& filename)
{
    std::ifstream stream(filename.c_str(), std::ios::binary);
    SHA1x checksum;
    checksum.sha1x_update(stream);
    return checksum.sha1x_final();
}


void SHA1x::reset()
{
    /* SHA1x initialization constants */
    digest[0] = 0x67452301;
    digest[1] = 0xefcdab89;
    digest[2] = 0x98badcfe;
    digest[3] = 0x10325476;
    digest[4] = 0xc3d2e1f0;

    /* Reset counters */
    numTansforms = 0;
    buffer = "";
}


/*
 * Hash a single 512-bit block. This is the core of the algorithm.
 */

void SHA1x::sha1x_function(int32 block[BLOCK_BYTES])
{
    //initialize function variables
    int32 a = digest[0]; //unsigned int A = h[0];
    int32 b = digest[1]; //unsigned int B = h[1];
    int32 c = digest[2]; //unsigned int C = h[2];
    int32 d = digest[3]; //unsigned int D = h[3];
    int32 e = digest[4]; //unsigned int E = h[4];


    // 4 rounds of 20 operations each. - hardcoded to avoid worng answers
    // looped answers !!!!
    // I GENUINELY TRIED OKAY!

    //-----------------sha1_f rounds---------------
    int rounds = 0;
    //...................................................................................
       //if (t < 16) :: sha1x_operation0
    while (rounds < 16) {
        sha1x_operation0(a, b, c, d, e, rounds);
        int32 A = e, B = a, C = b, D = c, E = d;
        a = A;
        b = B;
        c = C;
        d = D;
        e = E;
        rounds++;
    }
    //..........................................................................................................
        //if ( 16 >= t <20 20) :: sha1x_operation1
    while (rounds < 20) {
        sha1x_operation1(a, b, c, d, e, rounds);
        int32 A = e, B = a, C = b, D = c, E = d;
        a = A;
        b = B;
        c = C;
        d = D;
        e = E;
        rounds++;
    }
    //..........................................................................................................
        //if (t < 40) :: sha1x_operation1
    while (rounds < 40) {
        sha1x_operation2(a, b, c, d, e, rounds);
        int32 A = e, B = a, C = b, D = c, E = d;
        a = A;
        b = B;
        c = C;
        d = D;
        e = E;
        rounds++;
    }
    //..........................................................................................................
        //if ( 40>= t < 60) :: sha1x_operation3
    while (rounds < 60) {
        sha1x_operation3(a, b, c, d, e, rounds);
        int32 A = e, B = a, C = b, D = c, E = d;
        a = A;
        b = B;
        c = C;
        d = D;
        e = E;
        rounds++;
    }
    //..........................................................................................................
        //if ( 60 > = t < 80) :: sha1x_operation4
    while (rounds < 80) {
        sha1x_operation4(a, b, c, d, e, rounds);
        int32 A = e, B = a, C = b, D = c, E = d;
        a = A;
        b = B;
        c = C;
        d = D;
        e = E;
        rounds++;
    }
    //...............................................
        // variables back intot he digest constants
    digest[0] += a;
    digest[1] += b;
    digest[2] += c;
    digest[3] += d;
    digest[4] += e;

    //counting number of transformantions
    numTansforms++;
}


void SHA1x::buffer_to_block(const std::string& buffer, int32 block[BLOCK_BYTES])
{
    //turning std::string into int32 array (MSB) */
    for (unsigned int i = 0; i < BLOCK_INTS; i++)
    {
        block[i] = (buffer[4 * i + 3] & 0xff)
            | (buffer[4 * i + 2] & 0xff) << 8
            | (buffer[4 * i + 1] & 0xff) << 16
            | (buffer[4 * i + 0] & 0xff) << 24;
    }
}


// reading function....
void SHA1x::readXtract(std::istream& is, std::string& s, int max)
{
    char* sbuf = new char[max]; //makng the string buffer to extrac and copy values
    is.read(sbuf, max); //read -  extracts 'n' number of characters from the stream and stores it into the mentioned directory
    s.assign(sbuf, is.gcount());  //assign copies values extracted into sbuf
    //gcount - get character count...


}



///.................................................................................................................
///................................sha1x major call function! sha1x("stuff to encrypt").............................
///.................................................................................................................

std::string sha1x(const std::string& string)
{
    SHA1x checksum;
    checksum.sha1x_update(string);
    return checksum.sha1x_final();
}

///.................................................................................................................
///....................................................END OF FILE..................................................
///.................................................................................................................