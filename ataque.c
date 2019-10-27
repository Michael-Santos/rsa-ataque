/*
* @Author: Michael dos Santos
*/

#include<stdio.h>
#include<stdlib.h>

#define NUMBER_NUM_BITS 64

typedef long long int NUMBER;

/*
 * Obtém o valor de um bit específico
 * 
 * @params value - Número em que se deseja encontrar o bit
 * @params index - Índice do bit (bit zero é o bit mais à esquerda)
 * @returns Value of the bit
*/
int get_bit_by_index(NUMBER value, int index) {
    NUMBER left_shift = value << index;
    return left_shift >> NUMBER_NUM_BITS - 1;
}

/*
 * Encontra o bit mais a esquerda que possui valor 1
 * 
 * @params value - Número para encontrar o bit mais à esquerda com valor 1 
 * @returns Índice do bit
*/
int find_leftmost_one_bit(NUMBER value) {
    int bit = 0;
    
    for(int i = 0; i < NUMBER_NUM_BITS; i++) {
        bit = get_bit_by_index(value, i);
        // Workaround, pois apenas recebo -1
        if (bit == -1) return i;
    }   
    return(-1);
}


NUMBER power_module(NUMBER base, NUMBER power, NUMBER n) {
    NUMBER num_multiply, remainder_multiply , result, intermediate_result;

    result = 1;
    num_multiply = power / 2;
    remainder_multiply = power % 2;

    for(NUMBER i=0; i<num_multiply; i++) {
        intermediate_result = (base * base) % n;
        result *= intermediate_result;
    }

    if(remainder_multiply) {
        result *= base % n;
    }

    return result % n;
}

/*
 * Modo eficiente de calcular exponenciação usando operação do quadrado e multiplicação
 * com modulo n
 * 
 * @params base - Base da operação
 * @params power - Potência da operação
 * @params n - Module of operation
*/
NUMBER sqr_n_multiply_module(NUMBER base, NUMBER power, NUMBER n) {
    int index, bit;
    NUMBER result = base;
    
    index = find_leftmost_one_bit(power);
    
    for(int i = ++index; i < NUMBER_NUM_BITS; i++) {
        bit = get_bit_by_index(power, i);
        result = (result * result) % n;

        // Workaround, pois apenas recebo -1
        if(bit == -1) result = (result * base) % n;
    }
    return result;
}

/* 
 * Recebe um número formando definido como n = p*q e retorna p e q 
 * nas variáveis prime1 e prime2
 * 
 * @params number - Número definido como n = p*q com p e q primos
 * @params prime1 - Primeiro primo encontrado
 * @params prime2 - Segundo primo encontrado
*/
void prime_factorization(NUMBER number, NUMBER* prime1, NUMBER* prime2) {
    for(int i=3; i<100000; i+=2) {
        if(!(number % i)) {
            *prime1 = number / i;
            *prime2 = i;
            return;
        }
    }
}

/*
 * Calcula o phi de n em que n = p*q
 * 
 * @params p - Primo composto em n
 * @params q - Primo composto em n
 * @returns Calculo de phi de n
*/
NUMBER calc_phi_n(NUMBER p, NUMBER q) {
    return (p - 1) * (q - 1);
}

/*
 * Calcula o inverso modular de gdc(a, b)
 * 
 * @params a - Primeiro número
 * @params b - Segundo número
 * @returns Inverso modular de gdc(a, b)
*/
NUMBER extended_euclidean(NUMBER a, NUMBER b) {
    NUMBER remainder1, remainder2, s, t, s1, s2, t1, t2, quotient, aux;
    int i;

    remainder1 = a;
    remainder2 = b;
    s1 = 1;
    s2 = 0;
    t1 = 0;
    t2 = 1;
    i = 0;

    do {
        quotient = remainder1 / remainder2;
        aux = remainder1 % remainder2;
        remainder1 = remainder2;
        remainder2 = aux;
        
        s = s1 - quotient * s2;
        s1 = s2;
        s2 = s;
        t = t1 - quotient * t2;
        t1 = t2;
        t2 = t;

    } while(remainder2 != 0);
    
    return t1;
}


int main()
{
    NUMBER n, e, c, d, prime1, prime2, phi_n, message;

    // Recebe entradas
    scanf("%lld", &n);
    scanf("%lld", &e);
    scanf("%lld", &c);

    // Calcula d
    prime_factorization(n, &prime1, &prime2);
    
    // Calcula phi(n)
    phi_n = calc_phi_n(prime2, prime1);

    // Encotra o valor de d
    d = extended_euclidean(phi_n, e);

    if(d < 0) d += phi_n;

    // Desencripta a mensagem
    message = sqr_n_multiply_module(c, d, n);
    printf("%lld\n", message);
    return 0;
}
