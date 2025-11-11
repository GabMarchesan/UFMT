def f(x):
  # Insira a função f(x) aqui.
  return x**2

a = 0  # Limite inferior de integração
b = 1  # Limite superior de integração
Nmax = 100 # Valor máximo de N

with open('dados.txt', 'w') as arquivo:
  for N in range(1, Nmax + 1):
    Sn = 0
    Tn = 0
    for i in range(1, N + 1):
      # Cálculo do ponto médio (Sn)
      h = (b - a) / N
      xi = a + i * h
      Sn += f(xi) * h

      # Cálculo da área do trapézio (Tn)
      if i == 1 or i == N:
        Tn += f(xi) * h / 2
      else:
        Tn += f(xi) * h

    # Gravar os resultados no arquivo
    arquivo.write(f"{N} {Sn} {Tn}\n")

# Geração do Gráfico

import matplotlib.pyplot as plt

# Leitura dos dados do arquivo
dados = []
with open('dados.txt', 'r') as arquivo:
  for linha in arquivo:
    N, Sn, Tn = linha.split()
    dados.append((int(N), float(Sn), float(Tn)))

# Plot das curvas
plt.plot([d[0] for d in dados], [d[1] for d in dados], label="Retângulo")
plt.plot([d[0] for d in dados], [d[2] for d in dados], label="Trapézio")

# Configurações do gráfico
plt.xlabel("Número de Subintervalos (N)")
plt.ylabel("Valor Aproximado da Integral")
plt.legend()
plt.show()

# Impressão dos Resultados 

print(f"N\tSn\tTn")
with open('dados.txt', 'r') as arquivo:
  for linha in arquivo:
    N, Sn, Tn = linha.split()
    print(f"{N}\t{Sn}\t{Tn}")