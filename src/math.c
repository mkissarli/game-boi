/*////////////////////////////////////////////////////////////////////////////*/

/* 12.4 Fixed-point type imeplementation with basic mathematical operations.  */
/* We are using this represenation as we do not need high-precision with our  */
/* real numbers and would still like to represent large integral values.      */

#define FP_SHIFT (4            )
#define FP_SCALE (1 << FP_SHIFT)
#define FP_MASK  (FP_SCALE -  1)
#define FP_ONE   (FP_SCALE     )
#define FP_HALF  (FP_SCALE >> 1)

#define M_FP_2PI  ITOF(0xFF            )
#define M_FP_PI   ITOF(0x7F            )
#define M_FP_PI4  FDIV(M_FP_PI,ITOF(4) )
#define M_FP_3PI4 FMUL(ITOF(3),M_FP_PI4)

typedef INT16 FIXED;

#define FADD(a,b) ( (a)             + (b) )       /* Add two fixed-points.    */
#define FSUB(a,b) ( (a)             - (b) )       /* Sub two fixed-points.    */
#define FMUL(a,b) (((a) * (b)) >> FP_SHIFT)       /* Mul two fixed-points.    */
#define FDIV(a,b) (((a) * FP_SCALE) / (b) )       /* Div two fixed-points.    */

#define UTOF(x) ((x) << FP_SHIFT)                 /* UINT  to FIXED.          */
#define ITOF(x) ((x) << FP_SHIFT)                 /* SINT  to FIXED.          */
#define FTOU(x) ((x) /  FP_SCALE)                 /* FIXED to UINT (truncate).*/
#define FTOI(x) ((x) >> FP_SHIFT)                 /* FIXED to SINT (truncate).*/

#define FROUNDU(x) FTOU(x + FP_HALF)              /* Round FIXED to UINT.     */
#define FROUNDI(x) FTOI(x + FP_HALF)              /* Round FIXED to SINT.     */

#define SIN(x) ((FIXED)(SIN_TABLE[(((x))       ) & 0xFF])) /* Sine.           */
#define COS(x) ((FIXED)(SIN_TABLE[(((x)) + 0x40) & 0xFF])) /* Cosine.         */

static FIXED ATAN2 (FIXED y, FIXED x);                   /* Arc tangent.    */
static FIXED SQRT  (FIXED n         );                   /* Square root.    */

/* Extract the integral and fractional parts of a fixed-point value and then  */
/* convert them into integers; useful for printing out fixed-points, like so: */
/*                                                                            */
/*   printf("%d.%d", FINT(fixed), FFRAC(fixed,10));                           */
/*                                                                            */
/* The argument p to FFRAC is the level of precision to print with. Passing a */
/* value of 1 will result in one digit, 10 for two, 100 for three, and so on. */
#define FINT(  x)     ((x) >> FP_SHIFT)
#define FFRAC( x,p) ((((x) &  FP_MASK ) * p) / FP_SCALE)

#define FPRINT(x,p) FINT(x), FFRAC(x,p)

