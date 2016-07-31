#include "stdafx.h"
#include <iostream>
#include "coutRedirect.h"

using namespace std;
cout_redirect::cout_redirect(streambuf *new_buffer) : old(cout.rdbuf(new_buffer))
{ 
}

cout_redirect::~cout_redirect( ) 
{
    cout.rdbuf(old);
}
