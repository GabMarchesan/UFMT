#Gabriel Gomes Marchesan
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Passo 1: Carregar os dados dos arquivos CSV
df_soja = pd.read_csv('precos_soja.csv')
df_milho = pd.read_csv('precos_milho.csv')

# Ajustar a coluna de data (formato mês/ano) para datetime (usa o primeiro dia do mês)
df_soja['data'] = pd.to_datetime(df_soja['data'], format='%m/%Y')
df_milho['data'] = pd.to_datetime(df_milho['data'], format='%m/%Y')

# Renomear colunas para padronizar nomes
df_soja.rename(columns={'data': 'Data', 'valor_1': 'Preço_Soja'}, inplace=True)
df_milho.rename(columns={'data': 'Data', 'valor1': 'Preço_Milho'}, inplace=True)

# Passo 2: Caracterização inicial dos dados

print("Tipos dos atributos:")
print(df_soja.dtypes)
print(df_milho.dtypes)

print("\nEstatísticas descritivas da Soja (localização e dispersão):")
print(df_soja.describe())

print("\nEstatísticas descritivas do Milho (localização e dispersão):")
print(df_milho.describe())

# Passo 3: Visualização dos dados

plt.figure(figsize=(12, 6))

plt.subplot(1, 2, 1)
sns.boxplot(y=df_soja['Preço_Soja'])
plt.title('Boxplot dos preços da Soja')
plt.ylabel('Preço da Soja')

plt.subplot(1, 2, 2)
sns.boxplot(y=df_milho['Preço_Milho'])
plt.title('Boxplot dos preços do Milho')
plt.ylabel('Preço do Milho')

plt.tight_layout()
plt.show()

# Passo 4: Análise de correlação e covariância
# Para correlacionar preços, precisamos alinhar as datas. Vamos fazer um merge por Data

df_merged = pd.merge(df_soja[['Data', 'Preço_Soja']],
                     df_milho[['Data', 'Preço_Milho']],
                     on='Data', how='inner')

# Correlação entre preços
correlacao = df_merged['Preço_Soja'].corr(df_merged['Preço_Milho'])
print(f"\nCorrelação entre os preços da Soja e do Milho: {correlacao:.4f}")

# Covariância entre preços
covariancia = df_merged['Preço_Soja'].cov(df_merged['Preço_Milho'])
print(f"Covariância entre os preços da Soja e do Milho: {covariancia:.4f}")

# Gráfico de dispersão para visualização da relação entre os preços
plt.figure(figsize=(8, 6))
sns.scatterplot(data=df_merged, x='Preço_Soja', y='Preço_Milho', color='green')
plt.title('Dispersão: Preço da Soja vs Preço do Milho')
plt.xlabel('Preço da Soja')
plt.ylabel('Preço do Milho')
plt.show()
