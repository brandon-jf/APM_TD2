#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define TOTAL_THREADS 4
//#define TRIALS_PER_THREAD 250000 // Nombre de fléchettes par thread
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
    int TRIALS_PER_THREAD = atoi(argv[1]);
    double pi_approximation ;
    FILE *file = fopen("temps_par_calcul.txt", "a");
    if (file == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier.\n");
        return EXIT_FAILURE;
    }
    int total_points = TOTAL_THREADS * TRIALS_PER_THREAD;
    int points_inside_circle[TOTAL_THREADS];
    clock_t start_base = clock();
    
    for (int i = count; i > 0; i--)
        ;
    clock_t end_base = clock();
    clock_t start = clock();
    for (int k = count; k > 0; k--) {
        // Initialisation du tableau de résultats partiels
        for (int i = 0; i < TOTAL_THREADS; ++i) {
            points_inside_circle[i] = 0;
        }
         #pragma omp parallel num_threads(TOTAL_THREADS)
        {
            int thread_id = omp_get_thread_num();

            // Simulation des lancers de fléchettes pour chaque thread
            #pragma omp for
            for (int i = 0; i < TRIALS_PER_THREAD; ++i) {
                double x = random_double();
                double y = random_double();

                if (x * x + y * y <= 1) {
                    points_inside_circle[thread_id]++;
                }
            }
        }

        int total_inside_circle = 0;
        // Calcul du total des fléchettes à l'intérieur du cercle
        for (int i = 0; i < TOTAL_THREADS; ++i) {
            total_inside_circle += points_inside_circle[i];
        }

        // Calcul de l'approximation de π
        pi_approximation = 4.0 * total_inside_circle / total_points;

        // Affichage du résultat
        
    }
    printf("Approximation de pi: %f\n", pi_approximation);
    clock_t end = clock();

    // Calculate cost of single call.
    double elapsed_time = end - start - (end_base - start_base);
    elapsed_time = elapsed_time / CLOCKS_PER_SEC / COUNT;
    printf("temps de calcul : %lf\n", elapsed_time);
    fprintf(file, "%d %lf\n", TRIALS_PER_THREAD, elapsed_time);
    fclose(file);


    return 0;
}

