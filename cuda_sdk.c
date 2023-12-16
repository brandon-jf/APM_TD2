#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define TOTAL_BLOCKS 2
#define THREADS_PER_BLOCK 2
//#define TRIALS_PER_THREAD 25000
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
    srand(time(NULL));
    double mean;
    int count = COUNT;
    int globalTotal = 0;
    double pi_approximation ;
    double pi_approx_mean ;
    int TRIALS_PER_THREAD = atoi(argv[1]);
    FILE *file = fopen("temps_calcul.txt", "a");
    if (file == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier.\n");
        return EXIT_FAILURE;
    }
    clock_t start_base = clock();
    
    for (int i = count; i > 0; i--)
        ;
    clock_t end_base = clock();

    // Calcul du nombre total de points
    int total_points = TOTAL_BLOCKS * THREADS_PER_BLOCK * TRIALS_PER_THREAD;

    // Tableau pour stocker le nombre de fléchettes à l'intérieur du cercle
    int *localInCircle = (int *)malloc(TOTAL_BLOCKS * THREADS_PER_BLOCK * sizeof(int));
    clock_t start = clock();
    for (int k = count; k > 0; k--) {
        // Initialisation du tableau de résultats partiels
        for (int i = 0; i < TOTAL_BLOCKS * THREADS_PER_BLOCK; ++i) {
            localInCircle[i] = 0;
        }

        // Utilisation de la directive omp target pour transférer le tableau vers le device
        #pragma omp target enter data map(to: localInCircle[0:TOTAL_BLOCKS*THREADS_PER_BLOCK])

        // Utilisation de la directive omp teams et omp distribute pour définir la configuration du kernel
        #pragma omp target teams distribute num_teams(TOTAL_BLOCKS) thread_limit(THREADS_PER_BLOCK) map(from: localInCircle[0:TOTAL_BLOCKS*THREADS_PER_BLOCK])
        for (int team_id = 0; team_id < THREADS_PER_BLOCK; ++team_id) {
            int thread_id = team_id * THREADS_PER_BLOCK + omp_get_thread_num();
            //printf("thread_id = %d * %d + %d \n", team_id, THREADS_PER_BLOCK, omp_get_thread_num());

            // Simulation des lancers de fléchettes pour chaque thread
            #pragma omp parallel
            for (int i = 0; i < TRIALS_PER_THREAD; ++i) {
                double x = random_double();
                double y = random_double();

                if (x * x + y * y <= 1) {
                    // Utilisation de la directive omp atomic pour éviter les conflits d'accès concurrents
                    #pragma omp atomic
                    localInCircle[thread_id]++;
                }
            }

            // Affichage des résultats locaux pour chaque thread
            //printf("Résultat local [%d]: %d\n", thread_id, localInCircle[thread_id]);
        }

        // Utilisation de la directive omp target pour transférer le tableau depuis le device vers le host
        #pragma omp target exit data map(from: localInCircle[0:TOTAL_BLOCKS*THREADS_PER_BLOCK])


        globalTotal = 0;
        // Calcul du total des fléchettes à l'intérieur du cercle
        for (int i = 0; i < TOTAL_BLOCKS * THREADS_PER_BLOCK; ++i) {
            globalTotal += localInCircle[i];
            //printf("tab[%d]: %d\n", i, localInCircle[i]);
        }
        
        // Affichage du total de points à l'intérieur du cercle et du nombre total de points
       // printf("Total de points à l'intérieur du cercle : %d\n", globalTotal);
       // printf("Nombre total de points : %d\n", total_points);

        // Calcul de l'approximation de π
        pi_approximation = 4.0 * globalTotal / total_points;
        pi_approx_mean = pi_approx_mean + pi_approximation ;
        // Affichage du résultat
        //printf("Approximation de pi : %f\n", pi_approximation);
    }
    pi_approx_mean =pi_approx_mean/count;
    printf("Approximation de pi : %f\n", pi_approx_mean);
    clock_t end = clock();

    // Calculate cost of single call.
    double elapsed_time = end - start - (end_base - start_base);
    elapsed_time = elapsed_time / CLOCKS_PER_SEC / COUNT;
    printf("temps de calcul : %lf\n", elapsed_time);
    fprintf(file, "%d %lf\n", TRIALS_PER_THREAD, elapsed_time);
    fclose(file);


    // Libération de la mémoire allouée pour le tableau
    free(localInCircle);

    return 0;
}


