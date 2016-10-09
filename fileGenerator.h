#ifndef FILEGENERATOR_H_   
#define FILEGENERATOR_H_

// Necesario para leer el archivo avs
# define SWAP(x) ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )
# define FIX(x) (*(unsigned *)&(x) = \
         SWAP(*(unsigned *)&(x)))
         
#endif