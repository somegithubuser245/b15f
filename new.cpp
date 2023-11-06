# include <iostream>
# include <b15f/b15f.h>

int main ()
{
    B15F & drv = B15F :: getInstance () ;
    std::cout <<std::hex << drv.digitalRead0 () << std::endl ;
    std::cout <<std::hex << drv.readDipSwitch () << std::endl ;
    drv.digitalWrite0 (0xAA);

}