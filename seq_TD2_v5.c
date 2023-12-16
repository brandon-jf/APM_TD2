#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define COUNT 100
// Fonction pour générer un nombre réel aléatoire entre 0 et 1
double random_double() {
    return (double)rand() / RAND_MAX;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Il faut donner le nombre de points\n");
        return EXIT_SUCCESS;
    }
    srand(time(NULL));  // Initialisation du générateur de nombres aléatoires
    int count = COUNT;
    int total_points = 1000000;  // Nombre total de fléchettes lancées
    int points_inside_circle = 0;  // Nombre de fléchettes à l'intérieur du cercle
    int TRIALS_PER_THREAD = atoi(argv[1]);
    double pi_approximation ;
    FILE *file = fopen("temps_seq_calcul.txt", "a");
    if (file == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier.\n");
        return EXIT_FAILURE;
    }
    clock_t start_base = clock();
    
    for (int i = count; i > 0; i--)
        ;
    clock_t end_base = clock();
    clock_t start = clock();
    for (int k = count; k > 0; k--) {
        points_inside_circle = 0;
        // Simulation des lancers de fléchettes
        for (int i = 0; i < total_points; ++i) {
            // Génération de coordonnées aléatoires dans le quart de cercle
            double x = random_double();
            double y = random_double();

            // Vérification si la fléchette est à l'intérieur du quart de cercle
            if (x * x + y * y <= 1) {
                points_inside_circle++;
            }
        }

        // Calcul de l'approximation de π
        pi_approximation = 4.0 * points_inside_circle / total_points;

        // Affichage du résultat
        
    }
    clock_t end = clock();
    printf("Approximation de pi: %f\n", pi_approximation);
    // Calculate cost of single call.
    double elapsed_time = end - start - (end_base - start_base);
    elapsed_time = elapsed_time / CLOCKS_PER_SEC / COUNT;
    printf("temps de calcul : %lf\n", elapsed_time);
    fprintf(file, "%d %lf\n", TRIALS_PER_THREAD, elapsed_time);
    fclose(file);


    return 0;
}

