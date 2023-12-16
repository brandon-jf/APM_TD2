#!/bin/bash

# Nom du fichier de sortie
output_file="temps_calcul.txt"

# Supprime le fichier de sortie s'il existe déjà
rm -f "$output_file"

# Boucle for pour différentes valeurs de TRIALS_PER_THREAD
for TRIALS_PER_THREAD in {1000..100000..1000}; do
    echo "Exécution pour TRIALS_PER_THREAD = $TRIALS_PER_THREAD"
    
    # Exécution de votre programme omp_TD2_v5 avec la valeur actuelle de TRIALS_PER_THREAD
    ./omp_TD2_v5 "$TRIALS_PER_THREAD"
    ./seq_TD2_v5 "$TRIALS_PER_THREAD"
    ./par_TD2_v5 "$TRIALS_PER_THREAD"
    # Attendre un court instant pour laisser le système de fichiers se mettre à jour (facultatif)
    sleep 0.1
done

