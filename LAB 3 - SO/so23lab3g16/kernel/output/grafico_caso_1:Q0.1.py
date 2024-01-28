import re
import matplotlib.pyplot as plt

# Lee los resultados desde el archivo
with open('caso_1:Q0.1.txt', 'r') as file:
    lines = file.readlines()

# Expresiones regulares para buscar OPW y OPR en cada línea
pattern = r'(\d+): (\d+) OPW(\d+)T, (\d+) OPR(\d+)T'
opw_values = []
opr_values = []

for line in lines:
    match = re.search(pattern, line)
    if match:
        pid, opw, _, opr, _ = match.groups()
        opw_values.append(int(opw))
        opr_values.append(int(opr))

# Crea una lista de tiempo (puede ser simplemente el índice de la medición)
time_values = list(range(len(opw_values)))

# Calcula la media de OPR
mean_opr = sum(opr_values) / len(opr_values)

# Grafica los resultados
plt.plot(time_values, opw_values, label='OPW')
plt.plot(time_values, opr_values, label='OPR')

plt.xlabel('Tiempo')
plt.ylabel('Operaciones por unidad de tiempo')
plt.legend()
plt.title('Caso 1: Rendimiento 1 IOBench solo (Quantum 10 veces menor)')
plt.grid(True)

# Agrega etiquetas para el valor mínimo y máximo
min_opr = min(opr_values)
max_opr = max(opr_values)

plt.text(time_values[opr_values.index(min_opr)], min_opr, f'Min OPR: {min_opr}', fontsize=11, ha='left', va='bottom')
plt.text(time_values[opr_values.index(max_opr)], max_opr, f'Max OPR: {max_opr}', fontsize=11, ha='left', va='bottom')

# Agrega la media de OPR al gráfico
plt.text(time_values[-1], mean_opr, f'Media OPR: {mean_opr:.2f}', fontsize=11, ha='right', va='bottom')

plt.show()
