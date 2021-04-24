#include <stdio.h>
#include <gmp.h>

//функция нахождения обратного числа в кольце
void Revers(mpz_t p, mpz_t x, mpz_t revers)
{
    mpz_t new, old, q, r, h, a;
    int pos = -1;
    mpz_init_set_si(new, 1);
    mpz_init_set_si(old, 0);
    mpz_init_set(q, p);
    mpz_init(a);
    mpz_init(r);
    mpz_init(h);


    if (mpz_cmp_si(x, 0) < 0)
        mpz_add(a, p, x);
    else
        mpz_set(a, x);

    while (mpz_cmp_si(a, 0) > 0)
    {
        mpz_fdiv_r(r, q, a);
        mpz_fdiv_q(q, q, a);
        mpz_mul(h, q, new);
        mpz_add(h, h, old);
        mpz_set(old, new);
        mpz_set(new, h);
        mpz_set(q, a);
        mpz_set(a, r);
        pos *= -1;
    }

    if (pos == 1)
        mpz_set(revers, old);
    else
    {
        mpz_set(revers, p);
        mpz_sub(revers, revers, old);
    }
    mpz_clear(new);
    mpz_clear(old);
    mpz_clear(q);
    mpz_clear(r);
    mpz_clear(h);
    mpz_clear(a);
}

//Скалярная сумма двух точек P1(xP1, yP1) и P2(x2, y2) (возвращает 0, если сумма не является 0 точкой, и 1, если является)
int PointSum(mpz_t p, mpz_t a, mpz_t xP1, mpz_t yP1, mpz_t xP2, mpz_t yP2, mpz_t xR, mpz_t yR)
{
    mpz_t lambda, negy, tmp1, tmp2, resx, resy;
    //если точки являются обратными
    if (mpz_cmp(xP1, xP2) == 0 && mpz_cmp(yP1, negy) == 0)
        return 1;
    
    mpz_init(lambda);
    mpz_init(negy);
    mpz_init(tmp1);
    mpz_init(tmp2);
    mpz_init(resx);
    mpz_init(resy);
    mpz_neg(negy, yP2);
    //если точки не совпадают
    if (mpz_cmp(xP1, xP2) != 0)
    {
        //нахождение угла касательной
        mpz_sub(tmp1, yP2, yP1);
        mpz_sub(tmp2, xP2, xP1);
        Revers(p, tmp2, tmp2);
        mpz_mul(lambda, tmp1, tmp2);
        mpz_mod(lambda, lambda, p);
        //нахождение коорд. х результата
        mpz_mul(tmp1, lambda, lambda);
        mpz_sub(resx, tmp1, xP1);
        mpz_sub(resx, resx, xP2);
        mpz_mod(resx, resx, p);
        //нахождение у результата
        mpz_sub(tmp1, xP1, resx);
        mpz_mul(resy, lambda, tmp1);
        mpz_sub(resy, resy, yP1);
        mpz_mod(resy, resy, p);
    }
    //если точки совпадают
	else if (mpz_cmp(xP1, xP2) == 0 && mpz_cmp(yP1, yP2) == 0)
	{
        //нахождение угла касательной
        mpz_mul(tmp1, xP1, xP1);
        mpz_mul_si(tmp1, tmp1, 3);
        mpz_add(tmp1, tmp1, a);
        mpz_add(tmp2, yP1, yP1);
        Revers(p, tmp2, tmp2);
        mpz_mul(lambda, tmp1, tmp2);
        mpz_mod(lambda, lambda, p);
        //нахождение результирующей х
        mpz_mul(tmp1, lambda, lambda);
        mpz_sub(resx, tmp1, xP1);
        mpz_sub(resx, resx, xP1);
        mpz_mod(resx, resx, p);
        //нахождение результирующей у
        mpz_sub(tmp1, xP1, resx);
        mpz_mul(resy, lambda, tmp1);
        mpz_sub(resy, resy, yP1);
        mpz_mod(resy, resy, p);
	}
    mpz_set(xR, resx);
    mpz_set(yR, resy);
    mpz_clear(lambda);
    mpz_clear(negy);
    mpz_clear(tmp1);
    mpz_clear(tmp2);
    mpz_clear(resx);
    mpz_clear(resy);
    return 0;
}

//скалярное умножение точки P(xP, yP) на число n
int PointMul(mpz_t p, mpz_t a, mpz_t xP, mpz_t yP, mpz_t n, mpz_t xRes, mpz_t yRes)
{
    //Pnew для сложений точек Р в итерации внеш. цикла, Pold для получения конечной суммы
    double d;
    long int size;
    mpz_t x, sum_n;
    mpz_t xPnew, yPnew, xPold, yPold;
    mpz_init_set(x, n);
    mpz_init(sum_n);
    mpz_init(xPnew);
    mpz_init(yPnew);
    mpz_init(xPold);
    mpz_init(yPold);

    //проверка на то, что точка является 0
    int isNull;
    int Poldnull = 1;

    //для увеличения скорости расчёта число x(кол-во сложений) представляется в виде многочлена x=2^(size(x)-1)+2^(size(x-2^(size(x)-1))-1)+... 
    while (mpz_cmp_si(x, 0) > 0)
    {
        isNull = 0;
        mpz_set_si(sum_n, 1);
        size = mpz_sizeinbase(x, 2);
        
        //кол-во сложений точки P в одной итерации
        for (int i = 0; i < size-1; i++)
            mpz_mul_si(sum_n, sum_n, 2);
           

        mpz_set(xPnew, xP);
        mpz_set(yPnew, yP);

        for(int i = 0; i < size-1; i++)
        {
            //если пред. сумма в итерации не 0, то Pnew[i+1]=Pnew[i]+Pnew[i]
            if(isNull == 0)
                isNull = PointSum(p,a,xPnew,yPnew,xPnew,yPnew,xPnew,yPnew);
            //иначе Pnew[i+1]=P, так как P+0=0+P=P
            else
            {
                isNull = 0;
                mpz_set(xPnew, xP);
                mpz_set(yPnew, yP);
            }
        }
        //если результат пред. итерации 0, то Pold=Pnew
        if  (isNull == 0 && Poldnull == 1)
        {
            mpz_set(xPold, xPnew);
            mpz_set(yPold, yPnew);
            Poldnull = 0;
        }
        //если не 0, то Pold+=Pnew
        else if (isNull == 0 && Poldnull == 0)
            Poldnull = PointSum(p,a,xPnew,yPnew,xPold,yPold,xPold,yPold);
        
        //если конечная Pnew в итерации 0, то не обновляется Pold, т.к. Pold+0=Pold 
        
        //разность между х и 2^(size(x)-1)
        mpz_sub(x, x, sum_n);
    }

    //получение результата
    mpz_set(xRes, xPold);
    mpz_set(yRes, yPold);
    
    mpz_clear(x);
    mpz_clear(sum_n);
    mpz_clear(xPnew);
    mpz_clear(yPnew);
    mpz_clear(xPold);
    mpz_clear(yPold);
}
