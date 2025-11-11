.global first_char 
.global n_char

first_char:

    ##load byte (load = tirar da memória e colocar no registrador)
    lb a0, 2(a0)
    ret ##tipo um return

n_char:
    add a0, a0, a1
    lb a0, 0(a0)
    ret

