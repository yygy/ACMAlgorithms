
/*
* @brief given a and b, solve a*ka+b*kb = gcd(a,b)
* @param a
* @param b
* @return gcd(a,b)
*/
lld ExtGcd(lld a, lld b, lld& ka, lld& kb)
{
    if (a % b == 0)
    {
        ka = 1;
        kb = 1 - a / b;
        return b;
    }

    const lld c = a % b;

    lld kbb = 0, kc = 0;
    const lld g = ExtGcd(b, c, kbb, kc);

    lld kab = a / b;
    //a= kab * b + c
    //c=a-kab*b
    //kbb*b +kc * c = g
    //kbb*b+kc*(a-kab*b)
    //=kbb*b+kc*a-kc*kab*b
    //=kc*a + (kbb-kc*kab)*b = g
    ka = kc;
    kb = kbb - kc * kab;

    return g;
}
