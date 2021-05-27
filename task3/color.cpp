#include "color.hpp"

HSL RGB2HSL(const RGB& rgb)
{
    // #define debug(x) {fprintf(stderr, "return value ("#x") is: %lf\n", x); fflush(0);}

    const float _EPS = static_cast<float>(1e-3);

    double  fr = rgb.r / 255.f,
            fg = rgb.g / 255.f,
            fb = rgb.b / 255.f;
    double  max = std::max(std::max(rgb.r, rgb.g), rgb.b) / 255.f,
            min = std::min(std::min(rgb.r, rgb.g), rgb.b) / 255.f,
            dm = max - min,
            sm = max + min;

    // debug(max)
    // debug(min)

    double  h = 0,
            s = (fabs(dm) < _EPS) ? 0 : dm / (1.f - fabs(1.f - sm)),
            l = sm / 2.f;
    
    // debug(s)
    // debug(l)
    
    if(fabs(dm) > _EPS)
    {
        double  dr = fr / dm,
                dg = fg / dm,
                db = fb / dm;

        if(fabs(fr - max) < _EPS)   h = dg - db;
        if(fabs(fg - max) < _EPS)   h = 2 + db - dr;
        if(fabs(fb - max) < _EPS)   h = 4 + dr - dg;
    }
    h = (h < 0) ? h * 60 + 360 : h * 60;
    // debug(h/360.f)
    // debug(s)
    // debug(l)
    

    return HSL( static_cast<float>(h), 
                static_cast<float>(s * 100.f),
                static_cast<float>(l * 100.f));

}

RGB HSL2RGB(const HSL& hsl)
{
    const float _EPS = static_cast<float>(1e-3);
    
    if (fabs(hsl.s) < _EPS) return RGB (0, 0, 0);

    struct CALC
    {
        static double calc(double component, double Q, double P)
        {
            component += (component < 0) ? 1.f : 0.f;
            component -= (component > 1) ? 1.f : 0.f;
                                   
            if(6*component < 1)                     return P + ((Q - P) * 6.f * component);
            if(1 <= 6*component && 6*component < 3) return Q;
            if(3 <= 6*component && 6*component < 4) return P + ((Q - P) * ( 2.f / 3.f - component) * 6.f);
            return P;
        }
    };

    double  H = hsl.h / 360.f,
            S = hsl.s / 100.f,
            L = hsl.l / 100.f;


    double  Q = (L < 0.5f) ?  L * (1.f + S) : L + S - (L * S),
            P = 2.f * L - Q,
            Hk = H,
            Tr = Hk + 1.f / 3.f,
            Tg = Hk,
            Tb = Hk - 1.f / 3.f;
    // #define debug(x) {fprintf(stderr, "return value ("#x") is: %lf\n", x); fflush(0);}
    //fprintf(stderr, "Tr, Tg, Tb, Hk, P, Q : %lf, %lf, %lf, %lf, %lf, %lf\n", Tr, Tg, Tb, Hk, P, Q);
    
    //BYTE R = static_cast<BYTE>(std::round(255 * CALC::calc(Tr, Q, P)));
    //BYTE G = static_cast<BYTE>(std::round(255 * CALC::calc(Tg, Q, P)));
    //BYTE B = static_cast<BYTE>(std::round(255 * CALC::calc(Tb, Q, P))); 
    // debug(std::round(255 * CALC::calc(Tr, Q, P)))
    // debug(std::round(255 * CALC::calc(Tg, Q, P)))
    // debug(std::round(255 * CALC::calc(Tb, Q, P)))

    return RGB( static_cast<BYTE>(std::round(255 * CALC::calc(Tr, Q, P))),
                static_cast<BYTE>(std::round(255 * CALC::calc(Tg, Q, P))),
                static_cast<BYTE>(std::round(255 * CALC::calc(Tb, Q, P))));
    //return RGB( R, G, B);
}