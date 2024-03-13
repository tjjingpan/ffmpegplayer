#include <iostream>
#define __STDC_CONSTANT_MACROS
extern "C"
{
#include "libavcodec/avcodec.h "
}
int main()
{
	printf("%s", avcodec_configuration());
}