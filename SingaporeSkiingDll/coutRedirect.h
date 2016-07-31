#include "stdafx.h"
#include <iostream>

using namespace std;
class cout_redirect 
{
public:
    cout_redirect(streambuf *new_buffer);
    ~cout_redirect( );
private:
    streambuf *old;
};