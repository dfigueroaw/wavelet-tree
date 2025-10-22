#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wavelet.h"

int main(void)
{
    u32 vals[] = {5, 1, 9, 3, 7, 2, 8, 6, 4, 10};
    size_t n = sizeof(vals) / sizeof(*vals);

    WaveletTree* tree = malloc(sizeof(*tree));
    wavelet_tree_from_vec(tree, vals, n);

    u32 kth = 0;

    kth = wavelet_tree_kth(tree, 3, 8, 2);
    printf("%d\n", kth);
    kth = wavelet_tree_kth(tree, 1, 10, 5);
    printf("%d\n", kth);
    kth = wavelet_tree_kth(tree, 4, 7, 3);
    printf("%d\n", kth);

    u32 leq = 0;

    leq = wavelet_tree_leq(tree, 3, 8, 5);
    printf("%d\n", leq);
    leq = wavelet_tree_leq(tree, 1, 10, 7);
    printf("%d\n", leq);
    leq = wavelet_tree_leq(tree, 4, 7, 9);
    printf("%d\n", leq);

    wavelet_tree_free(tree);

    const char* str = "wavelettree";
    size_t len = strlen(str);

    WaveletTree* stree = malloc(sizeof(*tree));
    wavelet_tree_from_string(stree, str);

    char skth = 0;

    skth = (char)wavelet_tree_kth(stree, 1, len, 1);
    printf("%c\n", skth);
    skth = (char)wavelet_tree_kth(stree, 1, len, 3);
    printf("%c\n", skth);
    skth = (char)wavelet_tree_kth(stree, 3, 8, 2);
    printf("%c\n", skth);

    u32 sleq = 0;

    sleq = wavelet_tree_leq(stree, 1, len, 'm');
    printf("%d\n", sleq);
    sleq = wavelet_tree_leq(stree, 2, 9, 't');
    printf("%d\n", sleq);
    sleq = wavelet_tree_leq(stree, 1, len, 'a');
    printf("%d\n", sleq);

    wavelet_tree_free(stree);

    return 0;
}
