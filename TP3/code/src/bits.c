#include <util.h>
#include <datamgt.h>

//mette un 1 au bit d index nbit dans bytes et retourne l ancienne valeur
int bits_set(unsigned char *bytes, unsigned int nbit)
{
    int r = !!(bytes[nbit >> 3] & (1 << (nbit & 7))); // lecture de l ancienne valeur
	bytes[nbit >> 3] |= (1 << (nbit & 7)); // mettre un 1 au bit d index nbit
	return r; //retourner l ancienne valeur
}
//mette un 0 au bit d index nbit dans bytes et retourne l ancienne valeur
int bits_clear(unsigned char *bytes, unsigned int nbit){
    int r = !!(bytes[nbit >> 3] & (1 << (nbit & 7)));
	bytes[nbit >> 3] &= ~(1 << (nbit & 7));
	return r;
};

//demande si le bit est a 1 a l index nbit
int bits_isSet(unsigned char *bytes, unsigned int nbit) {
    return !!(bytes[nbit >> 3] & (1 << (nbit & 7)));
};

