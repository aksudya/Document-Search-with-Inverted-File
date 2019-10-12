#ifndef CONVERT_H
#define CONVERT_H


class Convert
{
    public:
        Convert();
        virtual ~Convert();
    static void bow2nbow(const char *srcFn, const char *dstFn, const char *wghFn);
    static void test();
};

#endif // CONVERT_H