/* Table of fixed-point sin values for implementing sin() and cos() functions.*/
/* The table goes from the range [0x00 - 0xFF] with 0xFF representing 2pi.    */
/*
static const UINT16 SIN_TABLE[] =
{
0x0000,0x0000,0x0000,0x0001,0x0001,0x0001,0x0002,0x0002,
0x0003,0x0003,0x0003,0x0004,0x0004,0x0005,0x0005,0x0005,
0x0006,0x0006,0x0006,0x0007,0x0007,0x0007,0x0008,0x0008,
0x0008,0x0009,0x0009,0x0009,0x000A,0x000A,0x000A,0x000B,
0x000B,0x000B,0x000B,0x000C,0x000C,0x000C,0x000C,0x000D,
0x000D,0x000D,0x000D,0x000D,0x000E,0x000E,0x000E,0x000E,
0x000E,0x000E,0x000F,0x000F,0x000F,0x000F,0x000F,0x000F,
0x000F,0x000F,0x000F,0x000F,0x000F,0x000F,0x000F,0x000F,
0x000F,0x000F,0x000F,0x000F,0x000F,0x000F,0x000F,0x000F,
0x000F,0x000F,0x000F,0x000F,0x000F,0x000F,0x000F,0x000E,
0x000E,0x000E,0x000E,0x000E,0x000E,0x000D,0x000D,0x000D,
0x000D,0x000D,0x000C,0x000C,0x000C,0x000C,0x000B,0x000B,
0x000B,0x000B,0x000A,0x000A,0x000A,0x0009,0x0009,0x0009,
0x0008,0x0008,0x0008,0x0007,0x0007,0x0007,0x0006,0x0006,
0x0006,0x0005,0x0005,0x0005,0x0004,0x0004,0x0003,0x0003,
0x0003,0x0002,0x0002,0x0001,0x0001,0x0001,0x0000,0x0000,
0x0000,0x0000,0x0000,0xFFFF,0xFFFF,0xFFFF,0xFFFE,0xFFFE,
0xFFFD,0xFFFD,0xFFFD,0xFFFC,0xFFFC,0xFFFB,0xFFFB,0xFFFB,
0xFFFA,0xFFFA,0xFFFA,0xFFF9,0xFFF9,0xFFF9,0xFFF8,0xFFF8,
0xFFF8,0xFFF7,0xFFF7,0xFFF7,0xFFF6,0xFFF6,0xFFF6,0xFFF5,
0xFFF5,0xFFF5,0xFFF5,0xFFF4,0xFFF4,0xFFF4,0xFFF4,0xFFF3,
0xFFF3,0xFFF3,0xFFF3,0xFFF3,0xFFF2,0xFFF2,0xFFF2,0xFFF2,
0xFFF2,0xFFF2,0xFFF1,0xFFF1,0xFFF1,0xFFF1,0xFFF1,0xFFF1,
0xFFF1,0xFFF1,0xFFF1,0xFFF1,0xFFF1,0xFFF1,0xFFF1,0xFFF1,
0xFFF1,0xFFF1,0xFFF1,0xFFF1,0xFFF1,0xFFF1,0xFFF1,0xFFF1,
0xFFF1,0xFFF1,0xFFF1,0xFFF1,0xFFF1,0xFFF1,0xFFF1,0xFFF2,
0xFFF2,0xFFF2,0xFFF2,0xFFF2,0xFFF2,0xFFF3,0xFFF3,0xFFF3,
0xFFF3,0xFFF3,0xFFF4,0xFFF4,0xFFF4,0xFFF4,0xFFF5,0xFFF5,
0xFFF5,0xFFF5,0xFFF6,0xFFF6,0xFFF6,0xFFF7,0xFFF7,0xFFF7,
0xFFF8,0xFFF8,0xFFF8,0xFFF9,0xFFF9,0xFFF9,0xFFFA,0xFFFA,
0xFFFA,0xFFFB,0xFFFB,0xFFFB,0xFFFC,0xFFFC,0xFFFD,0xFFFD,
0xFFFD,0xFFFE,0xFFFE,0xFFFF,0xFFFF,0xFFFF,0x0000,0x0000
};*/

/* The implementations for a number of different fixed-point math operations. */
/*
static FIXED ATAN2 (FIXED y, FIXED x)
{
    FIXED coeff_1,coeff_2, abs_y, r, angle;

    coeff_1 = M_FP_PI4;
    coeff_2 = M_FP_3PI4;

    abs_y = y;

         if (abs_y <  0) { abs_y = -abs_y; }
    else if (abs_y == 0) { abs_y = 1;      }

    if (x >= 0) {
        r = FDIV(x-abs_y, x+abs_y);
        angle = coeff_1 - FMUL(coeff_1,r);
    } else {
        r = FDIV(x+abs_y, abs_y-x);
        angle = coeff_2 - FMUL(coeff_1,r);
    }

    return ((y < 0) ? (-angle) : (angle));
}

static FIXED SQRT (FIXED n)
{
    FIXED s;
    UINT8 i;
    s = (n + 0xFF) >> 1;
    for (i=0; i<8; ++i) {
        s = (s + FDIV(n,s)) >> 1;
    }
    return s;
}*/

/*////////////////////////////////////////////////////////////////////////////*/
