# Script Gnuplot pour tracer le graphe à partir de temps_calcul.txt, temps_seq_calcul.txt et temps_par_calcul.txt

# Définir le terminal de sortie et le fichier
set terminal png
set output "temps_calcul_comparison.png"

# Paramètres du graphique
set title "Comparaison des temps de calcul en fonction de TRIALS_PER_THREAD"
set xlabel "TRIALS_PER_THREAD"
set ylabel "Temps de calcul moyen (s)"
set grid

# Utilisation de points pour la clarté du tracé
set style data points

# Charger les données à partir des fichiers
plot "temps_calcul.txt" using 1:2 with linespoints title "Temps de calcul (omp target team)", \
     "temps_seq_calcul.txt" using 1:2 with linespoints title "Temps de calcul (séquentiel)", \
     "temps_par_calcul.txt" using 1:2 with linespoints title "Temps de calcul (parallèle omp )"

