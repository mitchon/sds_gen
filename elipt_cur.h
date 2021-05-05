#ifndef ELIPT_CUR_H
#define ELIPT_CUR_H

int PointMul(mpz_t p, mpz_t a, mpz_t xP, mpz_t yP, mpz_t n, mpz_t xR, mpz_t yR);
int PointSum(mpz_t p, mpz_t a, mpz_t xP1, mpz_t yP1, mpz_t xP2, mpz_t yP2, mpz_t xR, mpz_t yR);
void Revers(mpz_t p, mpz_t x, mpz_t revers);

#endif